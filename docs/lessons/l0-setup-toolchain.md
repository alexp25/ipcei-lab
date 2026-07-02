---
title: "L0 - Toolchain Setup and First Application"
description: "Hello World and first debug session on FRDM-MCXA153"
nav_order: 1
parent: FRDM-MCXA153 Lessons
layout: lesson
source_url: https://github.com/alexp25/ipcei-lab/tree/main/src/lab_hello/hello_world
---

# L0 - Toolchain Setup and First Application

**Hello World on FRDM-MCXA153**

---

| | |
|---|---|
| **Session** | Setup workshop |
| **Topic** | `Toolchain`, `Build System`, `Debug` |
| **Hardware** | FRDM-MCXA153, USB Type-C cable, Windows/Linux/macOS PC |

## Context and Motivation

This first lab is the setup checkpoint for the course. Before doing peripheral work, every student must have a working MCUXpresso for VS Code environment, an imported MCUXpresso SDK repository, a buildable project, and a working debug session through the on-board MCU-Link probe.

> **Board:** FRDM-MCXA153, MCX A153 Cortex-M33, MCUXpresso SDK, VS Code, CMake, ARM GCC.

## Objectives

By the end of this lab you should be able to:

1. Install MCUXpresso for VS Code and its required tools.
2. Import the MCUXpresso SDK repository.
3. Create or import a first FRDM-MCXA153 example project.
4. Build the project with CMake presets.
5. Flash and debug the board through MCU-Link.
6. Use an AI assistant to inspect project structure and diagnose build issues.

## Session Plan

| Step | Activity | Result |
|---|---|---|
| 1 | Install MCUXpresso for VS Code | VS Code can see the NXP extension and installer |
| 2 | Install tool dependencies | ARM GCC, LinkServer, Config Tools are available |
| 3 | Import SDK repository | Local `mcuxsdk` repository exists |
| 4 | Import or create project | FRDM-MCXA153 project opens in VS Code |
| 5 | Build | `.elf` and `.bin` files are generated |
| 6 | Debug | Breakpoint at `main()` works |

## AI Assistant Prompts

Copy full prompts. Embedded answers are only useful when the assistant receives board, SDK, and toolchain context.

### Prompt: CMake Setup

```text
Context: FRDM-MCXA153, MCX A153 Cortex-M33, MCUXpresso SDK, ARM GCC, CMake, MCU-Link/LinkServer.
Problem: CMakePresets.json cannot find the SDK path on Windows. The path may contain spaces.
Explain how to configure the SDK/toolchain paths correctly and how to avoid path issues in CMake presets.
```

### Prompt: Blink the Green LED

```text
Context hardware: FRDM-MCXA153, MCX A153 Cortex-M33, MCUXpresso SDK, VS Code + CMake.
RGB LED D3 is active-low/common-anode. Green LED is on GPIO3 pin 12.
Write a minimal C example that configures the pin as GPIO output and blinks it every 500 ms.
Use MCUXpresso SDK APIs: CLOCK_EnableClock, PORT_SetPinMux, GPIO_PinInit, GPIO_PinWrite, SDK_DelayAtLeastUs.
Explain why GPIO LOW turns the LED on.
```

### Prompt: Explain the Project Structure

```text
Context: embedded C project for FRDM-MCXA153, MCUXpresso SDK, VS Code, CMake, ARM GCC, LinkServer/MCU-Link.
I opened an imported/generated project. Explain the project structure for a beginner:
1. how execution reaches main();
2. the role of CMakeLists.txt and CMakePresets.json;
3. the role of startup/linker files;
4. the role of generated files such as pin_mux.c, clock_config.c, board.c and .mex;
5. where application logic should be changed;
6. which generated files should be edited only through Config Tools;
7. how to navigate to SDK definitions such as GPIO_PinWrite().
Do not rewrite the code; explain the structure and execution flow.
```

## Setup Steps

### 1. Install MCUXpresso for VS Code

Install **MCUXpresso for VS Code** from the Extensions Marketplace. Open the extension view and run **Check Tool Dependencies**.

![MCUXpresso for VS Code - Check Tool Dependencies](../img/lab0/install_mcuxpresso_extension.png)

Click **Open Installer** to launch the MCUXpresso installer.

### 2. Install Required Components

Install at least:

- **Arm GNU Toolchain support**: compiler and libraries;
- **LinkServer**: GDB server for the on-board MCU-Link probe;
- **MCUXpresso Configuration Tools**: graphical Pins/Clocks/Peripherals configurator.

![MCUXpresso Installer - LinkServer and Config Tools selected](../img/lab0/install_linkserver_before_debug.png)

Wait until the installer reports that installation completed successfully.

### 3. Import the SDK Repository

In **MCUXPRESSO FOR VS CODE -> QUICKSTART PANEL**, select **Import Repository**.

Use a path without spaces, for example:

```text
C:\WORKSPACE\proiecte\ipcei-lab\src\sdks
```

![Import Repository - MCUXpresso SDK in progress](../img/lab0/import_mcuxpresso_repo_takes_a_long_time.png)

The import can take a long time because it downloads the SDK repository and west/git dependencies.

### 4. Import an Example or Create a New Project

After the SDK is available, use **Import Example from Repository** and choose:

- **Board:** FRDM-MCXA153
- **Template:** `demo_apps/hello_world` or `led_blinky`

![Import Example from Repository - hello_world on FRDM-MCXA153](../img/lab0/import_example.png)

You can also create a project from scratch with **New Project Wizard**.

![New Project Wizard - FRDM-MCXA153 selected](../img/lab0/create_new_project.png)

### 5. Build the Project

Use the VS Code build button or run:

```powershell
cmake --preset debug
cmake --build --preset debug
```

![led_blinky.c opened - successful build in terminal](../img/lab0/new_project_wizard.png)

### 6. Fix Build Errors with AI Assistance

If the build fails, use the AI assistant with precise error messages and project context.

![AI diagnoses SDK path issue](../img/lab0/use_genai_to_fix_build_error_sdk_path.png)

![AI identifies missing CMSIS include](../img/lab0/use_genai_to_fix_build_error_cmsis.png)

![CMakeLists.txt after adding the required CMSIS include](../img/lab0/use_genai_to_fix_build_error_cmsis_fixed.png)

### 7. Open MCUXpresso Config Tools

Peripheral configuration is stored in a `.mex` file. Open it from Windows Explorer or from VS Code with **Open with MCUXpresso Config Tools**.

![led_blinky.mex in Explorer](../img/lab0/configurator_open_mex_file.png)

![VS Code context menu - Open with Config Tools](../img/lab0/open_with_config.png)

### 8. Debug on the Board

Connect the FRDM-MCXA153 through the MCU-Link USB port and press **F5** or **Run and Debug**.

![Active debug session](../img/lab0/run_debug.png)

Set a breakpoint and step through the program.

![Breakpoint active in debug session](../img/lab0/run_debug_breakpoint.png)

If you see `No probe detected`, check that LinkServer is installed and that the board is connected to the correct USB port.

![No probe detected error](../img/lab0/run_error_no_probe_detected.png)

### 9. Ask AI to Explain Generated Code

Use the assistant to understand generated SDK files and the application entry point.

![AI explains led_blinky.c](../img/lab0/codex_explain_code.png)

## Common Pitfalls

- Avoid SDK paths with spaces.
- On Windows, install the MCU-Link/LinkServer drivers.
- On Linux, add the required udev rules for debug probes.
- The FRDM RGB LEDs are active-low: `GPIO LOW` means LED on.
- Do not edit generated files casually; prefer Config Tools for `pin_mux.c`, `clock_config.c`, and `peripherals.c`.

## Deliverable

Submit a personal repository or archive containing:

1. a buildable first FRDM-MCXA153 project;
2. a screenshot of a successful debug session with a breakpoint at `main()`;
3. a short note describing one setup issue and how you solved it.

---

[L1: GPIO - Digital Control and Buttons ->](../l1-gpio)

