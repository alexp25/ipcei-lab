
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

cmake_path(APPEND conf_file_path ${CMAKE_CURRENT_LIST_DIR} ${board} prj.conf)
set(CONF_FILE ${conf_file_path} ${CONF_FILE})
include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(main LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/main)


include(${board}/board_files.cmake)
include(${SdkRootDirPath}/CMakeLists.txt)


mcux_add_source(
    SOURCES main.c
            mouse_jiggler.c
            mouse_jiggler.h
            usb_device_ch9.c
            usb_device_ch9.h
            usb_device_descriptor.c
            usb_device_descriptor.h
            usb_device_hid.c
            usb_device_hid.h
            mouse.h
)

mcux_add_include(
    INCLUDES frdmmcxa153/board
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
mcux_add_config_mex_path( PATH . )
mcux_add_include(INCLUDES main .)

mcux_add_macro(
    CC "-DUSB_STACK_BM"
)
