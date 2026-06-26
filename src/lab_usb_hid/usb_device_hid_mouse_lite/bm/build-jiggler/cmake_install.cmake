# Install script for directory: C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm

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
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/cmake/extension/flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/cmake/extension/usage/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/cmake/extension/reports/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/devices/MCX/MCXA/MCXA153/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/examples/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/acmp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/acmp_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/adc12/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/adc16/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/adc_12b1msps_sar/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/adc_5hc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/adc_5v12b_ll18_015/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/adc_etc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/aes/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/afe/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/aipstz/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/anactrl/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/aoi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/aon_lpadc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/asmc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/asrc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/audmix/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/bbnsm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/bctu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/bee/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/biss/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/caam/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cadc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/camera_csr/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/capt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/casper/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cau3/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/ccm32k/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cdog/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/ce/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/ci_pi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cic_irb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cmc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cmp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cmp_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cmt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cmu_fc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cmu_fm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cns_acomp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cns_adc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cns_dac/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/common/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cop/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/crc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/csi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/ctimer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dac/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dac12/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dac14/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dac32/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dac_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dcdc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dcdc_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dcic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dcif/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dcif_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dcp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dma/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dma3/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dmamux/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dmic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dpr/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dpu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dpu_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dpu_irqsteer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dryice/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_adc16/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_aoi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_cadc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_cmp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_cop/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_crc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_crc16/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_dac/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_dma/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_dmamux/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_edma/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_eqdc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_evtg/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_ewm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_flexcan/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_gpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_i2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_lpi2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_mcm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_mscan/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_opamp_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_pdb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_pit/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_pmc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_port/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_pwm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_qdc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_qtmr/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_rgpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_sim/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_xbara/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dsc_xbarb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/dspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/easrc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/ecat/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/ecspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/edma/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/edma4/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/edma_rev2/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/eeprom/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/eim/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/ela_csec/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/elcdif/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/elemu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/emc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/emios/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/enc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/endat2p2/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/endat3/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/enet/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/enet_qos/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/epdc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/epit/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/eqdc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/erm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/esai/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/espi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/evtg/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/ewm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flash_c40/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flash_ftmr/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flash_k4/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flash_k4_iap/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flashiap/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexbus/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexcan/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexcomm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexram/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexspi_flr/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/fmc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/fmeas/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/fract_pll/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/ftm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/gdet/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/gdma/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/gint/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/glikey/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/gpc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/gpc_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/gpc_2/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/gpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/gpio_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/gpt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/hashcrypt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/hiperface/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/hsadc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/hscmp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/i2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/i3c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/iap/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/iap1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/iap3/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/iee/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/iee_apc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/ieer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/igf/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/igpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/ii2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/imu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/inputmux/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/intc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/intm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/intmux/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/iped/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/ipwm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/irq/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/irqsteer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/irqsteer_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/irtc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/isi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/itrc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/itrc_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/iuart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/jn_cmp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/jn_flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/jn_iocon/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/jn_pwm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/jn_rng/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/jpegdec/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/kbi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/key_manager/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/kpp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lcdic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lcdif/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lcdifv2/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lcdifv3/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lcu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/ldb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/ldb_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/ldb_combo_phy/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lin/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/llwu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lmem/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpacmp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpadc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_acomp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_adc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_crc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_dac/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_dma/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_enet/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_freqme/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_gpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_gpio_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_i2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_i2c_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_iocon/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_iocon_lite/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_iopctl/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_lcdc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_minispi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_miniusart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_rit/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_rtc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_rtc_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_spi_ssp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_vspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpc_vusart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpcmp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpflexcomm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpi2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpit/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpsci/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lptmr/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpuart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/ltc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mailbox/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mau/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mc_rgm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mcan/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mcm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mcx_cmc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mcx_enet/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mcx_spc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mcx_vbat/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mecc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mipi_csi2rx/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mipi_csi2rx_dwc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mipi_csi2rx_dwc_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mipi_dsi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mipi_dsi2_dwc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mipi_dsi_imx/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mipi_dsi_split/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mipi_dsi_split_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mmau/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mmdc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mmdvsq/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mmu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mpu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mrt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mscan/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mscm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/msgintr/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/msmc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/mu1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/netc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/nfc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/npx/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/ocotp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/opamp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/opamp_fast/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/ostimer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/otfad/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/otp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pdb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pdcon/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pdm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pint/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pit/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pls_pmu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/plu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pmc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pmc0/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pmu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pngdec/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/port/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/powerquad/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/prg/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/prince/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/puf/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/puf_v3/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pwm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pwt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pwt_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pxp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/qdc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/qn_acmp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/qn_adc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/qn_bod/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/qn_dac/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/qn_flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/qn_fsp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/qn_gpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/qn_inputmux/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/qn_iocon/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/qn_rng/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/qn_rtc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/qn_syscon/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/qn_wdt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/qsci/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/qspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/qtmr_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/qtmr_2/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/queued_spi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/rcm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/rdc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/rdc_sema42/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/reformatter/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/rgpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/rng/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/rng_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/rnga/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/rtc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/rtc_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/rtc_analog/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/rtc_jdp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/rtd_cmc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/rtwdog/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/s3mu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/sai/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/sar_adc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/sctimer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/sdadc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/sdhc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/sdif/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/sdma/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/sdramc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/sdu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/sema4/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/sema42/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/semc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/sfa/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/sha/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/sim/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/sinc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/slcd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/slcd_split/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/smartcard/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/smartdma/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/smc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/smm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/smscm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/snvs_hp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/snvs_lp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/software_i2s/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/spc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/spdif/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/spdif_xcvr/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/spi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/spifi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/spm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/sramc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/sramc_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/sramctl/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/src/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/ssarc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/stm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/swm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/swt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/syscon/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/sysctl/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/sysctr/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/sysmpu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/syspm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/tdet/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/tempmon/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/tempsense/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/tempsensor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/tempsensor_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/tenbaset_phy/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/tmu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/tmu_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/tmu_2/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/tmu_3/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/tpm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/trdc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/trdc_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/trgmux/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/trgsync/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/trng/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/tsc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/tsens/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/tspc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/tstmr/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/uart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/usdhc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/utick/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/vbat/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/virt_wrapper/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/vref/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/vref_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/waketimer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/wdog/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/wdog01/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/wdog32/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/wdog8/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/wdt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/wkpu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/wkt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/wuu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/wwdt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/xbar/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/xbar_1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/xbara/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/xbarb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/xbic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/xecc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/xrdc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/xrdc2/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/xspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cache/armv7-m7/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cache/armv8-a/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cache/cache64/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cache/llc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cache/lmem/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cache/lpcac/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cache/lpcac_n4a_mcxn/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cache/lplmem/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/cache/xcache/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexcomm/i2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexcomm/i2s/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexcomm/spi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexcomm/usart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexio/a-format/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexio/biss/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexio/camera/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexio/endat2p2/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexio/i2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexio/i2s/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexio/mculcd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexio/qspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexio/spi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexio/t-format/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexio/uart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/flexspi/flexspi_dma3/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpflexcomm/lpi2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpflexcomm/lpspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/lpflexcomm/lpuart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pn76/crc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pn76/crc_wrapper/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pn76/gpadc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pn76/gpdma/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pn76/gpt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pn76/i2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pn76/lpuart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pn76/pn_wrapper/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pn76/rng/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pn76/spi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pn76/wdt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/tsi/tsi_v2/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/tsi/tsi_v4/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/tsi/tsi_v5/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/tsi/tsi_v6/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pn76/hostif/pnev7642fama/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/drivers/pn76/hostif/pnev76fama/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/components/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/rtos/freertos/freertos-kernel/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/middleware/usb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/middleware/rtcesl/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/middleware/motor_control/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/middleware/mcuboot_opensource/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/middleware/freemaster/mcux/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/middleware/safety_iec60730b/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/middleware/cadence/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/middleware/se_hostlib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/middleware/canopennode/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_COMPONENT MATCHES "^[a-zA-Z0-9_.+-]+$")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
  else()
    string(MD5 CMAKE_INST_COMP_HASH "${CMAKE_INSTALL_COMPONENT}")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INST_COMP_HASH}.txt")
    unset(CMAKE_INST_COMP_HASH)
  endif()
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
  file(WRITE "C:/WORKSPACE/proiecte/ipcei-lab/src/lab_usb_hid/usb_device_hid_mouse_lite/bm/build-jiggler/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
