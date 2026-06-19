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

1. Sistem integrat funcțional: GPIO + UART + IRQ + Timer/PWM + ADC + LPI2C simultan
2. Mașină de stări (FSM) care coordonează toate perifericele fără RTOS
3. Identificarea și rezolvarea conflictelor de integrare (UART blocking vs IRQ latency)
4. Show & Tell intern: 5 min demo live + reflecție GenAI

## 🕐 Planul Sesiunii

| Interval | Activitate | Detaliu | Cine |
|---|---|---|---|
| `13:00–13:20` | **Arhitectura FSM** | State machine pe embedded: de ce e mai robustă decât if-else cascadat. Super-loop cu flag-uri volatile vs RTOS task. Schelet FSM cu GenAI. | cadru UPB |
| `13:20–15:00` | **Lab: sistem integrat** | Temperatura I2C 1Hz + UART log + PWM LED funcție de temperatură + alertă ISR buton + ADC potențiometru pentru prag variabil. | echipă + mentor |
| `15:00–16:00` | **Show & Tell intern** | 5 min/echipă: demo live + ce a greșit AI + cum s-a corectat. Agentul AI evaluează calitatea reflecției. | panel intern UPB+NXP |

## 🤖 Prompte pentru Asistentul AI UPB

> **Regulă:** Copiați prompt-ul complet — contextul hardware este obligatoriu.
> AI-ul va genera cod greșit (pentru alte familii NXP sau Arduino) fără aceste informații.

### Prompt: Schelet FSM integrat

```text
FRDM-MCXA153, toate perifericele active simultan:
  LPI2C0 (P3T1755 1Hz), LPUART0 (../log), SCTimer PWM (../lED), LPADC0 (prag), GPIO IRQ SW3.
Sarcina: schelet FSM în C — DOAR structura, nu implementarea completă.
typedef enum { STATE_IDLE, STATE_MONITORING, STATE_ALERT, STATE_COOLING } app_state_t;
Arhitectura: super-loop cu flag-uri volatile setate din ISR-uri, fără RTOS.
Generează:
  - header-ele necesare
  - structura de date globale
  - funcția void process_fsm(app_state_t *state)
  - comentarii despre ce trebuie protejat cu DisableIRQ/EnableIRQ.
```

### Prompt: Conflict UART blocking + ISR

```text
FRDM-MCXA153, sistem integrat.
Problema: LPUART_WriteBlocking() blochează CPU ~1ms la 115200 baud.
Impact: ISR-ul butonului SW3 nu poate răspunde în timp real în timpul transmisiei UART.
Soluții evaluate:
1. Buffer circular UART + transmisie în background (ISR LPUART TX empty)
2. LPUART DMA mode — CPU liber în timp ce DMA trimite
3. Printf mai rar (nu la fiecare ciclu)
Care e soluția optimă pentru un sistem fără RTOS pe MCX A153?
Explică tradeoff-urile.
```

## ⚠️ Capcane Critice

> Ce GenAI **nu știe** fără context explicit — verificați înainte de upload pe placă.

- `LPUART_WriteBlocking` blochează CPU complet — incompatibil cu ISR time-critical; folosiți buffer circular + TX IRQ sau LPUART DMA
- **GenAI va omite ATOMIC_BLOCK** pentru variabile partajate ISR/main — adăugați `DisableIRQ`/`EnableIRQ` manual
- ADC și I2C pot folosi același bus DMA — verificați conflictele de canal DMA dacă folosiți DMA pentru ambele

## ✅ Deliverable

> Sistem integrat funcțional + FSM documentat cu diagrama stărilor + Show & Tell 5 min + reflecție GenAI (ce a greșit, cum s-a corectat)

---

[← L7: LPI2C — Senzorul P3T1755 On-Board](../l7-lpi2c-p3t1755) · [L9: Testing, Documentare & Prezentare Finală →](../l9-testing-docs)
