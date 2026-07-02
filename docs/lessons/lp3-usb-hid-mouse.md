---
title: "LP3 - USB HID Mouse Jiggler"
description: "Adapting the SDK usb_device_hid_mouse_lite example into a USB HID mouse jiggler"
nav_order: 3
parent: Additional Demos
layout: lesson
source_url: https://github.com/alexp25/ipcei-lab/tree/main/src/lab_usb_hid/usb_device_hid_mouse_lite/bm
---

# LP3 - USB HID Mouse Jiggler

**From SDK USB HID mouse example to SW3-controlled mouse jiggler**

---

| | |
|---|---|
| **Peripherals** | `USB0`, `USB HID`, `GPIO`, `PORT` |
| **Duration** | 2 h |
| **Hardware** | FRDM-MCXA153, USB device port, SW3, red LED |
| **SDK source project** | `src/sdks/mcuxsdk/examples/usb_examples/usb_device_hid_mouse_lite` |
| **Local project** | `src/lab_usb_hid/usb_device_hid_mouse_lite/bm` |
| **Result** | Board enumerates as a HID mouse; SW3 enables/disables small cursor movements; LED shows active state |

## Context

This lab starts from the NXP MCUXpresso SDK `usb_device_hid_mouse_lite` example. The original example enumerates as a USB HID mouse and sends relative mouse reports that move the cursor in a rectangular pattern.

Your task is to turn it into a simple mouse jiggler:

- the local lab project must compile local sources, not SDK example sources;
- the movement pattern must be different from the original rectangle;
- SW3 toggles the jiggler on/off;
- the red LED indicates active state;
- a short explanation of the final code is generated with AI and checked manually.

This SDK sample does not use a `.mex` file. Configuration is done in code and CMake.

## Objectives

By the end of the lab you should be able to:

1. Copy/adapt a USB HID SDK example into a local project.
2. Modify HID mouse reports.
3. Update `CMakeLists.txt` so the build uses local sources.
4. Compare the local project with the original SDK example.
5. Implement SW3 toggle and LED active indicator.
6. Explain how a HID mouse report works.

## Functional Requirements

The final implementation must:

1. enumerate as a USB HID mouse;
2. move the cursor relative to its current position;
3. use small `8` pixel left/right/up/down movements;
4. avoid the original continuous rectangle path;
5. toggle active/inactive state with SW3;
6. turn the red LED on when active;
7. turn the red LED off when inactive.

## Project Structure

Start from the SDK example:

```text
src/sdks/mcuxsdk/examples/usb_examples/usb_device_hid_mouse_lite
```

Create/use a local lab project:

```text
src/lab_usb_hid/usb_device_hid_mouse_lite/bm/
  CMakeLists.txt
  CMakePresets.json
  prj.conf
  mouse.c
  mouse.h
  usb_device_ch9.c/.h
  usb_device_descriptor.c/.h
  usb_device_hid.c/.h
```

## Import SDK Project Only for Testing

You can import the SDK example briefly to verify the toolchain and board.

![Import SDK USB HID mouse project](../img/labp2/import_sdk_project_hid_usb.png)

Do not keep editing the SDK copy. The lab project should be local and independent.

## CMake Warning

Many SDK examples list source files relative to `${SdkRootDirPath}`:

```cmake
mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "examples/usb_examples/usb_device_hid_mouse_lite/bm/mouse.c"
)
```

For this lab, CMake must compile local files:

```cmake
mcux_add_include(
  BASE_PATH "${CMAKE_CURRENT_LIST_DIR}"
  INCLUDES "."
)

mcux_add_source(
  BASE_PATH "${CMAKE_CURRENT_LIST_DIR}"
  SOURCES
    "mouse.c"
    "mouse.h"
    "usb_device_ch9.c"
    "usb_device_ch9.h"
    "usb_device_descriptor.c"
    "usb_device_descriptor.h"
    "usb_device_hid.c"
    "usb_device_hid.h"
)
```

If this step is missed, you can edit local `mouse.c` while the firmware still uses the SDK file.

## HID Mouse Report

The example uses a 4-byte mouse report:

| Byte | Meaning |
|---:|---|
| `0` | mouse buttons |
| `1` | relative X delta |
| `2` | relative Y delta |
| `3` | wheel |

Suggested jiggler pattern:

```c
static const int8_t s_JigglePattern[][2] = {
    { 8,  0},
    {-8,  0},
    { 0,  8},
    { 0, -8},
};
```

Fill the report:

```c
g_UsbDeviceHidMouse.buffer[0] = 0U;
g_UsbDeviceHidMouse.buffer[1] = (uint8_t)s_JigglePattern[index][0];
g_UsbDeviceHidMouse.buffer[2] = (uint8_t)s_JigglePattern[index][1];
g_UsbDeviceHidMouse.buffer[3] = 0U;
```

When disabled, send neutral movement:

```c
g_UsbDeviceHidMouse.buffer[1] = 0U;
g_UsbDeviceHidMouse.buffer[2] = 0U;
```

## SW3 Toggle

SW3 is active-low. A simple solution uses polling with debounce:

1. read `GPIO_PinRead(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN)` periodically;
2. require a stable value for several iterations;
3. on a stable transition to `0`, toggle `jiggle_enabled`;
4. do not toggle again until the button is released.

## LED Indicator

Use the board LED macros/conventions:

```c
if (jiggle_enabled)
{
    GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, LOGIC_LED_ON);
}
else
{
    GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, LOGIC_LED_OFF);
}
```

Remember: the FRDM LEDs are active-low.

## AI Task Prompt

```text
I have an MCUXpresso SDK project for FRDM-MCXA153 based on:
src/sdks/mcuxsdk/examples/usb_examples/usb_device_hid_mouse_lite
I want to turn it into a USB HID mouse jiggler.
Requirements:
- local project must compile sources from the lab folder, not the SDK folder;
- update CMakeLists.txt for local sources;
- change HID mouse reports to move 8 pixels left/right/up/down relative to current position;
- movement must not be the original rectangle path;
- SW3 enables/disables the jiggler;
- red LED is on when active and off when inactive;
- there is no .mex file, so pin configuration is in code.
Explain which files to modify and propose a minimal patch.
```

## Compare with the SDK Example

Ask AI to compare:

```text
Compare these two projects:
1. src/sdks/mcuxsdk/examples/usb_examples/usb_device_hid_mouse_lite
2. src/lab_usb_hid/usb_device_hid_mouse_lite/bm
Focus on CMakeLists.txt, local vs SDK sources, mouse.c logic, HID descriptors, SW3/LED support, and removal of the original rectangle path.
```

Then verify manually that the build log uses local object files.

## Board Verification

1. Build:

   ```powershell
   cmake --preset debug
   cmake --build --preset debug
   ```

2. Flash the binary:

   ```text
   src/lab_usb_hid/usb_device_hid_mouse_lite/bm/debug/dev_hid_mouse_lite_bm.bin
   ```

3. Connect the board through the USB device port.
4. Confirm that the OS sees a HID mouse.
5. Verify short left/right/up/down movement, not a rectangle.
6. Press SW3 to stop movement and turn LED off.
7. Press SW3 again to restart movement and turn LED on.

## Common Problems

| Symptom | Likely cause | Check |
|---|---|---|
| Cursor still moves in rectangle | build still uses SDK sources | check `CMakeLists.txt` and rebuild cache |
| Device enumerates but does not move | deltas are zero or disabled | check report bytes 1/2 and `jiggle_enabled` |
| SW3 does not toggle | wrong GPIO or active-low logic | check `BOARD_SW3_*` and debounce |
| LED logic inverted | active-low LED | use `LOGIC_LED_ON/OFF` |
| Config Tools cannot open project | no `.mex` | this lab is code/CMake only |

## Final Documentation Prompt

```text
Explain the final USB HID mouse jiggler code for FRDM-MCXA153.
Include:
- how the HID mouse report works;
- how the movement pattern differs from the SDK example;
- how SW3 toggles the jiggler;
- how the LED indicates state;
- why CMakeLists.txt must use local sources instead of SDK sources.
Write for beginner embedded students.
```

## Deliverable

Submit:

1. local project that builds;
2. `CMakeLists.txt` using local sources;
3. modified `mouse.c` jiggler pattern;
4. working SW3 toggle;
5. working LED indicator;
6. short comparison with the SDK example;
7. short AI-generated explanation corrected by you.

