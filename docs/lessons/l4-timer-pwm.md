---
title: "L4 — PWM cu FlexPWM"
description: "Fade LED RGB cu FlexPWM0, pin mux și GenAI"
nav_order: 5
parent: Lecții FRDM-MCXA153
layout: lesson
---

# ⏱️ L4 — PWM cu FlexPWM

**Fade LED RGB cu FlexPWM0, pin mux și GenAI**

---

| | |
|---|---|
| **Ziua / Sesiunea** | Ziua 2, 23 iunie — după-amiază |
| **Periferic** | `FlexPWM0 · PORT pin mux · LPUART0` |
| **Durată** | 2h (13:00–15:00) |
| **Responsabil** | Cadru didactic UPB |
| **Hardware** | FRDM-MCXA153 · LED RGB · Serial Monitor · osciloscop opțional |

## 📌 Context și Motivație

În acest laborator folosim `FlexPWM0`, perifericul specializat pentru semnale PWM. Spre deosebire de un GPIO toggled în software, FlexPWM generează forma de undă în hardware: CPU-ul schimbă doar duty cycle-ul, iar perifericul continuă să producă semnalul la frecvență constantă.

Pe FRDM-MCXA153, LED-urile RGB pot fi legate la FlexPWM prin pin mux:

- roșu: `P3_12 / PWM0_X0`
- verde: `P3_13 / PWM0_X1`
- albastru: `P3_0 / PWM0_A0`

Vom porni de la un starter în care roșu și verde fac fade cu PWM. Extensia GenAI a laboratorului cere adăugarea LED-ului albastru pe PWM și implementarea unui color wheel.

> **Board:** FRDM-MCXA153 · MCX A153 (Cortex-M33 @ 96 MHz) · SDK MCUXpresso 24.12 · VS Code + CMake

## 🎯 Obiective

1. Înțelegerea diferenței dintre GPIO și PWM hardware.
2. Configurarea pinilor `P3_12` și `P3_13` ca ieșiri `FlexPWM0`.
3. Inițializarea `FlexPWM0` cu `PWM_GetDefaultConfig()`, `PWM_Init()` și `PWM_SetupPwm()`.
4. Actualizarea duty cycle-ului fără reinițializarea perifericului.
5. Folosirea Serial Monitor pentru a urmări duty cycle-ul.
6. Folosirea GenAI pentru a extinde starterul: LED albastru PWM + color wheel RGB.

## 🕐 Planul Sesiunii

| Interval | Activitate | Detaliu | Cine |
|---|---|---|---|
| `13:00–13:25` | **PWM hardware** | De ce folosim FlexPWM în loc de GPIO toggling. Frecvență, duty cycle, pin mux. | cadru UPB |
| `13:25–14:05` | **Starter red/green** | Citire cod: `PWM_InitLedChannel()`, `PWM_SetupPwm()`, `PWM_UpdatePwmDutycycle()`, `PWM_SetPwmLdok()`. | cadru UPB + studenți |
| `14:05–14:30` | **Debugging PWM** | Pin mux, `kPWM_PwmX`, `DISMAP`, Serial Monitor. | cadru UPB |
| `14:30–15:00` | **GenAI extension** | Studenții cer AI-ului să facă blue PWM și color wheel RGB. Verificare cod + `.mex` în Config Tools. | student + mentor |

## 🤖 Prompte Pentru Asistentul AI UPB

> **Regulă:** Copiați prompt-ul complet. Contextul hardware este obligatoriu.
> În acest laborator cereți explicit `FlexPWM0`; nu cereți SCTimer, CTIMER sau cod Arduino.

### Prompt: Explică Starterul FlexPWM

```text
Am un proiect MCUXpresso SDK pentru FRDM-MCXA153.
Starterul folosește FlexPWM0 pentru două LED-uri:
- roșu: P3_12 / PWM0_X0 / kPWM_Module_0 / kPWM_PwmX
- verde: P3_13 / PWM0_X1 / kPWM_Module_1 / kPWM_PwmX

Explică pe scurt, pentru studenți, cum funcționează:
1. pin mux-ul către FlexPWM;
2. PWM_GetDefaultConfig(), PWM_Init(), PWM_SetupPwm();
3. de ce folosim kPWM_PwmX și kPWM_LowTrue;
4. de ce trebuie PWM_SetPwmLdok() după PWM_UpdatePwmDutycycle();
5. de ce există PWM_SetupFaultDisableMap(..., 0U).

Nu rescrie tot codul. Explică pornind de la codul existent.
```

### Alte Variante De Timer

MCX A153 are și alte periferice utile:

- `CTIMER` este potrivit pentru măsurare de timp, întreruperi periodice și semnale simple.
- `SCTimer` poate genera PWM cu evenimente și match registers.

În acest laborator nu folosim aceste variante. Focusul este `FlexPWM0`, deoarece este perifericul dedicat pentru PWM multi-canal și se potrivește cel mai bine cu exercițiul RGB.

## ⚠️ Capcane Critice

> Ce GenAI **nu știe** fără context explicit — verificați înainte de upload pe placă.

- Pinul trebuie să fie rutat către FlexPWM în pin mux. Dacă rămâne GPIO, registrele PWM se pot actualiza fără efect vizibil pe LED.
- `PWM0_X0` și `PWM0_X1` folosesc canalul `kPWM_PwmX`, nu `kPWM_PwmA` sau `kPWM_PwmB`.
- `PWM_UpdatePwmDutycycle()` actualizează registre bufferizate; apelați `PWM_SetPwmLdok()` ca valoarea să devină activă.
- `DISMAP` poate ține ieșirea PWM dezactivată dacă fault mapping-ul nu este curățat.
- Duty cycle 0% sau 100% pot avea comportamente de margine; starterul folosește `[1, 99]` pentru fade roșu/verde.

## Starter Funcțional: FlexPWM0 Pe LED Roșu Și Verde

În varianta de laborator `src/lab_pwm/pwm_control`, PWM-ul este generat cu perifericul `FlexPWM0`. Starterul controlează două LED-uri:

- LED roșu: `P3_12 / PWM0_X0`
- LED verde: `P3_13 / PWM0_X1`

LED-ul albastru pornește intenționat ca GPIO/off. Studenții îl vor muta pe PWM în exercițiul GenAI.

### 1. Pin mux

Pinii trebuie trecuți din GPIO în funcția alternativă FlexPWM:

- `P3_12` → `PWM0_X0` → `kPORT_MuxAlt5`
- `P3_13` → `PWM0_X1` → `kPORT_MuxAlt5`

Fișierul activ pentru acest proiect este `frdmmcxa153/pwm_control/pin_mux.c`. Dacă pinii rămân configurați ca GPIO (`kPORT_MuxAlt0`), codul poate afișa corect `update duty cycle`, dar LED-urile nu se schimbă, deoarece semnalul FlexPWM nu ajunge la pini.

`FLEXPWM0` trebuie și scos din reset în inițializarea pinilor:

```c
RESET_ReleasePeripheralReset(kFLEXPWM0_RST_SHIFT_RSTn);
```

După ce roșu și verde sunt conduse de PWM, ele nu se mai inițializează ca GPIO. În starter, LED-ul albastru rămâne GPIO/off.

### 2. Inițializare FlexPWM

Se inițializează două submodule:

- `kPWM_Module_0` pentru `PWM0_X0` / LED roșu
- `kPWM_Module_1` pentru `PWM0_X1` / LED verde

Configurarea folosește canalul `kPWM_PwmX`, modul `kPWM_EdgeAligned`, frecvența `1000 Hz` și polaritatea `kPWM_LowTrue`.

```c
PWM_GetDefaultConfig(&pwmConfig);
PWM_Init(FLEXPWM0, kPWM_Module_0, &pwmConfig);
PWM_Init(FLEXPWM0, kPWM_Module_1, &pwmConfig);

pwmSignal.pwmChannel       = kPWM_PwmX;
pwmSignal.level            = kPWM_LowTrue;
pwmSignal.dutyCyclePercent = 1U;
pwmSignal.deadtimeValue    = 0U;
pwmSignal.faultState       = kPWM_PwmFaultState0;
pwmSignal.pwmchannelenable = true;
```

Pentru `PWMX`, documentația driverului NXP recomandă PWM edge-aligned și `kPWM_LowTrue`, deoarece semnalul PWMX edge-aligned este tratat ca negative-true.

### 3. Pornire și actualizare duty cycle

`PWM_SetupPwm()` configurează perioada și duty cycle-ul inițial. După setup, ieșirile PWMX se activează, se încarcă registrele bufferizate cu `LDOK`, apoi pornesc timerele:

```c
PWM_SetupPwm(FLEXPWM0, kPWM_Module_0, &pwmSignal, 1U, kPWM_EdgeAligned, 1000U, SystemCoreClock);
PWM_SetupPwm(FLEXPWM0, kPWM_Module_1, &pwmSignal, 1U, kPWM_EdgeAligned, 1000U, SystemCoreClock);

PWM_OutputEnable(FLEXPWM0, kPWM_PwmX, kPWM_Module_0);
PWM_OutputEnable(FLEXPWM0, kPWM_PwmX, kPWM_Module_1);

PWM_SetPwmLdok(FLEXPWM0, kPWM_Control_Module_0 | kPWM_Control_Module_1, true);
PWM_StartTimer(FLEXPWM0, kPWM_Control_Module_0 | kPWM_Control_Module_1);
```

În bucla principală, duty cycle-ul este actualizat fără reinițializarea timerului:

```c
PWM_UpdatePwmDutycycle(FLEXPWM0, kPWM_Module_0, kPWM_PwmX, kPWM_EdgeAligned, red_duty);
PWM_UpdatePwmDutycycle(FLEXPWM0, kPWM_Module_1, kPWM_PwmX, kPWM_EdgeAligned, green_duty);
PWM_SetPwmLdok(FLEXPWM0, kPWM_Control_Module_0 | kPWM_Control_Module_1, true);
```

`PWM_UpdatePwmDutycycle()` scrie valori noi în registre bufferizate. Fără `PWM_SetPwmLdok()`, modificarea poate să nu ajungă în registrele active.

### 4. Problema reală: LED-urile stinse după self-test

În debugging s-a observat următorul comportament:

- UART afișa `update duty cycle`, deci bucla principală funcționa.
- LED-urile se aprindeau într-un self-test GPIO.
- După trecerea pinilor pe FlexPWM, LED-urile rămâneau stinse.

Asta arată că pinii și LED-urile sunt corecte, iar problema este în calea FlexPWM.

Fixul critic a fost anularea fault disable mapping pentru PWMX:

```c
PWM_SetupFaultDisableMap(FLEXPWM0, kPWM_Module_0, kPWM_PwmX, kPWM_faultchannel_0, 0U);
PWM_SetupFaultDisableMap(FLEXPWM0, kPWM_Module_1, kPWM_PwmX, kPWM_faultchannel_0, 0U);
```

Motivul: registrele `DISMAP` ale FlexPWM pot mapa intrările de fault la ieșiri. Dacă un fault mapat este activ, ieșirea PWM poate fi dezactivată chiar dacă timerul rulează și duty cycle-ul se actualizează. În acest laborator nu folosim intrări de fault, deci maparea se curăță explicit.

### 5. Test de diagnostic recomandat

Un self-test GPIO înainte de PWM ajută la separarea problemelor de pin mux de problemele FlexPWM:

```c
PORT_SetPinMux(BOARD_INITPINS_LED_RED_PORT, BOARD_INITPINS_LED_RED_PIN, kPORT_MuxAlt0);
PORT_SetPinMux(BOARD_INITPINS_LED_GREEN_PORT, BOARD_INITPINS_LED_GREEN_PIN, kPORT_MuxAlt0);

LED_RED_INIT(LOGIC_LED_ON);
LED_GREEN_INIT(LOGIC_LED_ON);
SDK_DelayAtLeastUs(500000U, SystemCoreClock);

LED_RED_OFF();
LED_GREEN_OFF();

PORT_SetPinMux(BOARD_INITPINS_LED_RED_PORT, BOARD_INITPINS_LED_RED_PIN, kPORT_MuxAlt5);
PORT_SetPinMux(BOARD_INITPINS_LED_GREEN_PORT, BOARD_INITPINS_LED_GREEN_PIN, kPORT_MuxAlt5);
```

Interpretare:

- Dacă LED-urile nu se aprind nici în self-test, problema este la pin mapping, board config sau hardware.
- Dacă LED-urile se aprind în self-test, dar se sting când trec pe PWM, problema este în configurarea FlexPWM.

### Capcane Specifice FlexPWM0

- `PWM0_X0` folosește `kPWM_Module_0` și `kPWM_PwmX`.
- `PWM0_X1` folosește `kPWM_Module_1` și `kPWM_PwmX`.
- `PWM0_X0/PWM0_X1` nu sunt `kPWM_PwmA` sau `kPWM_PwmB`.
- După fiecare `PWM_UpdatePwmDutycycle()` trebuie apelat `PWM_SetPwmLdok()`.
- Pentru efectul breathing, intervalul `[1, 99]` evită cazurile limită 0% și 100%.
- LED-urile RGB de pe FRDM-MCXA153 sunt active-low (`LOGIC_LED_ON = 0`), deci polaritatea PWM trebuie aleasă ținând cont de acest lucru.

## Exercițiu GenAI: adăugați LED-ul albastru și color wheel

### Starea de pornire

Studenții pornesc de la proiectul `src/lab_pwm/pwm_control`, în care:

- LED-ul roșu face fade cu `FlexPWM0`, `PWM0_X0`, `kPWM_Module_0`, `kPWM_PwmX`;
- LED-ul verde face fade cu `FlexPWM0`, `PWM0_X1`, `kPWM_Module_1`, `kPWM_PwmX`;
- LED-ul albastru este încă GPIO/off, pe `P3_0`;
- Serial Monitor afișează duty cycle pentru roșu și verde.

Scopul exercițiului este ca studenții să folosească un asistent GenAI pentru a extinde proiectul, nu pentru a rescrie tot laboratorul.

### Cerință

Actualizați proiectul astfel încât:

1. LED-ul albastru să fie controlat cu PWM, nu GPIO.
2. LED-ul RGB să facă fade între culori:
   - roșu → verde;
   - verde → albastru;
   - albastru → roșu.
3. Serial Monitor să afișeze duty cycle pentru toate cele trei canale:

```text
duty: red=... green=... blue=...
```

4. Fișierul `.mex` să fie actualizat, astfel încât în MCUXpresso Config Tools pinul albastru să apară ca FlexPWM, nu GPIO.

### Informații hardware obligatorii pentru prompt

LED-ul albastru folosit pe placă este:

- pin fizic: `P3_0`;
- funcție PWM: `PWM0_A0`;
- FlexPWM: `FLEXPWM0`, `kPWM_Module_0`, `kPWM_PwmA`;
- pin mux: `kPORT_MuxAlt5`.

Important: pe același submodul `kPWM_Module_0` vor exista două canale PWM:

- `kPWM_PwmX` pentru LED roșu (`PWM0_X0`);
- `kPWM_PwmA` pentru LED albastru (`PWM0_A0`).

### Prompt recomandat pentru studenți

```text
Am un proiect MCUXpresso SDK pentru FRDM-MCXA153.
Codul curent face fade între LED roșu și LED verde folosind FlexPWM0:
- roșu: P3_12 / PWM0_X0 / kPWM_Module_0 / kPWM_PwmX
- verde: P3_13 / PWM0_X1 / kPWM_Module_1 / kPWM_PwmX

Vreau să adaug LED-ul albastru pe PWM:
- albastru: P3_0 / PWM0_A0 / kPWM_Module_0 / kPWM_PwmA
- pin mux pentru P3_0 trebuie schimbat din GPIO în FlexPWM0 A,0, adică kPORT_MuxAlt5

Cerințe:
1. păstrează codul simplu pentru studenți;
2. configurează PWM și pentru albastru;
3. păstrează fixul PWM_SetupFaultDisableMap(..., 0U), inclusiv pentru canalul albastru;
4. implementează fade color wheel: red -> green -> blue -> red;
5. afișează în Serial Monitor duty cycle pentru red, green și blue;
6. spune-mi ce trebuie schimbat și în fișierul .mex ca pinul P3_0 să fie vizibil în Config Tools ca FlexPWM0 A,0.
```

### Checklist de verificare

În cod:

- `PWM_InitLedChannel()` poate primi și canalul PWM (`kPWM_PwmX` sau `kPWM_PwmA`), nu doar submodulul.
- Pentru albastru trebuie apelat `PWM_SetupPwm()` cu `kPWM_Module_0` și `kPWM_PwmA`.
- Pentru albastru trebuie apelat `PWM_SetupFaultDisableMap(FLEXPWM0, kPWM_Module_0, kPWM_PwmA, kPWM_faultchannel_0, 0U)`.
- `RGB_SetDuty()` trebuie să actualizeze trei canale:
  - roșu: module 0, PWMX;
  - verde: module 1, PWMX;
  - albastru: module 0, PWMA.
- După actualizări trebuie apelat `PWM_SetPwmLdok()`.

În pin mux:

- `P3_0` trebuie să fie `PWM0_A0`, nu `GPIO3, 0`.
- În `frdmmcxa153/pwm_control/pin_mux.c`, `P3_0` trebuie să folosească `kPORT_MuxAlt5`.
- Inițializarea GPIO pentru LED-ul albastru trebuie eliminată sau lăsată nefolosită după ce pinul este controlat de PWM.

În `.mex`:

- `frdmmcxa153/pwm_control.mex` trebuie să conțină pinul:

```xml
<pin peripheral="FlexPWM0" signal="A, 0" pin_num="46" pin_signal="P3_0/WUU0_IN22/TRIG_IN0/CT_INP16/PWM0_A0">
```

- Deschideți `.mex` în MCUXpresso Config Tools și verificați vizual că `P3_0` apare ca `FlexPWM0 A,0`.

## ✅ Deliverable

> Starter red/green FlexPWM funcțional + duty cycle în Serial Monitor + extensie GenAI pentru blue PWM și color wheel + `.mex` verificat vizual în Config Tools

---

[← L3: Întreruperi Externe & NVIC](../l3-intreruperi) · [L5: LPADC — Conversie Analogică 12 biți →](../l5-lpadc)
