---
title: "LP1 - Configure New Project"
description: "Using MCUXpresso Config Tools to configure pins, peripherals, clocks, and generated code in a new FRDM-MCXA153 project"
nav_order: 0
parent: Additional Demos
layout: lesson
source_url: https://github.com/alexp25/ipcei-lab/tree/main/src/lab_new_project/main
---

# LP1 - Configure New Project

**Using MCUXpresso Config Tools as the configuration source for a new FRDM-MCXA153 project**

---

| | |
|---|---|
| **Board** | FRDM-MCXA153 |
| **Starting point** | New project template from `src/lab_new_project/main` |
| **Tooling** | MCUXpresso for VS Code + MCUXpresso Config Tools |
| **Config file** | Project `.mex` file |
| **Generated code** | Pins, clocks, peripherals, and component configuration |
| **Goal** | Open the project configuration, change hardware settings, review generated code, and apply the update safely |

## Context

This page shows the normal workflow for configuring a fresh FRDM-MCXA153 project with MCUXpresso Config Tools. Start from the new project template created in **LP1 - New Project Template**, then use the `.mex` configuration file as the entry point for the Pins, Peripherals, Clocks, and Components tools.

The important habit is to make hardware configuration changes in Config Tools first, then review the generated file changes before applying them to the project. That keeps the C code, pin muxing, clock tree, interrupt setup, and component metadata synchronized.


## Open the Project in Config Tools

Clone or open the starter project from the repository, then use the MCUXpresso extension in VS Code to open the project with MCUXpresso Config Tools.

![Opening the project with MCUXpresso Config Tools](../img/labp0/01_open_config_tools.png)

*Figure 1. Opening the project through the MCUXpresso extension. The project should already contain the `.mex` file used by Config Tools.*

Open the existing `.mex` configuration file from the project. Make sure the selected project folder is the same folder that contains the CMake project you are building.

![Opening the existing MEX configuration file](../img/labp0/02_open_mex_file.png)

*Figure 2. Loading the existing project configuration. Reusing the project `.mex` file prevents Config Tools from generating code into an unrelated folder.*

## Configure Pins

Use the **Pins** tool for package pin assignment and pin electrical characteristics. In this example, pin `P1_7` is assigned to `GPIO1`.

![Selecting GPIO1 on P1_7 in the Pins tool](../img/labp0/03_select_gpio_pin.png)

*Figure 3. Selecting a GPIO function for a package pin in the Pins tool.*

After the signal is selected, configure the pin properties required by the application. For a button input, that usually means input direction and an enabled pull-up resistor.

![Configuring GPIO pin characteristics](../img/labp0/04_configure_pin_characteristics.png)

*Figure 4. Configuring pin direction and pull-up settings. These choices are generated into the pin mux source files.*

## Configure Peripherals

Use the **Peripherals** tool for peripheral drivers, interrupts, component instances, and generated initialization code.

For a GPIO input interrupt, enable the GPIO peripheral instance and configure the corresponding interrupt, such as `GPIO1_IRQn`.

![Configuring GPIO1 interrupt support](../img/labp0/05_gpio_interrupt_peripheral.png)

*Figure 5. Configuring the GPIO peripheral and enabling the interrupt used by the application.*

For PWM output, add or enable the FlexPWM component, then configure the submodule and channel used by the signal.

![Enabling the FlexPWM component](../img/labp0/06_enable_flexpwm_component.png)

*Figure 6. Adding or enabling a FlexPWM component from the Peripherals tool.*

![Configuring FlexPWM submodule 0 channel A](../img/labp0/07_configure_flexpwm_channel.png)

*Figure 7. Configuring FlexPWM submodule 0 channel A. The generated code will initialize the selected PWM block and channel.*

If the application needs the system tick interrupt, enable the `SysTick_IRQn` entry through the NVIC configuration.

![Enabling SysTick through NVIC configuration](../img/labp0/08_enable_systick_nvic.png)

*Figure 8. Adding the SysTick interrupt through the NVIC peripheral configuration.*

Some driver features require software components. Enable the required components before generating code so that the project metadata, include paths, and generated initialization code stay aligned.

![Enabling required software components](../img/labp0/09_enable_software_components.png)

*Figure 9. Enabling software components required by the configured peripherals.*

## Configure Clocks

Peripheral configuration and clock configuration are connected. For example, enabling `CTIMER0` in the Peripherals tool also requires a valid clock source.

![CTIMER0 requires a clock configuration](../img/labp0/10_ctimer_requires_clock.png)

*Figure 10. CTIMER0 configuration showing that the peripheral needs a clock source before the generated setup is complete.*

Switch to the **Clocks** tool and enable the clock path required by the peripheral.

![Enabling the CTIMER0 clock](../img/labp0/11_enable_ctimer_clock.png)

*Figure 11. Enabling the clock used by CTIMER0 in the Clocks tool.*

Update the clock source selector when the clock tree requires a specific source for the peripheral.

![Selecting the clock source](../img/labp0/12_select_clock_source.png)

*Figure 12. Selecting the clock source used by the timer peripheral.*

Return to the **Peripherals** tool and select the configured clock for `CTIMER0`.

![Selecting the CTIMER0 clock in the Peripherals tool](../img/labp0/13_select_ctimer_clock.png)

*Figure 13. Selecting the clock source from the peripheral configuration after it has been enabled in the Clocks tool.*

Use the tool's automatic calculations for timer frequency and prescaler values when possible. This reduces hand-calculation mistakes and makes the intended frequency visible in the generated configuration.

![Automatically calculating timer prescaler settings](../img/labp0/14_auto_timer_prescaler.png)

*Figure 14. Letting Config Tools calculate the timer frequency and prescaler configuration.*

## Update the Project Code

When the configuration is complete, click **Update Code**. Review the generated file diff before applying it.

![Reviewing generated code before applying updates](../img/labp0/15_update_code_review.png)

*Figure 15. Reviewing the code update. Apply the update only after confirming that the generated files target the expected project folders.*

After applying the update, rebuild the project from VS Code or from the terminal. If the generated files changed pins, clocks, or peripheral init code, verify that the application source still calls the generated initialization functions in the expected order.

## Add Application Source Files

Generated files under the board folder should stay generated. Put application-owned helpers in a separate project folder, for example `app/`, and keep the root application file focused on startup and the main application flow.

A typical layout is:

```text
src/lab_new_project/configured/
├── led_blinky.c
├── app/
│   ├── uart.c
│   └── uart.h
├── frdmmcxa153/
│   ├── app/
│   └── board/
└── CMakeLists.txt
```

Header files such as `app/uart.h` can be included from the root application file:

```c
#include "app/uart.h"
```

C source files must also be added to the CMake target. To include all current and future `.c` files placed directly in `app/`, collect them before `mcux_add_source()` and pass the list as project sources:

```cmake
include(${board}/board_files.cmake)

file(GLOB app_sources CONFIGURE_DEPENDS RELATIVE ${CMAKE_CURRENT_LIST_DIR} app/*.c)

mcux_add_source(
    SOURCES led_blinky.c
            ${app_sources}
)
```

`CONFIGURE_DEPENDS` tells CMake/Ninja to re-check the `app/` folder when the project is built. `RELATIVE ${CMAKE_CURRENT_LIST_DIR}` is important for MCUXpresso SDK projects because `mcux_add_source()` expects project-relative paths. Without `RELATIVE`, absolute paths can be combined incorrectly with the project root.

After changing `CMakeLists.txt`, rerun CMake or build the project so the generated build files are refreshed. If a new `.c` file is added to `app/` later, rebuild once to let CMake discover it.

## Add New Components Later

You can return to Config Tools at any time to add components or peripherals. Open the same project `.mex` file, add the component, configure pins and clocks as needed, then update code again.

![Adding new components or peripherals](../img/labp0/16_add_components.png)

*Figure 16. Adding additional components from Config Tools after the project already exists.*

## SDK Driver Components and `prj.conf`

MCUXpresso Config Tools can configure a peripheral instance, but the CMake project also needs the matching SDK driver component enabled. In this project, those component selections live in the board-specific file:

```text
src/lab_new_project/configured/frdmmcxa153/prj.conf
```

The root `prj.conf` still exists because the MCUX SDK Kconfig flow expects an application-level `prj.conf` in the project root. Treat that root file as a small shim. Put board/project driver selections in `frdmmcxa153/prj.conf`, because the project `CMakeLists.txt` explicitly adds `${board}/prj.conf` to `CONF_FILE`.

When Config Tools shows a message such as `CTIMER Driver is not found in the toolchain/IDE project`, it means the peripheral configuration exists, but the corresponding SDK driver is not selected in the project. In practice, this sometimes has to be fixed manually in `frdmmcxa153/prj.conf`.

For the common lab template, `frdmmcxa153/prj.conf` should look like this. Expand it only when you need to compare the file contents.

<details class="prompt-code" markdown="1">
<summary><strong>Show `frdmmcxa153/prj.conf`</strong></summary>

```conf
CONFIG_MCUX_PRJSEG_module.board.suite=y
CONFIG_MCUX_PRJSEG_module.board.use_board_clock=y
CONFIG_MCUX_COMPONENT_driver.clock=y
CONFIG_MCUX_COMPONENT_driver.inputmux_connections=y
CONFIG_MCUX_COMPONENT_driver.reset=y
CONFIG_MCUX_COMPONENT_device.CMSIS=y
CONFIG_MCUX_COMPONENT_device.system=y
CONFIG_MCUX_COMPONENT_device.startup=y
CONFIG_MCUX_COMPONENT_driver.common=y
CONFIG_MCUX_COMPONENT_driver.ctimer=y
CONFIG_MCUX_COMPONENT_driver.gpio=y
CONFIG_MCUX_COMPONENT_driver.lpadc=y
CONFIG_MCUX_COMPONENT_driver.lpi2c=y
CONFIG_MCUX_COMPONENT_driver.lpuart=y
CONFIG_MCUX_COMPONENT_driver.lpspi=y
CONFIG_MCUX_COMPONENT_driver.mcx_spc=y
CONFIG_MCUX_COMPONENT_driver.port=y
CONFIG_MCUX_COMPONENT_driver.pwm=y
CONFIG_MCUX_COMPONENT_utility.assert_lite=y
CONFIG_MCUX_COMPONENT_utilities.misc_utilities=y
CONFIG_MCUX_COMPONENT_utility.str=y
CONFIG_MCUX_COMPONENT_utility.debug_console_lite=y
CONFIG_MCUX_COMPONENT_component.lpuart_adapter=y
CONFIG_MCUX_PRJSEG_module.board.boardfile=n
CONFIG_MCUX_PRJSEG_module.board.console_lite=n
CONFIG_MCUX_PRJSEG_project.hw_app_project_folder=n
CONFIG_MCUX_PRJSEG_project.hw_app_customize_folder=y
CONFIG_MCUX_PRJSEG_module.board.clock=n
CONFIG_MCUX_PRJSEG_module.board.clock_customize_folder=y
CONFIG_MCUX_PRJSEG_module.board.pinmux_project_folder=n
CONFIG_MCUX_PRJSEG_module.board.pinmux_customize_folder=y
```

</details>

After editing `prj.conf`, rerun CMake so the generated build configuration and Config Tools metadata see the same components:

```powershell
cmake --preset debug
```

Then reload the `.mex` file or reopen MCUXpresso Config Tools. If the warning remains, check that `cfg_tools/cfg_require.json` and `cfg_tools/project_info.json` point to `frdmmcxa153/prj.conf`, not the root shim.

## Troubleshooting

If Config Tools reports SDK, component, or repository errors, check the SDK setup before changing application code. Typical causes are an SDK that is not fully imported, missing MCUXpresso tool dependencies, or a project opened from the wrong folder.

![SDK configuration error in Config Tools](../img/labp0/17_sdk_config_error.png)

*Figure 17. SDK or configuration errors usually point to a toolchain or SDK setup problem rather than a C source problem.*

![Missing SDK components error](../img/labp0/18_sdk_components_error.png)

*Figure 18. Missing component errors can appear when the SDK installation or Config Tools component database is incomplete.*

Before regenerating code again, confirm:

1. The MCUXpresso SDK for `FRDM-MCXA153` is imported and visible in the extension.
2. The project was opened from the folder that contains the active `.mex` file.
3. The generated code preview writes to the files compiled by CMake.
4. Required SDK driver components are enabled in `frdmmcxa153/prj.conf`; edit this file manually when Config Tools does not add the component.
5. Required peripheral clocks are enabled in the Clocks view.

## Checklist

1. Open the project `.mex` file from the active project folder.
2. Assign pins and configure pin characteristics in the Pins tool.
3. Enable peripherals and interrupts in the Peripherals tool, then confirm matching SDK driver components in `frdmmcxa153/prj.conf`.
4. Enable required clocks in the Clocks tool.
5. Return to Peripherals when a peripheral needs the newly configured clock.
6. Click **Update Code**, review the diff, and apply the update.
7. Rebuild the project and test the behavior on the board.
