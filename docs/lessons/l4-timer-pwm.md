---
title: "L4 — Timer & PWM — SCTimer și CTIMER"
description: "Breathing LED, timer periodic, osciloscop"
nav_order: 5
parent: Lecții FRDM-MCXA153
layout: default
---

# ⏱️ L4 — Timer & PWM — SCTimer și CTIMER

**Breathing LED, timer periodic, osciloscop**

---

| | |
|---|---|
| **Ziua / Sesiunea** | Ziua 2, 23 iunie — după-amiază |
| **Periferic** | `SCTimer/PWM · CTIMER0 · FRO_HF` |
| **Durată** | 2h (13:00–15:00) |
| **Responsabil** | Cadru didactic UPB |
| **Hardware** | FRDM-MCXA153 · LED RGB (PWM pe albastru PIO3_13) · osciloscop recomandat |

## 📌 Context și Motivație

FRDM-MCXA153 are SCTimer — un timer 32-bit de stare configurable cu events și match registers, ideal pentru PWM multi-canal fără CPU overhead. CTIMER este mai simplu (similar Timer1 AVR). Relevanță NXP: SCTimer este folosit în controlul motoarelor BLDC/PMSM — core business NXP automotive.

> **Board:** FRDM-MCXA153 · MCX A153 (Cortex-M33 @ 96 MHz) · SDK MCUXpresso 24.12 · VS Code + CMake

## 🎯 Obiective

1. PWM la 1 kHz generat pe LED albastru (PIO3_13, `SCT0_OUT2`) cu duty cycle variabil
2. Funcție `void pwm_set_duty(uint8_t percent)` fără reinițializarea timer-ului
3. CTIMER0 periodic de 1 secundă cu ISR și afișare contor pe UART
4. Breathing LED effect: duty cycle 0→100→0% la fiecare 10ms via CTIMER ISR

## 🕐 Planul Sesiunii

| Interval | Activitate | Detaliu | Cine |
|---|---|---|---|
| `13:00–13:30` | **SCTimer vs CTIMER** | Arhitectura SCTimer: events, states, match registers, output-uri. CTIMER: match register simplu, similar Timer1 AVR. Formula: `period = FRO_HF / frequency`. | cadru UPB |
| `13:30–14:15` | **Demo PWM SCTimer** | `SCTIMER_GetDefaultConfig()`, `SCTIMER_Init()`, `SCTIMER_SetupPwm()`. Actualizare duty cycle: `SCTIMER_UpdatePwmDutycycle()`. Clock: FRO_HF → SCTimer. | cadru UPB |
| `14:15–15:00` | **Lab + GenAI session** | Studenții: breathing LED cu CTIMER 10ms ISR. Formula SCTimer calculată manual. Verificare pe osciloscop. AI confundă SCTimer cu CTIMER. | student + mentor |

## 🤖 Prompte pentru Asistentul AI UPB

> **Regulă:** Copiați prompt-ul complet — contextul hardware este obligatoriu.
> AI-ul va genera cod greșit (pentru alte familii NXP sau Arduino) fără aceste informații.

### Prompt: PWM SCTimer

```text
Context hardware: FRDM-MCXA153, MCX A153 Cortex-M33 @ 96 MHz, SDK MCUXpresso 24.12.
Sarcina: PWM pe LED albastru (PIO3_13, SCT0_OUT2), frecvență=1kHz, duty cycle inițial=50%.
Funcție void pwm_set_duty(uint8_t percent) — actualizare fără reinit timer.
Include:
  SCTIMER_GetDefaultConfig(), SCTIMER_Init()
  SCTIMER_SetupPwm(), SCTIMER_UpdatePwmDutycycle()
Clock source: FRO_HF (96 MHz) → SCTimer.
Arată calculul: SCTimer period = FRO_HF / frequency, match value pentru 50%.
```

### Prompt: CTIMER periodic 1s

```text
FRDM-MCXA153, CTIMER0, SDK MCUXpresso 24.12.
Sarcina: timer periodic de 1 secundă cu ISR care incrementează contor
și afișează valoarea pe LPUART0.
Include:
  CTIMER_GetDefaultConfig(), CTIMER_Init()
  CTIMER_SetupMatch(), CTIMER_EnableInterrupts(), CTIMER_StartTimer()
Clock: FRO_HF_DIV (48 MHz) → CTIMER0.
Arată calculul match value pentru exact 1 secundă.
```

## ⚠️ Capcane Critice

> Ce GenAI **nu știe** fără context explicit — verificați înainte de upload pe placă.

- **SCTimer OUT index ≠ GPIO pin index** — verificați în Reference Manual tabelul pin mux: `SCT0_OUTn` → care GPIO
- `SCTIMER_UpdatePwmDutycycle()` primește **evenimentul PWM** (returnat de `SCTIMER_SetupPwm`), NU indexul match register
- Duty cycle 0% sau 100% pot necesita tratament special — verificați comportamentul SDK

## ✅ Deliverable

> Breathing LED funcțional + semnal PWM verificat pe osciloscop + formula calculată manual + CTIMER periodic pe UART

---

[← L3: Întreruperi Externe & NVIC](l3-intreruperi.md) · [L5: LPADC — Conversie Analogică 12 biți →](l5-lpadc.md)
