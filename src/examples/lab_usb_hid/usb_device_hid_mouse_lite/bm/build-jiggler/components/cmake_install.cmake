# Install script for directory: C:/WORKSPACE/proiecte/ipcei-lab/src/sdks/mcuxsdk/components

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/dev_hid_mouse_lite_bm")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Users/alex predescu/.mcuxpressotools/arm-gnu-toolchain-14.2.rel1-mingw-w64-x86_64-arm-none-eabi/bin/arm-none-eabi-objdump.exe")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/assert/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/audio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/aws_iot/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/button/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/common_task/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/coremark/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/crc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/debug_console/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/debug_console_lite/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/display/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/exception_handling/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/gpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/i2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/internal_flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/led/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/lists/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/log/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/mem_manager/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/messaging/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/misc_utilities/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/osa/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/panic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/pmic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/power_manager/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/pwm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/reset/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/reset1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/rng/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/rpmsg/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/rtc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/sensor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/serial_manager/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/shell/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/spi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/str/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/timer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/timer_manager/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/time_stamp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/touch/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/uart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/video/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/phy/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/IS42SM16800H/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/adc_sensor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/conn_fwloader/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/ele_crypto/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/ele_hseb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/i3c_bus/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/imx_sm_crc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/imu_adapter/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/mt48lc2m32b2/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/mt48lc4m16a2/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/mx25_flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/mx25l_flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/mx25r_flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/rtt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/scmi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/sdu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/slcd_engine/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/smt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/srtm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/sx1502/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/wifi_bt_module/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/smbus/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/debug_console_rtt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/notifier/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/ele_base_api/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/format/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/mpi_loader/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/pinctrl/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/clock/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/silicon_id/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/sm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/systick_timer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/timer_lptmr/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/unity/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/power/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/codec/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/expander/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/edgefast_wifi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/eeprom_emulation/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/storage/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/debug/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/gen_hal/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/lce/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/cmsis_drivers/cmsis_dspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/cmsis_drivers/cmsis_ecspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/cmsis_drivers/cmsis_enet/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/cmsis_drivers/cmsis_flexcomm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/cmsis_drivers/cmsis_gpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/cmsis_drivers/cmsis_i2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/cmsis_drivers/cmsis_ii2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/cmsis_drivers/cmsis_iuart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/cmsis_drivers/cmsis_lpc_gpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/cmsis_drivers/cmsis_lpc_i2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/cmsis_drivers/cmsis_lpc_vspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/cmsis_drivers/cmsis_lpc_vusart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/cmsis_drivers/cmsis_lpi2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/cmsis_drivers/cmsis_lpsci/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/cmsis_drivers/cmsis_lpspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/cmsis_drivers/cmsis_lpuart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/cmsis_drivers/cmsis_spi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/examples/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/cmsis_drivers/cmsis_uart/cmake_install.cmake")
endif()

