---
title: "L8 — Integrare — Sistem Complet cu FSM"
description: "Toate perifericele, super-loop, Show & Tell"
nav_order: 9
parent: Lecții FRDM-MCXA153
layout: lesson
---

# 🔧 L8 — Integrare — Sistem Complet cu FSM

**Toate perifericele, super-loop, Show & Tell**

---

| | |
|---|---|
| **Ziua / Sesiunea** | Ziua 4, 25 iunie — după-amiază + Ziua 5 |
| **Periferic** | `Toate perifericele · FSM · Super-loop` |
| **Durată** | 3h (13:00–16:00) |
| **Responsabil** | Echipe + Mentori UPB & NXP |
| **Hardware** | FRDM-MCXA153 cu toate conexiunile din lecțiile L1–L7 |

## 📌 Context și Motivație

Sesiunea de integrare testează dacă perifericele învățate separat funcționează împreună — cel mai realist exercițiu din perspectivă industrială. Problemele de integrare sunt specifice hardware-ului și nu pot fi detectate de GenAI fără context. AI-ul va genera FSM corect conceptual dar va omite protecția variabilelor partajate.

> **Board:** FRDM-MCXA153 · MCX A153 (Cortex-M33 @ 96 MHz) · SDK MCUXpresso 24.12 · VS Code + CMake

## 🎯 Obiective

1. Sistem integrat funcțional: GPIO + UART + IRQ + SysTick/FlexPWM + LPADC + I3C(I2C) simultan
2. Mașină de stări (FSM) care coordonează toate perifericele fără RTOS
3. Identificarea și rezolvarea conflictelor de integrare (atomic flag reads, WFI în super-loop)
4. Show & Tell intern: 5 min demo live + reflecție GenAI

## 🕐 Planul Sesiunii

| Interval | Activitate | Detaliu | Cine |
|---|---|---|---|
| `13:00–13:20` | **Arhitectura FSM** | State machine pe embedded: de ce e mai robustă decât if-else cascadat. Super-loop cu flag-uri volatile vs RTOS task. Schelet FSM cu GenAI. | cadru UPB |
| `13:20–15:00` | **Lab: sistem integrat** | Temperatură P3T1755 la 2Hz via I3C0 + UART log + FlexPWM LED în funcție de temperatură + alertă ISR buton SW3 + LPADC0 potențiometru pentru prag variabil. | echipă + mentor |
| `15:00–16:00` | **Show & Tell intern** | 5 min/echipă: demo live + ce a greșit AI + cum s-a corectat. Agentul AI evaluează calitatea reflecției. | panel intern UPB+NXP |

## ⚙️ Arhitectura Demo Integrat

### Perifericele active simultan

| Periferic | Rol | Canal / Pin |
|---|---|---|
| **I3C0** (mod I2C-compat) | Citire temperatură P3T1755 | On-board |
| **LPADC0** | Prag variabil potențiometru | Canal 8 — Arduino A0 (`ADC0_A8`) |
| **FlexPWM0 Mod.0** | LED Roșu intensitate variabilă | PWM0\_X0 |
| **FlexPWM0 Mod.1** | LED Verde intensitate variabilă | PWM0\_X1 |
| **GPIO** | LED Albastru (indicator COOLING) | `BOARD_LED_BLUE` |
| **GPIO IRQ** | Buton SW3 — start/stop monitoring | `BOARD_SW3_IRQ` — falling edge |
| **SysTick** | Tick 1 ms → eșantionare la 500 ms | `SystemCoreClock / 1000` |
| **LPUART0** | Log stare + temperatură + prag | 115200 baud, console |

### FSM — Mașina de Stări

```
           SW3 apăsat
    ┌──────────────────────────────┐
    ↓                              │
 STATE_IDLE ──[SW3]──► STATE_MONITORING
                              │
                      temp ≥ prag
                              │
                              ▼
                        STATE_ALERT
                              │
                    temp < prag − 2°C
                              │
                              ▼
                       STATE_COOLING
                          │       │
              temp < prag−2°C   temp ≥ prag
                  (stabil)           │
                    │                ▼
                    └──────► STATE_MONITORING / STATE_ALERT
```

```c
typedef enum {
    STATE_IDLE = 0,
    STATE_MONITORING,
    STATE_ALERT,
    STATE_COOLING
} app_state_t;
```

**Tranziții FSM:**

| Stare curentă | Condiție | Stare următoare |
|---|---|---|
| `IDLE` | SW3 apăsat | `MONITORING` |
| `MONITORING` | SW3 apăsat | `IDLE` |
| `MONITORING` | `temp >= prag` | `ALERT` |
| `ALERT` | `temp < prag − 2°C` | `COOLING` |
| `COOLING` | `temp < prag − 2°C` | `MONITORING` |
| `COOLING` | `temp >= prag` | `ALERT` |

### Comportament LED per stare

| Stare | LED Roșu (PWM) | LED Verde (PWM) | LED Albastru (GPIO) |
|---|---|---|---|
| `IDLE` | 0% | 0% | OFF |
| `MONITORING` | `temperature_to_duty(temp, prag)` | `100% − roșu` | OFF |
| `ALERT` | 100% | 0% | OFF |
| `COOLING` | `temperature_to_duty(temp, prag)` | `100% − roșu` | **ON** |

### Prag variabil ADC

Potențiometrul pe `Arduino A0 (ADC0_A8)` setează pragul între **20°C și 40°C**:

```c
#define THRESHOLD_MIN_CELSIUS      20.0F
#define THRESHOLD_SPAN_CELSIUS     20.0F
#define COOLING_HYSTERESIS_CELSIUS 2.0F

float adc_raw_to_threshold(uint16_t rawValue) {
    return THRESHOLD_MIN_CELSIUS + ((float)rawValue * THRESHOLD_SPAN_CELSIUS) / 65535.0F;
}
```

### Super-loop cu WFI și flag-uri atomice

```c
// ISR scrie flag-ul
void BOARD_SW3_IRQ_HANDLER(void) {
    if (GPIO_PinGetInterruptFlag(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN)) {
        GPIO_PinClearInterruptFlag(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN);
        g_buttonPressed = true;  // volatile bool
    }
}

// Main citește atomic (take_flag)
static bool take_flag(volatile bool *flag) {
    bool value;
    __disable_irq();
    value = *flag;
    *flag = false;
    __enable_irq();
    return value;
}

// Super-loop
while (1) {
    bool sampleTick   = take_flag(&g_sampleTick);
    bool buttonPressed = take_flag(&g_buttonPressed);

    if (sampleTick) {
        /* citire ADC, I3C, FSM, LED, PRINTF */
    } else if (buttonPressed) {
        process_fsm(&state, true, ...);
    }
    __WFI();  // CPU doarme până la următoarea IRQ (SysTick/SW3)
}
```

## 🤖 Prompte pentru Asistentul AI UPB

> **Regulă:** Copiați prompt-ul complet — contextul hardware este obligatoriu.
> AI-ul va genera cod greșit (pentru alte familii NXP sau Arduino) fără aceste informații.

### Prompt: Schelet FSM integrat

```text
FRDM-MCXA153, MCX A153 Cortex-M33 @ 96 MHz, SDK MCUXpresso 24.12, toate perifericele active simultan:
  I3C0 mod I2C-compat (P3T1755 onboard, eșantionare 500 ms via SysTick),
  LPUART0 (log stare + temperatură),
  FlexPWM0 Module0/Module1 (LED roșu/verde la 1 kHz),
  LPADC0 canal 8 Arduino A0 (prag variabil 20–40°C),
  GPIO IRQ SW3 falling edge (start/stop monitoring).

typedef enum { STATE_IDLE, STATE_MONITORING, STATE_ALERT, STATE_COOLING } app_state_t;

Sarcina: schelet FSM în C — DOAR structura, nu implementarea completă.
Arhitectura: super-loop cu flag-uri volatile setate din ISR-uri, fără RTOS.
Generează:
  - structura de date globale (volatile bool g_sampleTick, g_buttonPressed, volatile uint32_t g_msTicks)
  - funcția static bool take_flag(volatile bool *flag) cu __disable_irq()/__enable_irq()
  - funcția void process_fsm(app_state_t *state, bool buttonPressed, bool tempValid, float temp, float prag)
  - main() cu super-loop și __WFI()
  - comentarii despre ce trebuie protejat cu __disable_irq()/__enable_irq().
```

### Prompt: Conflict UART blocking + ISR

```text
FRDM-MCXA153, sistem integrat, super-loop cu __WFI().
Problema: LPUART_WriteBlocking() / PRINTF() blochează CPU ~1ms la 115200 baud.
Impact: dacă SysTick sau SW3 IRQ se declanșează în timpul transmisiei UART, flag-ul volatile bool
        este setat corect, dar main() nu îl citește imediat — latența crește.
Soluții evaluate:
1. Buffer circular UART + transmisie în background (ISR LPUART TX empty)
2. LPUART DMA mode — CPU liber în timp ce DMA trimite
3. PRINTF mai rar (nu la fiecare ciclu de 500 ms, ci la schimbări de stare)
Care e soluția optimă pentru un sistem fără RTOS pe MCX A153?
Explică tradeoff-urile față de abordarea actuală (take_flag + __WFI).
```

## ⚠️ Capcane Critice

> Ce GenAI **nu știe** fără context explicit — verificați înainte de upload pe placă.

- **FlexPWM, nu SCTimer:** MCX A153 folosește `FLEXPWM0` pentru LED RGB. GenAI va genera cod SCTimer (pentru LPC55Sxx) — greșit pe această familie.
- **I3C0 în mod I2C-compat, nu LPI2C0:** Senzorul P3T1755 este conectat la I3C0, nu la un LPI2C dedicat. Inițializarea este diferită.
- **Sampling 500 ms (2 Hz), nu 1 Hz:** `SAMPLE_PERIOD_MS = 500U` — documentele anterioare menționau 1 Hz; codul real eșantionează la 500 ms.
- **GenAI va omite `take_flag()` atomic:** Variabilele `volatile bool` setate din ISR trebuie citite cu `__disable_irq()`/`__enable_irq()` pentru a evita race conditions. GenAI generează `if (g_flag)` direct — nesigur.
- **`__WFI()` blochează dacă ISR-ul a trecut deja:** Dacă SW3 se apasă între `take_flag()` și `__WFI()`, flag-ul este consumat dar CPU nu intră în WFI — comportament corect deoarece SysTick va trezi CPU la 1 ms. Totuși, fără SysTick activ, `__WFI()` poate bloca indefinit.
- **ADC și I3C pe ceasuri diferite:** `ADC0` folosește `kFRO12M_to_ADC0`, I3C folosește ceasul sistemului. Verificați `CLOCK_SetClockDiv` pentru ADC înainte de `LPADC_Init`.
- **LPADC calibrare obligatorie:** `LPADC_DoOffsetCalibration()` + `LPADC_DoAutoCalibration()` trebuie apelate după init; GenAI le omite frecvent.

## ✅ Deliverable

> Sistem integrat funcțional + FSM documentat cu diagrama stărilor + Show & Tell 5 min + reflecție GenAI (ce a greșit, cum s-a corectat)

**Comportament așteptat la rulare:**
```
FRDM-MCXA153 integration demo: GPIO + UART + IRQ + SysTick/PWM + ADC + LPI2C
Press SW3 to start/stop monitoring. A0 sets threshold from 20.0 C to 40.0 C.
state=IDLE     temp=N/A  threshold=30.00 C adc=0
[SW3 apăsat]
state=MONITORING temp=25.50 C threshold=30.00 C adc=32767
state=ALERT      temp=31.20 C threshold=30.00 C adc=32767
state=COOLING    temp=27.80 C threshold=30.00 C adc=32767
```

---

[← L7: LPI2C — Senzorul P3T1755 On-Board](../l7-lpi2c-p3t1755) · [L9: Testing, Documentare & Prezentare Finală →](../l9-testing-docs)
