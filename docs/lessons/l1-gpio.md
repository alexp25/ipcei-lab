---
title: "L1 — GPIO — Control Digital & Butoane"
description: "PORT mux + GPIO data pe Cortex-M33"
nav_order: 2
parent: Lecții FRDM-MCXA153
layout: default
---

# 💡 L1 — GPIO — Control Digital & Butoane

**PORT mux + GPIO data pe Cortex-M33**

---

| | |
|---|---|
| **Ziua / Sesiunea** | Ziua 1, 22 iunie — dimineață |
| **Periferic** | `PORT · GPIO · FGPIO` |
| **Durată** | 2h (09:00–11:00) |
| **Responsabil** | Cadru didactic UPB |
| **Hardware** | FRDM-MCXA153 · RGB LED D3 · buton SW3 · osciloscop opțional |

## 📌 Context și Motivație

Prima lecție de periferic introduce arhitectura GPIO pe Cortex-M33/MCX A — fundamental diferită față de DDR/PORT/PIN de pe AVR. Pe MCX A, configurarea pinului (`PORT_SetPinConfig`) este separată de controlul datelor (`GPIO_PinWrite`). Această separare reflectă realitatea SoC-urilor industriale: mux-ul și drive strength sunt independente de logica aplicației.

> **Board:** FRDM-MCXA153 · MCX A153 (Cortex-M33 @ 96 MHz) · SDK MCUXpresso 24.12 · VS Code + CMake

## 🎯 Obiective

1. Configurare pin ca output GPIO și control independent al celor 3 LED-uri RGB
2. Citire stare buton SW3 (PIO0_6, pull-up intern) prin polling cu debounce software
3. Înțelegerea arhitecturii MCX: PORT (mux + configurare) separat de GPIO (date)
4. Primul prompt contextualizat cu referința completă SDK NXP formulat și testat

## 🕐 Planul Sesiunii

| Interval | Activitate | Detaliu | Cine |
|---|---|---|---|
| `09:00–09:30` | **Arhitectura GPIO MCX** | PORT vs GPIO pe Cortex-M33: pin mux, pull-up/down, drive strength. Registre: `PORT_PCRn`, `GPIO_PDDR`, `GPIO_PDOR`, `GPIO_PDIR`. Comparație DDR/PORT AVR. | cadru UPB |
| `09:30–10:15` | **Demo SDK: RGB LED** | `CLOCK_EnableClock(kCLOCK_Port3)`, `PORT_SetPinMux(PORT3,12,kPORT_MuxAsGpio)`, `GPIO_PinInit()`, `GPIO_PinWrite()`. Demonstrație anod comun. | cadru UPB |
| `10:15–11:00` | **Lab + GenAI session** | Studenții: semafor FSM (RED 2s → YELLOW 1s → GREEN 2s) + reset buton SW3. Prompt structurat cu context complet. Identificare erori AI. | student + mentor |

## 🤖 Prompte pentru Asistentul AI UPB

> **Regulă:** Copiați prompt-ul complet — contextul hardware este obligatoriu.
> AI-ul va genera cod greșit (pentru alte familii NXP sau Arduino) fără aceste informații.

### Prompt: GPIO config + semafor

```text
Context hardware: FRDM-MCXA153, MCX A153 Cortex-M33 @ 96 MHz, SDK MCUXpresso 24.12.
RGB LED D3: R=PIO1_7, G=PIO3_12, B=PIO3_13 — ANOD COMUN (LOW=aprins, HIGH=stins).
Buton SW3: PIO0_6, pull-up intern activat.
Sarcina: funcție void led_set(bool r, bool g, bool b) + FSM semafor RED→YELLOW→GREEN.
Include:
  CLOCK_EnableClock(kCLOCK_Port1), CLOCK_EnableClock(kCLOCK_Port3)
  PORT_SetPinMux(PORT3, 12u, kPORT_MuxAsGpio)
  GPIO_PinInit(), GPIO_PinWrite()
NU folosi Arduino/HAL generic — SDK NXP exclusiv.
```

### Prompt: Debug GPIO

```text
FRDM-MCXA153, SDK MCUXpresso 24.12.
Problema: PORT_SetPinMux(PORT3, 12u, kPORT_MuxAsGpio) + GPIO_PinWrite(GPIO3, 12u, 0u)
dar LED-ul verde nu se aprinde. LED-ul roșu (PIO1_7) funcționează corect.
Ce verific?
Hint: CLOCK_EnableClock pe port-ul 3, valoarea kPORT_MuxAsGpio,
și logica anodului comun (LOW = aprins).
```

## ⚠️ Capcane Critice

> Ce GenAI **nu știe** fără context explicit — verificați înainte de upload pe placă.

- **PORT și GPIO sunt periferice separate** — `CLOCK_EnableClock(kCLOCK_PortX)` trebuie apelat pentru **fiecare port** folosit
- `kPORT_MuxAsGpio` poate diferi ca valoare numerică între familii MCX — verificați enum-ul din `fsl_port.h`
- `GPIO_PinWrite(GPIO3, 12u, 0u)` = LED verde **APRINS** (nu stins — anod comun)

## ✅ Deliverable

> Semafor FSM funcțional pe RGB + buton SW3 polling + 3 prompts documentate în Moodle cu analiza erorilor AI

---

[← L0: Setup Toolchain & Prima Aplicație](l0-setup-toolchain.md) · [L2: LPUART — Comunicație Serială →](l2-lpuart.md)
