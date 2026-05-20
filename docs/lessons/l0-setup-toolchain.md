---
title: "L0 — Setup Toolchain & Prima Aplicație"
description: "Hello World pe FRDM-MCXA153"
nav_order: 1
parent: Lecții FRDM-MCXA153
layout: default
---

# 🛠️ L0 — Setup Toolchain & Prima Aplicație

**Hello World pe FRDM-MCXA153**

---

| | |
|---|---|
| **Ziua / Sesiunea** | Ziua 0 — Pre-school (asincron) |
| **Periferic** | `Toolchain · Build System · Debug` |
| **Durată** | ~3h asincron |
| **Responsabil** | Stefan Turcu |
| **Hardware** | FRDM-MCXA153 + cablu USB Type-C · PC Windows/Linux/macOS |

## 📌 Context și Motivație

Pasul zero, livrat asincron înainte de ziua 1 fizică. Participanții primesc un repo GitHub template (`ipcei-upb/frdm-mcxa153-template`) cu structura CMake pre-configurată. Stefan este disponibil pe Discord pentru troubleshooting. Scopul: toată lumea vine în ziua 1 cu mediul de lucru funcțional.

> **Board:** FRDM-MCXA153 · MCX A153 (Cortex-M33 @ 96 MHz) · SDK MCUXpresso 24.12 · VS Code + CMake

## 🎯 Obiective

1. VS Code + extensii (C/C++, CMake Tools, Cortex-Debug) instalate și funcționale
2. MCUXpresso SDK 24.12 descărcat, `NXP_SDK_ROOT` configurat în `CMakePresets.json`
3. Prima aplicație (blink LED verde) compilată, flash-uită și rulând pe placă
4. MCU-Link CMSIS-DAP debugging funcțional — breakpoint pe `main()` în VS Code

## 🕐 Planul Sesiunii

| Interval | Activitate | Detaliu | Cine |
|---|---|---|---|
| `Async` | **Instalare toolchain** | VS Code + extensii C/C++, CMake Tools, Cortex-Debug. ARM GNU Toolchain 13.x. OpenOCD pentru CMSIS-DAP. | student |
| `Async` | **Clone repo template** | `git clone https://github.com/ipcei-upb/frdm-mcxa153-template`<br>Structura: `src/`, `boards/`, `CMakeLists.txt`, `CMakePresets.json` | student |
| `Async` | **Config SDK** | Descărcare `SDK_24.12_FRDM-MCXA153` de pe mcuxpresso.nxp.com. Setare `NXP_SDK_ROOT` în `CMakePresets.json`. | student |
| `Async` | **Build + flash** | `cmake --preset frdm-mcxa153-debug` → `cmake --build build/` → drag-drop `.bin` pe MCU-Link drive. | student |
| `Async` | **Debug session** | F5 în VS Code → breakpoint pe `GPIO_PinWrite()` → step-over → LED schimbă starea. | student |

## 🤖 Prompte pentru Asistentul AI UPB

> **Regulă:** Copiați prompt-ul complet — contextul hardware este obligatoriu.
> AI-ul va genera cod greșit (pentru alte familii NXP sau Arduino) fără aceste informații.

### Prompt: Setup CMake

```text
Context: FRDM-MCXA153, MCX A153 Cortex-M33, SDK MCUXpresso 24.12.
Toolchain: ARM GNU Toolchain 13.x, CMake 3.27+, debugger MCU-Link OB (CMSIS-DAP).
Problema: CMakePresets.json nu găsește NXP_SDK_ROOT pe Windows (PATH cu spații).
Cum configurez variabila de mediu corect și cum evit path-uri cu spații în CMake?
```

### Prompt: Blink LED verde

```text
Context hardware: FRDM-MCXA153, SDK MCUXpresso 24.12, VS Code + CMake.
RGB LED D3: R=PIO1_7, G=PIO3_12, B=PIO3_13 — ANOD COMUN → LOW=aprins.
Scrie codul complet pentru blink LED verde la 500ms.
Include:
  CLOCK_EnableClock(kCLOCK_Port3)
  PORT_SetPinMux(PORT3, 12u, kPORT_MuxAsGpio)
  GPIO_PinInit(), GPIO_PinWrite(), SDK_DelayAtLeastUs()
Explică de ce GPIO LOW aprinde LED-ul (anod comun).
```

## ⚠️ Capcane Critice

> Ce GenAI **nu știe** fără context explicit — verificați înainte de upload pe placă.

- **Path SDK cu spații** → CMake eșuează; folosiți path fără spații (ex: `C:/NXP/SDK`)
- **MCU-Link driver**: Windows necesită Zadig pentru OpenOCD; Linux necesită regulă udev
- **LED RGB anod comun**: `GPIO LOW` = LED **APRINS** (opus intuiției Arduino standard)

## ✅ Deliverable

> Repo GitHub personal cu blink funcțional + screenshot debug session cu breakpoint activ

---

[L1: GPIO — Control Digital & Butoane →](l1-gpio.md)
