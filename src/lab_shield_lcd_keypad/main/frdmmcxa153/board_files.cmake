
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_configuration(
    CC "-DSDK_DEBUGCONSOLE=1"
    CX "-DSDK_DEBUGCONSOLE=1"
)


mcux_add_source(
    SOURCES frdmmcxa153/board.c
            frdmmcxa153/board.h
)

mcux_add_include(
    INCLUDES frdmmcxa153
)

mcux_add_source(
    SOURCES ../cfg_tools/board/clock_config.c
            ../cfg_tools/board/clock_config.h
)

mcux_add_include(
    INCLUDES ../cfg_tools/board
)

mcux_add_source(
    SOURCES ../cfg_tools/board/pin_mux.c
            ../cfg_tools/board/pin_mux.h
)

mcux_add_include(
    INCLUDES ../cfg_tools/board
)

mcux_add_source(
    SOURCES ../cfg_tools/board/peripherals.c
            ../cfg_tools/board/peripherals.h
            ../cfg_tools/board/RTE_Device.h
)

mcux_add_include(
    INCLUDES ../cfg_tools/board
)

mcux_add_source(
    SOURCES main/app.h
            main/hardware_init.c
)

mcux_add_include(
    INCLUDES main
)
