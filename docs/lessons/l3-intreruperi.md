---
title: "L3 — Întreruperi Externe & NVIC"
description: "GPIO IRQ pe Cortex-M33, volatile, debounce"
nav_order: 4
parent: Lecții FRDM-MCXA153
layout: lesson
---

# ⚡ L3 — Întreruperi Externe & NVIC

**GPIO IRQ pe Cortex-M33, volatile, debounce**

---

| | |
|---|---|
| **Ziua / Sesiunea** | Ziua 2, 23 iunie — dimineață |
| **Periferic** | `GPIO IRQ · NVIC · SysTick` |
| **Durată** | 2h (09:00–11:00) |
| **Responsabil** | Cadru didactic UPB |
| **Hardware** | FRDM-MCXA153 · SW3 (PIO0_6) · LED RGB |

## 📌 Context și Motivație

Pe Cortex-M33, întreruperile sunt gestionate de NVIC (Nested Vectored Interrupt Controller). GPIO IRQ pe MCX A: fiecare PORT are un singur vector de întrerupere — toți pinii de pe PORT0 împart `GPIO0_IRQHandler`. Diferit față de `INT0_vect` individual de pe AVR. Trebuie verificat flag-ul ISF (Interrupt Status Flag) individual pentru a identifica pinul.

> **Board:** FRDM-MCXA153 · MCX A153 (Cortex-M33 @ 96 MHz) · SDK MCUXpresso 24.12 · VS Code + CMake

## 🎯 Obiective

1. ISR funcțional pentru butonul SW3 (PIO0_6, falling edge) — toggle LED verde
2. Înțelegerea diferenței AVR vs Cortex-M33: port IRQ partajat vs INT0_vect individual
3. Implementare debouncing în ISR fără delay — timestamp-based (SysTick ms)
4. Contor apăsări cu protecție atomică (DisableIRQ/EnableIRQ)

## 🕐 Planul Sesiunii

| Interval | Activitate | Detaliu | Cine |
|---|---|---|---|
| `09:00–09:30` | **NVIC pe Cortex-M33** | Vector table, priority levels. ISF flag per pin. `EnableIRQ()` vs `NVIC_EnableIRQ()`. Comparație `sei()`/`ISR()` AVR. | cadru UPB |
| `09:30–10:15` | **Demo: SW3 ISR** | `PORT_SetPinInterruptConfig(PORT0,6,kPORT_InterruptFallingEdge)`, `EnableIRQ(GPIO0_IRQn)`, `GPIO0_IRQHandler` complet cu `GPIO_PortClearInterruptFlags()`. | cadru UPB |
| `10:15–11:00` | **Lab + GenAI session** | Studenții: ISR + debounce timestamp + contor UART la fiecare 5 apăsări. AI omite frecvent `GPIO_PortClearInterruptFlags()`. | student + mentor |

## 🤖 Prompte pentru Asistentul AI UPB

> **Regulă:** Copiați prompt-ul complet — contextul hardware este obligatoriu.
> AI-ul va genera cod greșit (pentru alte familii NXP sau Arduino) fără aceste informații.

### Prompt: ISR buton SW3

```text
Context hardware: FRDM-MCXA153, MCX A153 Cortex-M33, SDK MCUXpresso 24.12.
Buton SW3: PIO0_6, pull-up intern, falling edge trigger.
Sarcina: ISR care togglează LED verde (PIO3_12, anod comun) + incrementează contor volatile.
Include:
  PORT_SetPinInterruptConfig(PORT0, 6u, kPORT_InterruptFallingEdge)
  EnableIRQ(GPIO0_IRQn)
  GPIO0_IRQHandler complet cu GPIO_PortClearInterruptFlags()
volatile uint32_t g_button_count;
Explică de ce GPIO_PortClearInterruptFlags() este OBLIGATORIU la finalul ISR.
```

### Prompt: ISR re-intrare infinit

```text
FRDM-MCXA153, GPIO0_IRQHandler pentru SW3.
Problema: ISR-ul se declanșează la nesfârșit după prima apăsare de buton.
Diagnosticare: am GPIO_PortToggleOutputPins, am volatile flag, am EnableIRQ,
dar NU am GPIO_PortClearInterruptFlags în ISR.
Explică mecanismul exact pe Cortex-M33: de ce flagul ISF nedegajat
cauzează re-intrarea continuă? Ce se întâmplă în NVIC?
```

## ⚠️ Capcane Critice

> Ce GenAI **nu știe** fără context explicit — verificați înainte de upload pe placă.

- `GPIO_PortClearInterruptFlags()` **OBLIGATORIU** la finalul ISR — fără el ISR se re-apelează la infinit (ISF rămâne setat)
- Toți pinii de pe PORT0 împart `GPIO0_IRQHandler` — verificați ISF flag individual cu `GPIO_PortGetInterruptFlags()`
- `volatile` NU garantează atomicitate pe operații read-modify-write — folosiți `DisableIRQ`/`EnableIRQ`

## ✅ Deliverable

> ISR SW3 cu debounce + contor UART + analiza documentată a mecanismului ISF + comparație AVR vs NVIC

---

[← L2: LPUART — Comunicație Serială](../l2-lpuart) · [L4: Timer & PWM — SCTimer și CTIMER →](../l4-timer-pwm)
