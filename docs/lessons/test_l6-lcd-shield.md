---
title: "L6 - LCD 1602 si tastatura analogica"
description: "Integrarea unui LCD Keypad Shield in 4-bit mode, configurare GPIO si LPADC, MCUXpresso Config Tools"
nav_order: 7
parent: Lecții FRDM-MCXA153
layout: lesson
---

# L6 - LCD 1602 si tastatura analogica

**Adaugarea unei aplicatii noi intr-un proiect MCUXpresso existent si alinierea codului cu MCUXpresso Config Tools**

---

| | |
|---|---|
| **Ziua / Sesiunea** | Ziua 3, 24 iunie - dupa-amiaza |
| **Periferice** | `GPIO - ADC0/LPADC - Arduino headers - HD44780` |
| **Durata** | 2h (13:00-15:00) |
| **Responsabil** | Cadru didactic UPB |
| **Hardware** | FRDM-MCXA153 si Arduino LCD Keypad Shield 1602 |
| **Proiect** | `src/lab_lcd/main` |

## Context si motivatie

In acest laborator adaugam suport pentru un **Arduino LCD Keypad Shield 1602** intr-un proiect MCUXpresso SDK existent.

Shield-ul contine:

- un LCD 16x2 compatibil HD44780;
- o interfata paralela in mod 4 biti;
- cinci butoane conectate printr-o retea de rezistente la Arduino `A0`;
- un semnal optional pentru controlul iluminarii de fundal.

Exercitiul nu urmareste doar functionarea LCD-ului. Accentul este pus pe procesul complet de integrare:

1. inspectarea structurii si conventiilor proiectului existent;
2. separarea codului reutilizabil de codul specific placii;
3. configurarea pinilor in fisierul `.mex`;
4. regenerarea fisierelor cu MCUXpresso Config Tools;
5. identificarea modificarilor produse de generator;
6. alinierea CMake si a metadatelor proiectului cu noile fisiere generate;
7. verificarea finala prin compilare.

Acest flux este important in proiectele embedded reale. Codul sursa, fisierele generate si configuratia build-ului trebuie sa descrie acelasi hardware.

## Obiective

La finalul laboratorului, studentul va putea:

1. identifica maparea pinilor Arduino pe MCXA153;
2. explica secventa de initializare HD44780 in mod 4 biti;
3. citi o tastatura analogica prin LPADC;
4. scala praguri de tip Arduino 10 biti pentru un ADC de 12 biti;
5. implementa filtrare prin citiri stabile consecutive;
6. organiza codul pe niveluri reutilizabile si specifice placii;
7. adauga pinii in MCUXpresso Config Tools;
8. interpreta modificarile produse de generator;
9. corecta sursele si include path-urile folosite de CMake;
10. verifica faptul ca fisierele generate sunt sursa reala folosita de build.

## Arhitectura hardware

### LCD HD44780 in mod 4 biti

Controllerul HD44780 poate transfera un octet folosind:

- 8 linii de date; sau
- 4 linii de date, in doua transferuri succesive.

Shield-ul foloseste modul 4 biti. Pentru fiecare octet:

1. se scrie nibble-ul superior pe `D4-D7`;
2. se genereaza un impuls pe `EN`;
3. se scrie nibble-ul inferior;
4. se genereaza un nou impuls pe `EN`.

Semnalul `RS` selecteaza tipul transferului:

- `RS = 0`: comanda;
- `RS = 1`: caracter pentru memoria DDRAM.

Linia `RW` a shield-ului este conectata uzual la masa. Driverul este astfel **write-only** si foloseste delay-uri in locul citirii flagului `busy`.

### Tastatura analogica

Butoanele `RIGHT`, `UP`, `DOWN`, `LEFT` si `SELECT` sunt conectate printr-un divizor rezistiv la `A0`.

Fiecare buton produce un domeniu diferit de tensiune. Aplicatia citeste ADC-ul si compara valoarea cu praguri configurabile.

Butonul `RESET` nu face parte din tastatura analogica. El actioneaza resetul hardware al placii si nu trebuie raportat ca tasta.

## Maparea shield-ului

Maparea folosita de proiect este:

| Semnal shield | Pin Arduino | Pin MCXA153 | Rol |
|---|---|---|---|
| LCD `RS` | `D8` | `P2_7` | selectie comanda/date |
| LCD `EN` | `D9` | `P3_31` | impuls de validare |
| LCD `D4` | `D4` | `P2_5` | bit de date 0 |
| LCD `D5` | `D5` | `P3_12` | bit de date 1 |
| LCD `D6` | `D6` | `P3_13` | bit de date 2 |
| LCD `D7` | `D7` | `P3_1` | bit de date 3 |
| Backlight | `D10` | `P3_11` | control optional |
| Keypad | `A0` | `P1_10 / ADC0_A8` | retea rezistiva |

`P3_12` si `P3_13` sunt conectati si la LED-urile rosu si verde de pe placa. Aceste LED-uri pot palpai in timpul transferurilor catre LCD.

## Compatibilitate electrica

FRDM-MCXA153 foloseste GPIO si ADC la **3.3 V**.

> Nu aplicati 5 V direct pe un GPIO sau pe intrarea ADC a microcontrolerului.

Multe variante de LCD Keypad Shield sunt proiectate pentru placi Arduino alimentate la 5 V. Trebuie verificata schema shield-ului.

### Intrarea A0

Unele shield-uri alimenteaza reteaua rezistiva a butoanelor din 5 V. In acest caz, tensiunea de pe `A0` poate depasi 3.3 V.

Solutii:

- alimentarea unui shield verificat ca fiind compatibil la 3.3 V;
- un divizor rezistiv;
- un circuit de adaptare de nivel.

Un divizor suplimentar modifica valorile ADC. Pragurile trebuie masurate si ajustate.

### Semnalele LCD

Aplicatia nu citeste magistrala LCD, deci LCD-ul nu conduce liniile `D4-D7` catre MCU. Totusi, un controller alimentat la 5 V poate sa nu garanteze recunoasterea nivelului logic de 3.3 V ca HIGH.

Folositi:

- un modul LCD compatibil la 3.3 V; sau
- adaptare de nivel pe semnalele de control si date.

### Backlight D10

Controlul iluminarii este dezactivat implicit:

```c
#define LCD_SHIELD_ENABLE_BACKLIGHT_CONTROL 0
```

Shield-urile existente folosesc circuite si polaritati diferite pentru D10. Activati controlul numai dupa verificarea schemei.

## Inspectarea proiectului existent

Inainte de implementare au fost verificate:

| Fisier | Rol |
|---|---|
| `main.c` | structura aplicatiei si bucla principala |
| `CMakeLists.txt` | inregistrarea surselor si include path-urilor |
| `frdmmcxa153/prj.conf` | componentele SDK activate |
| `main.mex` | configuratia folosita de MCUXpresso Config Tools |
| `frdmmcxa153/board_files.cmake` | fisierele specifice placii |
| `frdmmcxa153/main/hardware_init.c` | secventa `BOARD_InitHardware()` |
| `cfg_tools/board/*` | fisierele generate pentru pini, ceasuri si periferice |

API-urile deja folosite in proiect au fost pastrate:

- `GPIO_PinWrite()` si `GPIO_PinInit()`;
- `LPADC_*`;
- `SDK_DelayAtLeastUs()`;
- `PRINTF()`;
- `BOARD_InitHardware()`.

Aceasta inspectie evita introducerea unui HAL paralel sau a unor conventii incompatibile cu restul proiectului.

## Arhitectura software

Codul este impartit in patru niveluri:

```text
main.c
  |
  +-- hd44780.c/.h
  |     Driver LCD reutilizabil
  |
  +-- lcd_keypad.c/.h
  |     Praguri, scalare si debounce
  |
  +-- frdmmcxa153_lcd_shield.c/.h
        Mapare si integrare cu SDK-ul NXP
             |
             +-- cfg_tools/board/pin_mux.*
             +-- fsl_gpio.h
             +-- fsl_lpadc.h
```

### De ce separare?

`hd44780.c` nu trebuie sa cunoasca:

- numele placii;
- perifericul GPIO NXP;
- numerele pinilor Arduino.

`lcd_keypad.c` nu trebuie sa cunoasca:

- ADC0;
- canalul 8;
- registrele MCXA153.

Doar adaptorul `frdmmcxa153_lcd_shield.c` leaga driverele reutilizabile de hardware.

## Driverul HD44780

Driverul foloseste callback-uri:

```c
typedef struct
{
    void *context;
    hd44780_control_write_t write_rs;
    hd44780_control_write_t write_enable;
    hd44780_data_write_t write_data;
    hd44780_delay_t delay_us;
} hd44780_config_t;
```

Secventa de initializare:

1. asteptare dupa alimentare;
2. trimiterea repetata a nibble-ului `0x3`;
3. trecerea in mod 4 biti cu `0x2`;
4. `Function Set`: 4 biti, 2 linii;
5. display ON;
6. clear display;
7. entry mode.

API-ul public permite:

```c
HD44780_Init(&lcd, &config);
HD44780_Clear(&lcd);
HD44780_SetCursor(&lcd, column, row);
HD44780_WriteString(&lcd, "text");
HD44780_WriteLine(&lcd, row, "text");
```

`HD44780_WriteLine()` completeaza linia cu spatii pana la 16 caractere. Astfel, un text nou mai scurt nu lasa caractere vechi pe ecran.

## Driverul tastaturii

Pragurile implicite sunt exprimate ca valori ADC Arduino pe 10 biti:

| Buton | Domeniu |
|---|---|
| `RIGHT` | `0-80` |
| `UP` | `81-200` |
| `DOWN` | `201-400` |
| `LEFT` | `401-600` |
| `SELECT` | `601-800` |
| `NONE` | `>800` |

Configuratia contine pragurile, domeniul ADC si numarul de esantioane stabile:

```c
typedef struct
{
    void *context;
    lcd_keypad_read_adc_t read_adc;
    uint16_t adc_max_value;
    uint8_t stable_sample_count;
    lcd_keypad_thresholds_t thresholds_10bit;
} lcd_keypad_config_t;
```

### Scalare 12 biti la 10 biti

ADC-ul este citit in domeniul:

```text
0 ... 4095
```

Pragurile sunt definite in domeniul:

```text
0 ... 1023
```

Conversia este:

```text
value_10bit = raw_12bit * 1023 / 4095
```

Implementarea foloseste aritmetica pe 32 biti si rotunjire.

### Filtrare

Aplicatia necesita trei citiri consecutive pentru acelasi buton:

```c
shield->keypad.stable_sample_count = 3U;
```

La o perioada de polling de `5 ms`, schimbarea este validata dupa aproximativ `15 ms`.

### Timeout ADC

Citirea rezultatului nu asteapta infinit:

```c
uint32_t timeout = KEYPAD_ADC_TIMEOUT;

while (timeout > 0U)
{
    if (LPADC_GetConvResult(ADC0, &result))
    {
        /* rezultat valid */
    }
    timeout--;
}
```

Un periferic neconfigurat corect nu trebuie sa blocheze permanent aplicatia.

## Configurarea pinilor in MCUXpresso Config Tools

Fisierul canonic al proiectului este:

```text
src/lab_lcd/main/main.mex
```

In instrumentul **Pins**, functia `BOARD_InitPins` contine:

| Identifier | Configuratie |
|---|---|
| `LCD_D4` | `GPIO2, 5`, output, initial LOW |
| `LCD_D5` | `GPIO3, 12`, output, initial LOW |
| `LCD_D6` | `GPIO3, 13`, output, initial LOW |
| `LCD_D7` | `GPIO3, 1`, output, initial LOW |
| `LCD_RS` | `GPIO2, 7`, output, initial LOW |
| `LCD_EN` | `GPIO3, 31`, output, initial LOW |
| `LCD_BACKLIGHT` | `GPIO3, 11`, output, initial LOW |
| `LCD_KEYPAD_A0` | `ADC0_A8` |

Pentru `LCD_KEYPAD_A0`:

- `Pull Enable = Disabled`;
- `Input Buffer Enable = Disabled`;
- semnalul este `ADC0 A,8`.

Fisierele generate sunt:

```text
cfg_tools/board/pin_mux.c
cfg_tools/board/pin_mux.h
cfg_tools/board/clock_config.c
cfg_tools/board/clock_config.h
cfg_tools/board/peripherals.c
cfg_tools/board/peripherals.h
cfg_tools/board/RTE_Device.h
```

Adaptorul foloseste simbolurile generate:

```c
BOARD_INITPINS_LCD_RS_GPIO
BOARD_INITPINS_LCD_RS_PIN
BOARD_INITPINS_LCD_EN_GPIO
BOARD_INITPINS_LCD_D4_GPIO
BOARD_INITPINS_LCD_KEYPAD_A0_PORT
```

Nu se folosesc numere GPIO hard-coded in driverul reutilizabil.

## Ce s-a intamplat dupa regenerare

Configurarea initiala a proiectului folosea fisiere generate in:

```text
frdmmcxa153/main/pin_mux.*
frdmmcxa153/frdmmcxa153/clock_config.*
main/peripherals.*
```

Dupa rularea MCUXpresso Config Tools, generatorul a scris fisierele in:

```text
cfg_tools/board/*
```

Unele dintre fisierele vechi au fost eliminate, dar CMake continua sa le refere. Build-ul a esuat cu un mesaj de forma:

```text
ninja: error: ... clock_config.c, missing and no known rule to make it
```

Problema nu era in driverul LCD. Configuratia proiectului si fisierele generate nu mai aveau aceeasi structura.

## Corectiile de aliniere

### 1. Sursele specifice placii

`frdmmcxa153/board_files.cmake` si `frdmmcxa153/cfg_tools_generated.cmake` au fost actualizate pentru:

```cmake
../cfg_tools/board/clock_config.c
../cfg_tools/board/pin_mux.c
../cfg_tools/board/peripherals.c
```

Directorul `cfg_tools/board` a fost adaugat in include path.

### 2. Eliminarea surselor duplicate

Vechile:

```text
main/peripherals.c
main/peripherals.h
```

nu mai sunt compilate. Proiectul foloseste versiunea generata din `cfg_tools/board`.

Compilarea ambelor versiuni ar produce initializari duplicate sau simboluri definite de mai multe ori.

### 3. Fisierul `.mex` canonic

CMake a fost configurat sa foloseasca fisierul din radacina proiectului:

```cmake
mcux_add_config_mex_path(PATH .)
```

Astfel, proiectul nu alterneaza intre doua fisiere `.mex` din directoare diferite.

### 4. Metadatele Configuration Tools

`cfg_tools/project_info.json` a fost actualizat pentru a indica:

```text
cfg_tools/board/clock_config.*
cfg_tools/board/pin_mux.*
cfg_tools/board/peripherals.*
```

Metadatele invechite pot determina o regenerare viitoare sa recreeze structura gresita.

### 5. O singura sursa pentru initializarea pinilor

`BOARD_InitPins()` generat configureaza:

- clock gate-urile PORT si GPIO;
- reseturile perifericelor;
- multiplexarea;
- proprietatile electrice;
- directia GPIO;
- starea initiala.

Adaptorul shield-ului nu mai repeta aceste operatii.

El pastreaza doar:

- scrierea semnalelor LCD;
- initializarea LPADC;
- citirea ADC;
- configurarea callback-urilor.

Aceasta regula este esentiala: modificarile facute in Pins Tool trebuie sa fie respectate de aplicatie, nu suprascrise ulterior de cod manual.

## Initializarea LPADC

Componenta SDK este activata in:

```text
frdmmcxa153/prj.conf
```

prin:

```ini
CONFIG_MCUX_COMPONENT_driver.lpadc=y
```

Adaptorul configureaza:

- `ADC0`;
- canalul `ADC0_A8`;
- command ID `1`;
- trigger ID `0`;
- software trigger;
- rezolutie standard de 12 biti;
- referinta `VDDA`.

```c
LPADC_GetDefaultConvCommandConfig(&command_config);
command_config.channelNumber = 8U;
command_config.conversionResolutionMode =
    kLPADC_ConversionResolutionStandard;
LPADC_SetConvCommandConfig(ADC0, 1U, &command_config);
```

## Aplicatia demo

Demo-ul poate fi activat sau dezactivat:

```c
#define LCD_KEYPAD_DEMO_ENABLE 1
```

La pornire, LCD-ul afiseaza:

```text
FRDM-MCXA153
Key: NONE
```

Bucla principala:

1. citeste tastatura;
2. aplica filtrarea;
3. compara butonul stabil cu cel afisat;
4. actualizeaza LCD-ul numai daca valoarea s-a schimbat;
5. scrie numele butonului si in consola.

Valori posibile:

```text
RIGHT
UP
DOWN
LEFT
SELECT
NONE
```

## Compilare si verificare

Din directorul proiectului:

```powershell
cd src/lab_lcd/main
cmake --preset debug
cmake --build --preset debug
```

Fisierele rezultate:

```text
debug/main.elf
debug/main.bin
```

Verificati si lista surselor active:

```text
debug/main_source_list.txt
```

Aceasta trebuie sa contina:

```text
cfg_tools/board/clock_config.c
cfg_tools/board/pin_mux.c
cfg_tools/board/peripherals.c
```

Nu trebuie sa contina referinte catre fisiere generate sterse.

## Verificare practica

1. Verificati tensiunile shield-ului inainte de conectare.
2. Conectati shield-ul la headerele Arduino.
3. Conectati placa prin portul MCU-Link.
4. Compilati si programati `debug/main.bin`.
5. Deschideti terminalul la `115200 8N1`.
6. Verificati mesajul de pornire pe LCD.
7. Apasati fiecare buton.
8. Comparati numele de pe LCD cu mesajul din consola.
9. Daca pragurile sunt gresite, masurati valorile ADC reale si ajustati configuratia.

## Diagnostic

### LCD-ul nu afiseaza nimic

Verificati:

- alimentarea si contrastul;
- maparea `RS`, `EN`, `D4-D7`;
- nivelurile logice 3.3 V / 5 V;
- secventa si delay-urile HD44780;
- daca build-ul foloseste `cfg_tools/board/pin_mux.c`.

### LCD-ul afiseaza caractere incorecte

Posibile cauze:

- ordinea `D4-D7` este gresita;
- impulsul `EN` este prea scurt;
- initializarea in 4 biti este incompleta;
- nivelul HIGH de 3.3 V nu este acceptat de LCD-ul alimentat la 5 V.

### Butonul raportat este gresit

Verificati:

- tensiunea maxima de pe A0;
- masa comuna;
- valorile ADC reale;
- efectul unui divizor de protectie;
- pragurile configurate.

### Build-ul esueaza dupa regenerare

Verificati:

- calea fisierelor generate din `.mex`;
- `board_files.cmake`;
- `cfg_tools_generated.cmake`;
- include path-urile;
- sursele duplicate;
- `cfg_tools/project_info.json`;
- o noua rulare `cmake --preset debug`.

## Capcane critice

- Nu conectati un A0 de 5 V direct la ADC-ul MCXA153.
- Nu presupuneti ca toate shield-urile LCD Keypad au aceeasi schema.
- Nu tratati RESET ca buton analogic.
- Nu folositi praguri 10 biti direct pe o valoare ADC de 12 biti fara scalare.
- Nu asteptati infinit rezultatul ADC.
- Nu reconfigurati manual pinii dupa `BOARD_InitPins()` daca Pins Tool trebuie sa ramana sursa de adevar.
- Nu compilati simultan fisierele `peripherals.c` vechi si cele regenerate.
- Nu presupuneti ca generatorul pastreaza vechiul director de output.
- Dupa regenerare, rulati intotdeauna configurarea CMake, nu numai build-ul incremental.
- `P3_12` si `P3_13` sunt partajati cu LED-urile onboard.

## Exercitii

1. Afisati valoarea ADC bruta pe a doua linie a LCD-ului.
2. Masurati valorile reale pentru fiecare buton si definiti praguri personalizate.
3. Inlocuiti cele trei citiri consecutive cu un filtru bazat pe timp.
4. Implementati detectarea evenimentelor `pressed` si `released`.
5. Adaugati repetare automata pentru `UP` si `DOWN`.
6. Mutati perioada de polling pe un timer, fara delay blocant in bucla principala.
7. Activati controlul backlight-ului numai dupa verificarea schemei shield-ului.
8. Mutati un semnal LCD in Pins Tool, regenerati codul si demonstrati ca adaptorul foloseste noul simbol generat.

## Prompt recomandat pentru un asistent AI

```text
Lucrez intr-un proiect MCUXpresso SDK existent pentru FRDM-MCXA153.
Vreau sa adaug un Arduino LCD Keypad Shield 1602:
- HD44780, LCD 16x2, mod paralel 4 biti;
- RS=D8, EN=D9, D4=D4, D5=D5, D6=D6, D7=D7;
- backlight optional pe D10;
- butoane RIGHT, UP, DOWN, LEFT, SELECT prin divizor rezistiv pe A0.

Inainte de modificari:
1. inspecteaza pattern-urile GPIO, LPADC, delay, logging, pin mux si CMake;
2. identifica maparea Arduino pentru FRDM-MCXA153;
3. pastreaza stilul proiectului.

Arhitectura ceruta:
- driver HD44780 reutilizabil;
- driver keypad reutilizabil, cu praguri configurabile;
- adaptor separat pentru FRDM-MCXA153;
- demo separat si usor de dezactivat.

Praguri Arduino 10 biti:
- RIGHT 0-80;
- UP 81-200;
- DOWN 201-400;
- LEFT 401-600;
- SELECT 601-800;
- NONE peste 800.

Scaleaza pragurile pentru ADC-ul de 12 biti, adauga debounce prin citiri stabile
si timeout pentru polling ADC.

Adauga pinii in main.mex si foloseste simbolurile BOARD_INITPINS_LCD_* generate.
Dupa regenerarea MCUXpresso Config Tools:
- verifica unde au fost generate pin_mux, clock_config si peripherals;
- aliniaza board_files.cmake, cfg_tools_generated.cmake, include path-urile si
  cfg_tools/project_info.json;
- elimina initializarile duplicate;
- ruleaza cmake --preset debug si cmake --build --preset debug.

Documenteaza explicit compatibilitatea 3.3 V / 5 V.
Nu face refactorizari fara legatura cu aplicatia.
```

## Deliverable

Aplicatia trebuie sa:

- configureze pinii shield-ului in `main.mex`;
- foloseasca fisierele regenerate din `cfg_tools/board`;
- initializeze LCD-ul in mod 4 biti;
- afiseze textul de pornire;
- detecteze `RIGHT`, `UP`, `DOWN`, `LEFT`, `SELECT` si `NONE`;
- foloseasca praguri configurabile si filtrare;
- nu blocheze permanent la citirea ADC;
- pastreze driverele reutilizabile separate de adaptorul placii;
- compileze cu presetul `debug`;
- documenteze riscurile electrice ale shield-urilor de 5 V.

---

[← L5: LPADC - Masurarea tensiunilor analogice](../l5-lpadc)
