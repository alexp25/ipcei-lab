---
title: "L5 - LPADC - Masurarea tensiunilor analogice"
description: "Configurare LPADC0, scanarea canalelor SW3 si Arduino A0-A5, conversie in volti"
nav_order: 6
parent: Lecții FRDM-MCXA153
layout: lesson
---

# L5 - LPADC - Masurarea tensiunilor analogice

**Configurare cu MCUXpresso Config Tools, calibrare, software trigger si afisarea tensiunilor in consola**

---

| | |
|---|---|
| **Ziua / Sesiunea** | Ziua 3, 24 iunie - dimineata |
| **Periferic** | `ADC0 (LPADC) - command queue - result FIFO - VDDA 3.3 V` |
| **Durata** | 2h (09:00-11:00) |
| **Responsabil** | Cadru didactic UPB |
| **Hardware** | FRDM-MCXA153, cablu USB-C si surse analogice de maximum 3.3 V |
| **Proiect** | `src/lab_adc/main` |

## Context si motivatie

Convertorul LPADC al microcontrolerului MCXA153 foloseste o arhitectura bazata pe comenzi de conversie, triggere si un FIFO de rezultate. Fluxul este diferit de ADC-urile simple la care selectarea canalului si pornirea conversiei se fac printr-un singur registru.

Aplicatia realizata in acest laborator:

1. configureaza pinii analogici din MCUXpresso Config Tools;
2. initializeaza si calibreaza `ADC0`;
3. foloseste rezolutia high-resolution de 16 biti;
4. selecteaza succesiv fiecare canal analogic;
5. porneste conversia prin software trigger;
6. converteste valoarea raw in milivolti;
7. afiseaza rezultatele in consola seriala.

> **Important:** Pe MCXA153 se foloseste driverul `fsl_lpadc.h`. Driverul `ADC16`, intalnit pe alte familii NXP, nu este compatibil cu acest proiect.

## Obiective

La finalul laboratorului, studentul va putea:

1. identifica pinii si canalele ADC pentru `SW3` si conectorul Arduino;
2. configura pinii ca intrari analogice in fisierul `.mex`;
3. initializa si calibra LPADC;
4. configura o comanda si un software trigger;
5. citi rezultatul din FIFO prin polling;
6. transforma valoarea raw intr-o tensiune;
7. verifica rezultatele prin consola seriala.

## Maparea intrarilor analogice

Aplicatia citeste sapte intrari:

| Nume in aplicatie | Pin MCU | Conector placa | Canal LPADC |
|---|---|---|---|
| `SW3` | `P1_7` | buton SW3 / J1[1] | `ADC0_A23` |
| Arduino `A0` | `P1_10` | J4[2] | `ADC0_A8` |
| Arduino `A1` | `P1_12` | J4[4] | `ADC0_A10` |
| Arduino `A2` | `P1_13` | J4[6] | `ADC0_A11` |
| Arduino `A3` | `P2_0` | J4[8] | `ADC0_A0` |
| Arduino `A4` | `P3_31` | J4[10] | `ADC0_A12` |
| Arduino `A5` | `P3_30` | J4[12] | `ADC0_A13` |

Tensiunea aplicata pe un pin ADC trebuie sa ramana in intervalul `0 V ... VDDA`. In configuratia laboratorului, `VDDA` este considerata `3.3 V`.

Butonul `SW3` poate fi observat analogic: tensiunea canalului se modifica atunci cand butonul este apasat sau eliberat.

## Configurarea pinilor in MCUXpresso Config Tools

Deschideti fisierul:

```text
src/lab_adc/main/frdmmcxa153/main.mex
```

In instrumentul **Pins**, verificati functia `BOARD_InitPins` si configurati:

| Eticheta | Semnal |
|---|---|
| `SW3` | `ADC0_A23` |
| `ARDUINO_A0` | `ADC0_A8` |
| `ARDUINO_A1` | `ADC0_A10` |
| `ARDUINO_A2` | `ADC0_A11` |
| `ARDUINO_A3` | `ADC0_A0` |
| `ARDUINO_A4` | `ADC0_A12` |
| `ARDUINO_A5` | `ADC0_A13` |

Pentru fiecare pin analogic:

- `Pull Enable` trebuie sa fie `Disabled`;
- `Input Buffer Enable` trebuie sa fie `Disabled`;
- multiplexarea trebuie sa fie functia analogica `ADC0_Ax`.

Dupa generarea codului, configuratia apare in:

```text
frdmmcxa153/main/pin_mux.c
frdmmcxa153/main/pin_mux.h
```

`BOARD_InitPins()` activeaza ceasurile pentru porturi, elibereaza perifericele din reset si configureaza pinii ADC cu bufferul digital dezactivat.

## Activarea driverului LPADC

Driverul trebuie activat in configuratia proiectului:

```ini
CONFIG_MCUX_COMPONENT_driver.lpadc=y
```

Codul include:

```c
#include "fsl_lpadc.h"
```

## Structura aplicatiei

### Lista canalelor

Canalele sunt descrise intr-un tabel, astfel incat aceeasi functie de citire sa poata fi folosita pentru toate intrarile:

```c
typedef struct
{
    const char *name;
    uint32_t channel;
} adc_input_t;

static const adc_input_t adc_inputs[] = {
    {"SW3", 23U},
    {"A0", 8U},
    {"A1", 10U},
    {"A2", 11U},
    {"A3", 0U},
    {"A4", 12U},
    {"A5", 13U},
};
```

### Initializarea ADC

Ceasul ADC este obtinut din FRO de 12 MHz:

```c
CLOCK_SetClockDiv(kCLOCK_DivADC0, 1U);
CLOCK_AttachClk(kFRO12M_to_ADC0);
RESET_ReleasePeripheralReset(kADC0_RST_SHIFT_RSTn);
```

Configuratia foloseste referinta `VDDA` si modul high-resolution:

```c
LPADC_GetDefaultConfig(&adc_config);
adc_config.powerLevelMode = kLPADC_PowerLevelAlt4;
adc_config.enableAnalogPreliminary = true;
adc_config.referenceVoltageSource = kLPADC_ReferenceVoltageAlt3;
LPADC_Init(ADC0, &adc_config);

LPADC_DoOffsetCalibration(ADC0);
LPADC_DoAutoCalibration(ADC0);
```

Calibrarea este efectuata o singura data, dupa initializarea perifericului.

### Comanda si triggerul

Aplicatia foloseste:

- command ID `1`;
- trigger ID `0`;
- software trigger;
- conversie high-resolution.

```c
LPADC_GetDefaultConvTriggerConfig(&trigger_config);
trigger_config.targetCommandId = ADC_COMMAND_ID;
trigger_config.enableHardwareTrigger = false;
LPADC_SetConvTriggerConfig(ADC0, ADC_TRIGGER_ID, &trigger_config);
```

### Citirea unui canal

Canalul este introdus in configuratia comenzii inaintea fiecarei conversii:

```c
static uint16_t read_adc_channel(uint32_t channel)
{
    lpadc_conv_command_config_t command_config;
    lpadc_conv_result_t result;

    LPADC_GetDefaultConvCommandConfig(&command_config);
    command_config.channelNumber = channel;
    command_config.conversionResolutionMode =
        kLPADC_ConversionResolutionHigh;
    LPADC_SetConvCommandConfig(ADC0, ADC_COMMAND_ID, &command_config);

    LPADC_DoSoftwareTrigger(ADC0, ADC_TRIGGER_MASK);
    while (!LPADC_GetConvResult(ADC0, &result))
    {
    }

    return result.convValue;
}
```

`LPADC_GetConvResult()` returneaza `false` pana cand FIFO-ul contine un rezultat valid.

## Conversia in tensiune

In modul high-resolution, aplicatia foloseste domeniul:

```text
raw = 0 ... 65535
```

Pentru referinta de `3300 mV`:

```text
voltage_mV = raw * 3300 / 65535
```

Implementarea adauga jumatate din numitor pentru rotunjire:

```c
uint32_t voltage_mv =
    ((uint32_t)raw_value * 3300U + (65535U / 2U)) / 65535U;
```

Produsul maxim incape intr-un `uint32_t`:

```text
65535 * 3300 = 216265500
```

## Bucla principala

Toate intrarile sunt citite la fiecare `500 ms`:

```c
for (size_t i = 0U; i < ARRAY_SIZE(adc_inputs); i++)
{
    uint16_t raw_value = read_adc_channel(adc_inputs[i].channel);
    /* conversie si PRINTF */
}

SDK_DelayAtLeastUs(500000U, SystemCoreClock);
```

In codul proiectului, dimensiunea tabelului este calculata cu `sizeof`.

## Consola seriala

Setari:

- `115200 baud`;
- `8` biti de date;
- fara paritate;
- `1` bit de stop;
- fara flow control.

Exemplu de iesire:

```text
ADC voltage measurement: SW3 and Arduino A0-A5
SW3     (ADC0_A23)     raw = 65410     voltage = 3.294 V
A0      (ADC0_A8)      raw = 32760     voltage = 1.650 V
A1      (ADC0_A10)     raw = 0         voltage = 0.000 V
A2      (ADC0_A11)     raw = 0         voltage = 0.000 V
A3      (ADC0_A0)      raw = 0         voltage = 0.000 V
A4      (ADC0_A12)     raw = 0         voltage = 0.000 V
A5      (ADC0_A13)     raw = 0         voltage = 0.000 V
```

Pinii neconectati pot avea valori instabile. O intrare analogica flotanta nu reprezinta automat `0 V`.

## Compilare

Din directorul proiectului:

```powershell
cd src/lab_adc/main
cmake --preset debug
cmake --build --preset debug
```

Pentru varianta release:

```powershell
cmake --preset release
cmake --build --preset release
```

Fisierele rezultate sunt:

```text
debug/main.elf
debug/main.bin
release/main.elf
release/main.bin
```

## Verificare practica

1. Conectati placa prin portul MCU-Link.
2. Deschideti terminalul serial la `115200 8N1`.
3. Programati fisierul `main.bin`.
4. Aplicati o tensiune cunoscuta intre `0 V` si `3.3 V` pe unul dintre pinii `A0-A5`.
5. Conectati masa sursei externe la `GND` pe placa.
6. Comparati valoarea afisata cu un multimetru.
7. Apasati `SW3` si observati modificarea tensiunii pe `ADC0_A23`.

## Capcane critice

- Nu aplicati tensiuni negative sau mai mari decat `VDDA`.
- Toate echipamentele trebuie sa aiba masa comuna.
- Bufferul digital trebuie dezactivat pentru pinii analogici.
- Nu activati rezistentele interne pull-up/pull-down pentru masuratori analogice externe.
- `ADC_COMMAND_ID` trebuie sa fie diferit de zero.
- Triggerul trebuie executat inainte de citirea rezultatului.
- Canalele Arduino nu corespund numeric cu numele lor: de exemplu, Arduino `A0` este `ADC0_A8`.
- O intrare neconectata este flotanta si poate produce valori aparent aleatoare.
- Formula pe 12 biti, cu numitorul `4095`, este gresita pentru configuratia high-resolution folosita in acest proiect.
- Configuratia trebuie modificata in `frdmmcxa153/main.mex`, fisierul `.mex` folosit de build.

## Exercitii

1. Calculati rezolutia teoretica in microvolti per bit pentru `VDDA = 3.3 V`.
2. Calculati media a 16 conversii pentru fiecare canal.
3. Afisati numai canalele a caror tensiune s-a modificat cu peste `10 mV`.
4. Inlocuiti delay-ul blocant cu un timer periodic.
5. Estimati eroarea masurarii folosind un multimetru de referinta.

## Deliverable

Aplicatia trebuie sa:

- configureze in `.mex` intrarile `SW3` si Arduino `A0-A5`;
- compileze cu driverul LPADC;
- calibreze ADC-ul;
- citeasca toate cele sapte canale prin software trigger;
- afiseze valoarea raw si tensiunea in volti;
- documenteze maparea pin-canal si formula de conversie.

---

[← L4: Timer & PWM - SCTimer si CTIMER](../l4-timer-pwm) · [L6: LPSPI - Comunicatie SPI si dispozitiv extern →](../l6-lpspi)
