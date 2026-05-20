---
title: "Lecții FRDM-MCXA153"
description: "GenAI & Embedded Engineering — Școala de vară IPCEI NXP 2026"
nav_order: 1
has_children: true
layout: default
---

# 📚 Lecții FRDM-MCXA153

**GenAI & Embedded Engineering — Școala de vară IPCEI UPB–NXP · Ediția 1, 2026**

---

## Contextul Hardware

| Componentă | Specificație |
|---|---|
| **MCU** | MCXA153VLH |
| **CORE** | Arm Cortex-M33 @ 96 MHz |
| **SDK** | MCUXpresso SDK 24.12 |
| **TOOLCHAIN** | VS Code + CMake 3.27+ + ARM GNU Toolchain 13.x |
| **DEBUGGER** | MCU-Link OB (CMSIS-DAP, LPC55S69) |
| **LED** | D3 RGB LED — R=PIO1_7, G=PIO3_12, B=PIO3_13 — ANOD COMUN (LOW = aprins) |
| **BTN** | SW2=PIO1_7 (ISP/user), SW3=PIO0_6 (wake-up/user) |
| **UART** | LPUART0 → MCU-Link VCOM → USB Type-C J6 |
| **I2C** | LPI2C0 → P3T1755DPJ on-board (adresa 0x48, I3C/I2C, temperatură) |
| **SPI** | LPSPI0 → Arduino header (D10–D13), LPSPI1 → Mikroe click |
| **ADC** | LPADC0 — 12 biți, single-ended, intrări Arduino A0–A5 |
| **TIMER** | SCTimer/PWM (32-bit, multi-match), CTIMER0/1/2/3, LPIT0 |
| **CLK** | FRO96M system clock (default SDK board_init) |

> **Toolchain:** VS Code + CMake 3.27+ + ARM GNU Toolchain 13.x + MCUXpresso SDK 24.12

---

## Structura Lecțiilor

| Lecție | Periferică | Ziua | Durată |
|---|---|---|---|
| [L0 — Setup Toolchain & Prima Aplicație](l0-setup-toolchain.md) | `Toolchain · Build System · Debug` | Ziua 0 — Pre-school (asincron) | ~3h asincron |
| [L1 — GPIO — Control Digital & Butoane](l1-gpio.md) | `PORT · GPIO · FGPIO` | Ziua 1, 22 iunie — dimineață | 2h (09:00–11:00) |
| [L2 — LPUART — Comunicație Serială](l2-lpuart.md) | `LPUART0 · VCOM · FRO_HF_DIV` | Ziua 1, 22 iunie — după-amiază | 2h (13:00–15:00) |
| [L3 — Întreruperi Externe & NVIC](l3-intreruperi.md) | `GPIO IRQ · NVIC · SysTick` | Ziua 2, 23 iunie — dimineață | 2h (09:00–11:00) |
| [L4 — Timer & PWM — SCTimer și CTIMER](l4-timer-pwm.md) | `SCTimer/PWM · CTIMER0 · FRO_HF` | Ziua 2, 23 iunie — după-amiază | 2h (13:00–15:00) |
| [L5 — LPADC — Conversie Analogică 12 biți](l5-lpadc.md) | `LPADC0 · Command Queue · VREFH 3.3V` | Ziua 3, 24 iunie — dimineață | 2h (09:00–11:00) |
| [L6 — LPSPI — Comunicație SPI & Device Extern](l6-lpspi.md) | `LPSPI0 · Arduino Header · FIFO` | Ziua 3, 24 iunie — după-amiază | 2h (13:00–15:00) |
| [L7 — LPI2C — Senzorul P3T1755 On-Board](l7-lpi2c-p3t1755.md) | `LPI2C0 · P3T1755DPJ · 400 kHz Fast Mode` | Ziua 4, 25 iunie — dimineață | 2h (09:00–11:00) |
| [L8 — Integrare — Sistem Complet cu FSM](l8-integrare.md) | `Toate perifericele · FSM · Super-loop` | Ziua 4, 25 iunie — după-amiază + Ziua 5 | 3h (13:00–16:00) |
| [L9 — Testing, Documentare & Prezentare Finală](l9-testing-docs.md) | `Unity · Doxygen · GitHub Actions · Show & Tell` | Ziua 5 + Săptămâna 2 online | 2h sesiune + async |

---

## Regula de Aur pentru Prompte

Fiecare prompt trebuie să conțină:

1. **MCU explicit** — `MCXA153`, Cortex-M33 @ 96 MHz
2. **SDK versiune** — MCUXpresso SDK 24.12
3. **Clock source** pentru periferica respectivă
4. **Pinout-ul specific** plăcii FRDM-MCXA153
5. **Ce s-a încercat deja** — nu cerați cod din senin

> Fără aceste informații, AI-ul va genera cod pentru alte familii NXP (Kinetis, LPC) sau pentru Arduino.

---

## Caiet de Practică

Fiecare lecție contribuie la **caietul de practică** al echipei:

| Capitol | Contribuție |
|---|---|
| Cap. 1 — Context | L0: motivația și toolchain-ul ales |
| Cap. 2 — Metodologie GenAI | L1–L7: promptele reprezentative per periferic |
| Cap. 3 — Arhitectura | L8: FSM și diagrama stărilor |
| Cap. 4 — Implementare | L1–L8: cod + teste |
| Cap. 5 — Comparativ baseline vs AI | Toate: tabel timp/calitate/bugs |
| Cap. 6 — Reflecție | L9: ce a mers cu AI, ce nu, concluzii |

---

*Proiect IPCEI ME/CT · Tema 6: Upgrading Curricula · UPB – NXP Semiconductors · 2026*
