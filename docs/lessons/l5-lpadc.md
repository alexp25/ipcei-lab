---
title: "L5 — LPADC — Conversie Analogică 12 biți"
description: "Command/result queue, voltmetru, prag LED"
nav_order: 6
parent: Lecții FRDM-MCXA153
layout: lesson
---

# 📊 L5 — LPADC — Conversie Analogică 12 biți

**Command/result queue, voltmetru, prag LED**

---

| | |
|---|---|
| **Ziua / Sesiunea** | Ziua 3, 24 iunie — dimineață |
| **Periferic** | `LPADC0 · Command Queue · VREFH 3.3V` |
| **Durată** | 2h (09:00–11:00) |
| **Responsabil** | Cadru didactic UPB |
| **Hardware** | FRDM-MCXA153 · potențiometru 10kΩ pe A0 (PIO0_29) sau senzor analogic |

## 📌 Context și Motivație

LPADC0 pe MCX A153 folosește un API bazat pe command queue și result queue — diferit fundamental față de ADC AVR cu ADSC/ADIF. Rezoluție 12 biți (4096 nivele), VREFH = 3.3V. Greșeala clasică GenAI: generează cod pentru `ADC16` (familia Kinetis) care **NU există** pe MCX. Relevanță NXP: senzorii ADAS automotive folosesc ADC similar.

> **Board:** FRDM-MCXA153 · MCX A153 (Cortex-M33 @ 96 MHz) · SDK MCUXpresso 24.12 · VS Code + CMake

## 🎯 Obiective

1. LPADC0 configurat în single-ended mode, canal 0 (PIO0_29, Arduino A0)
2. Funcție `uint16_t adc_read_mv(void)` care returnează tensiunea în mV (0–3300)
3. Înțelegerea command/result queue API — complet diferit față de ADSC/ADIF AVR
4. Voltmetru simplu: citire ADC → mV → UART + LED prag la 2.0V

## 🕐 Planul Sesiunii

| Interval | Activitate | Detaliu | Cine |
|---|---|---|---|
| `09:00–09:30` | **LPADC arhitectură** | Command queue, result queue, software/hardware trigger. 12 biți = 4096 nivele. VREFH = 3.3V. Comparație ADMUX/ADCSRA AVR. | cadru UPB |
| `09:30–10:15` | **Demo: citire canal A0** | `LPADC_GetDefaultConfig()`, `LPADC_Init()`, `LPADC_SetConvCommandConfig()` cmdId=1, `LPADC_DoSoftwareTrigger()`, `LPADC_GetConvResult()`. | cadru UPB |
| `10:15–11:00` | **Lab + GenAI session** | Studenții: voltmetru (citire ADC → mV → UART @ 10Hz + LED roșu dacă >2V). Prompt cu VREF explicit. AI confundă frecvent LPADC cu ADC16. | student + mentor |

## 🤖 Prompte pentru Asistentul AI UPB

> **Regulă:** Copiați prompt-ul complet — contextul hardware este obligatoriu.
> AI-ul va genera cod greșit (pentru alte familii NXP sau Arduino) fără aceste informații.

### Prompt: LPADC citire mV

```text
Context hardware: FRDM-MCXA153, MCX A153 Cortex-M33 @ 96 MHz, SDK MCUXpresso 24.12.
LPADC0, canal 0 (PIO0_29, Arduino A0, single-ended). VREFH=3.3V. Rezoluție 12-bit.
Sarcina: uint16_t adc_read_mv(void) — returnează tensiunea în mV (0–3300).
Include:
  LPADC_GetDefaultConfig(), LPADC_Init()
  LPADC_SetConvCommandConfig() cu cmdId=1
  LPADC_SetConvTriggerConfig() cu software trigger
  LPADC_DoSoftwareTrigger(), LPADC_GetConvResult()
Formula conversie: mv = (raw * 3300) / 4095
IMPORTANT: folosește driverele LPADC, NU ADC16 (altă familie NXP, incompatibil).
```

### Prompt: LPADC debug

```text
FRDM-MCXA153, LPADC0, SDK MCUXpresso 24.12.
Problema: LPADC_GetConvResult returnează mereu false (conversie incompletă).
Config: cmdId=1, software trigger activat. Potențiometru pe PIO0_29 (A0).
Diagnoza pas cu pas:
1. CLOCK_EnableClock(kCLOCK_Lpadc0) apelat?
2. PIO0_29 configurat ca analog (kPORT_PinDisabledOrAnalog), NU GPIO?
3. LPADC_DoSoftwareTrigger apelat ÎNAINTE de LPADC_GetConvResult?
Explică ordinea corectă a operațiilor.
```

## ⚠️ Capcane Critice

> Ce GenAI **nu știe** fără context explicit — verificați înainte de upload pe placă.

- `PIO0_29` (A0) trebuie configurat ca intrare analogică (`kPORT_PinDisabledOrAnalog`), **NU** ca GPIO
- `LPADC_GetConvResult` returnează `false` dacă conversia nu e completă — așteptați `true` înainte de a citi
- **Command ID trebuie să fie ≥ 1** (0 este rezervat în LPADC SDK) — eroare frecventă de AI

## ✅ Deliverable

> Voltmetru funcțional: citire ADC + mV pe UART + LED prag + formula conversie documentată și verificată experimental

---

[← L4: Timer & PWM — SCTimer și CTIMER](../l4-timer-pwm) · [L6: LPSPI — Comunicație SPI & Device Extern →](../l6-lpspi)
