---
title: "L0 — Setup Toolchain & Prima Aplicație"
description: "Hello World pe FRDM-MCXA153"
nav_order: 1
parent: Lecții FRDM-MCXA153
layout: lesson
---

# 🛠️ L0 — Setup Toolchain & Prima Aplicație

**Hello World pe FRDM-MCXA153**

---

| | |
|---|---|
| **Ziua / Sesiunea** | Ziua 1 — Setup Workshop |
| **Periferic** | `Toolchain · Build System · Debug` |
| **Durată** | ~3h |
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

## 🤖 Prompt-uri pentru Asistentul AI UPB

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

## ⚠️ Capcane

> Ce GenAI **nu știe** fără context explicit — verificați înainte de upload pe placă.

- **Path SDK cu spații** → CMake eșuează; folosiți path fără spații (ex: `C:/NXP/SDK`)
- **MCU-Link driver**: Windows necesită Zadig pentru OpenOCD; Linux necesită regulă udev
- **LED RGB anod comun**: `GPIO LOW` = LED **APRINS** (opus intuiției Arduino standard)


## 🔧 Pași de Setup

### 1. Instalare extensie MCUXpresso pentru VS Code

Căutați **MCUXpresso for VS Code** în Extensions Marketplace și instalați-o. La prima deschidere apare pagina **Discover NXP MCUXpresso for VS Code** cu secțiunea **Check Tool Dependencies**.

![MCUXpresso for VS Code — Check Tool Dependencies](../img/lab0/install_mcuxpresso_extension.png)

Apăsați **Open Installer** pentru a lansa MCUXpresso Installer.

### 2. Instalare componente prin MCUXpresso Installer

Selectați și instalați cel puțin:
- **NXP libraries and header files for Arm GNU Toolchain** — compilatorul ARM
- **LinkServer** — server GDB pentru probe-ul MCU-Link de pe placă
- **MCUXpresso Configuration Tools** — unealta grafică pentru configurare pini, clock, periferice

![MCUXpresso Installer — LinkServer și Config Tools selectate](../img/lab0/install_linkserver_before_debug.png)

> Instalarea durează câteva minute. Urmăriți log-ul din partea de jos a ferestrei și așteptați `*** Installation completed successfully ***`.

### 3. Import SDK repository

În panoul **MCUXPRESSO FOR VS CODE → QUICKSTART PANEL**, apăsați **Import Repository**. Configurați:
- **Repository:** MCUXpresso SDK (`https://github.com/nxp-mcuxpresso/mcuxsdk-manifests`)
- **Name:** `mcuxsdk`
- **Location:** un folder fără spații în cale (ex. `c:\WORKSPACE\proiecte\ipcei-lab\src\sdks`)

![Import Repository — MCUXpresso SDK în progres](../img/lab0/import_mcuxpresso_repo_takes_a_long_time.png)

> **Atenție:** Importul SDK durează mult (descarcă ~2 GB via west/git). Folosiți acest timp pentru a citi specificațiile plăcii FRDM-MCXA153.

### 4. Import exemplu din repository

După ce SDK-ul este importat, apăsați **Import Example from Repository**. Selectați:
- **Board:** FRDM-MCXA153
- **Template:** `demo_apps/hello_world`

![Import Example from Repository — hello_world pe FRDM-MCXA153](../img/lab0/import_example.png)

Alternativ, puteți crea un proiect nou de la zero cu **New Project Wizard**:

![New Project Wizard — FRDM-MCXA153 selectat](../img/lab0/create_new_project.png)

### 5. Build proiect

Apăsați **Build** din panoul proiectului (sau `Ctrl+Shift+B`). Prima compilare rulează CMake și generează fișierele de build.

![led_blinky.c deschis — build cu succes în terminal](../img/lab0/new_project_wizard.png)

### 6. Rezolvare erori de build cu AI

Dacă apar erori de build, folosiți asistentul AI integrat în VS Code pentru diagnoză și corectare.

**Eroare cale SDK în CMakePresets.json:**

![AI diagnostichează eroarea de cale SDK](../img/lab0/use_genai_to_fix_build_error_sdk_path.png)

**Eroare include CMSIS lipsă:**

![AI identifică include-ul fsl_lpuart_cmsis.h lipsă](../img/lab0/use_genai_to_fix_build_error_cmsis.png)

**CMakeLists.txt după corectare:**

![CMakeLists.txt cu include-urile CMSIS adăugate corect](../img/lab0/use_genai_to_fix_build_error_cmsis_fixed.png)

### 7. Deschidere MCUXpresso Config Tools

Configurația perifericelor (pini, clock, periferice) este stocată în fișierul `.mex`. Poate fi deschis în două moduri:

**Din Windows Explorer** — fișierul `led_blinky.mex` are tipul *MCUXpresso Config Tools Settings File*:

![led_blinky.mex în Explorer — tip MCUXpresso Config Tools Settings File](../img/lab0/configurator_open_mex_file.png)

**Din VS Code** — click dreapta pe orice fișier din proiect → **Open with MCUXpresso Config Tools**:

![Meniu contextual VS Code — Open with MCUXpresso Config Tools](../img/lab0/open_with_config.png)

### 8. Debug pe placă

Conectați placa FRDM-MCXA153 prin USB Type-C (portul J6 — MCU-Link) și apăsați **F5** sau butonul **Run and Debug**.

![Debug session activă — led_blinky rulând pe placă](../img/lab0/run_debug.png)

Setați un breakpoint în cod și observați execuția pas cu pas:

![Breakpoint activ în debug session](../img/lab0/run_debug_breakpoint.png)

> **Eroare frecventă `No probe detected`:** apare dacă LinkServer nu este instalat sau placa nu e conectată la portul corect. Verificați că LED-ul verde de pe MCU-Link este aprins.

![Eroare — No probe detected](../img/lab0/run_error_no_probe_detected.png)

### 9. AI explică codul generat

Folosiți Copilot sau Claude din VS Code pentru a înțelege codul SDK generat automat:

![AI explică structura led_blinky.c](../img/lab0/codex_explain_code.png)

---

## ✅ Deliverable

> Repo GitHub personal cu blink funcțional + screenshot debug session cu breakpoint activ

---

[L1: GPIO — Control Digital & Butoane →](../l1-gpio)
