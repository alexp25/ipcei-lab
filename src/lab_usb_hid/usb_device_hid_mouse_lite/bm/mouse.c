/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017,2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_hid.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"
#include "mouse.h"

#include "board.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "fsl_port.h"

#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif

#define MOUSE_JIGGLER_STEP (1)
#define MOUSE_JIGGLER_REPORTS_PER_SIDE (40U)
#define MOUSE_JIGGLER_SW_GPIO BOARD_SW3_GPIO
#define MOUSE_JIGGLER_SW_PORT PORT1
#define MOUSE_JIGGLER_SW_PIN BOARD_SW3_GPIO_PIN
#define MOUSE_JIGGLER_SW_PRESSED (0U)
#define MOUSE_JIGGLER_DEBOUNCE_POLLS (3000U)
#define MOUSE_JIGGLER_LED_GPIO BOARD_LED_RED_GPIO
#define MOUSE_JIGGLER_LED_PORT PORT3
#define MOUSE_JIGGLER_LED_PIN BOARD_LED_RED_GPIO_PIN

void BOARD_InitHardware(void);
void USB_DeviceClockInit(void);
void USB_DeviceIsrEnable(void);
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle);
#endif

static usb_status_t USB_DeviceHidMouseSendReport(void);
static usb_status_t USB_DeviceHidInterruptIn(usb_device_handle deviceHandle,
                                             usb_device_endpoint_callback_message_struct_t *event,
                                             void *arg);
static void USB_DeviceApplicationInit(void);
static void MouseJiggler_InitSwitch(void);
static void MouseJiggler_PollSwitch(void);
static void MouseJiggler_InitStatusLed(void);
static void MouseJiggler_SetEnabled(uint8_t enabled);

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_SetupOutBuffer[8];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_MouseBuffer[USB_HID_MOUSE_REPORT_LENGTH];
usb_hid_mouse_struct_t g_UsbDeviceHidMouse;
static volatile uint8_t s_JiggleEnabled = 1U;

static usb_status_t USB_DeviceHidMouseSendReport(void)
{
    static uint8_t reportCount = 0U;
    static int8_t xStep       = MOUSE_JIGGLER_STEP;

    g_UsbDeviceHidMouse.buffer[0] = 0U;
    g_UsbDeviceHidMouse.buffer[1] = (0U != s_JiggleEnabled) ? (uint8_t)xStep : 0U;
    g_UsbDeviceHidMouse.buffer[2] = 0U;
    g_UsbDeviceHidMouse.buffer[3] = 0U;

    if (++reportCount >= MOUSE_JIGGLER_REPORTS_PER_SIDE)
    {
        reportCount = 0U;
        xStep       = (int8_t)-xStep;
    }

    return USB_DeviceSendRequest(g_UsbDeviceHidMouse.deviceHandle, USB_HID_MOUSE_ENDPOINT_IN,
                                 g_UsbDeviceHidMouse.buffer, USB_HID_MOUSE_REPORT_LENGTH);
}

static usb_status_t USB_DeviceHidInterruptIn(usb_device_handle deviceHandle,
                                             usb_device_endpoint_callback_message_struct_t *event,
                                             void *arg)
{
    if (0U == g_UsbDeviceHidMouse.attach)
    {
        return kStatus_USB_Error;
    }

    if ((NULL != event) && (event->length == USB_CANCELLED_TRANSFER_LENGTH))
    {
        return kStatus_USB_Error;
    }

    return USB_DeviceHidMouseSendReport();
}

usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    uint8_t *temp8     = (uint8_t *)param;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
            USB_DeviceControlPipeInit(g_UsbDeviceHidMouse.deviceHandle);
            g_UsbDeviceHidMouse.attach = 0U;
            error                      = kStatus_USB_Success;
            break;

        case kUSB_DeviceEventSetConfiguration:
            if ((NULL != temp8) && (USB_HID_MOUSE_CONFIGURE_INDEX == *temp8))
            {
                usb_device_endpoint_init_struct_t epInitStruct;
                usb_device_endpoint_callback_struct_t epCallback;

                epCallback.callbackFn    = USB_DeviceHidInterruptIn;
                epCallback.callbackParam = handle;

                epInitStruct.zlt             = 0U;
                epInitStruct.transferType    = USB_ENDPOINT_INTERRUPT;
                epInitStruct.endpointAddress = USB_HID_MOUSE_ENDPOINT_IN |
                                               (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
                epInitStruct.maxPacketSize   = FS_HID_MOUSE_INTERRUPT_IN_PACKET_SIZE;
                epInitStruct.interval        = FS_HID_MOUSE_INTERRUPT_IN_INTERVAL;

                USB_DeviceInitEndpoint(g_UsbDeviceHidMouse.deviceHandle, &epInitStruct, &epCallback);

                g_UsbDeviceHidMouse.attach = 1U;
                error                      = USB_DeviceHidMouseSendReport();
            }
            break;

        case kUSB_DeviceEventSetInterface:
            error = kStatus_USB_Success;
            break;

        default:
            break;
    }

    return error;
}

usb_status_t USB_DeviceGetSetupBuffer(usb_device_handle handle, usb_setup_struct_t **setupBuffer)
{
    static uint32_t hidMouseSetup[2];

    if (NULL == setupBuffer)
    {
        return kStatus_USB_InvalidParameter;
    }

    *setupBuffer = (usb_setup_struct_t *)&hidMouseSetup;
    return kStatus_USB_Success;
}

usb_status_t USB_DeviceConfigureRemoteWakeup(usb_device_handle handle, uint8_t enable)
{
    return kStatus_USB_InvalidRequest;
}

usb_status_t USB_DeviceConfigureEndpointStatus(usb_device_handle handle, uint8_t ep, uint8_t status)
{
    if ((USB_HID_MOUSE_ENDPOINT_IN != (ep & USB_ENDPOINT_NUMBER_MASK)) ||
        (0U == (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK)))
    {
        return kStatus_USB_InvalidRequest;
    }

    if (0U != status)
    {
        return USB_DeviceStallEndpoint(handle, ep);
    }

    return USB_DeviceUnstallEndpoint(handle, ep);
}

usb_status_t USB_DeviceGetClassReceiveBuffer(usb_device_handle handle,
                                             usb_setup_struct_t *setup,
                                             uint32_t *length,
                                             uint8_t **buffer)
{
    if ((NULL == length) || (NULL == buffer) || (*length > sizeof(s_SetupOutBuffer)))
    {
        return kStatus_USB_InvalidRequest;
    }

    *buffer = s_SetupOutBuffer;
    return kStatus_USB_Success;
}

usb_status_t USB_DeviceProcessClassRequest(usb_device_handle handle,
                                           usb_setup_struct_t *setup,
                                           uint32_t *length,
                                           uint8_t **buffer)
{
    return kStatus_USB_InvalidRequest;
}

static void MouseJiggler_InitSwitch(void)
{
    const gpio_pin_config_t swGpioConfig = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic  = 0U,
    };
    const port_pin_config_t swPortConfig = {
        .pullSelect          = kPORT_PullDisable,
        .pullValueSelect     = kPORT_LowPullResistor,
        .slewRate            = kPORT_FastSlewRate,
        .passiveFilterEnable = kPORT_PassiveFilterDisable,
        .openDrainEnable     = kPORT_OpenDrainDisable,
        .driveStrength       = kPORT_LowDriveStrength,
        .driveStrength1      = kPORT_NormalDriveStrength,
        .mux                 = kPORT_MuxAsGpio,
        .inputBuffer         = kPORT_InputBufferEnable,
        .invertInput         = kPORT_InputNormal,
        .lockRegister        = kPORT_UnlockRegister,
    };

    CLOCK_EnableClock(kCLOCK_GateGPIO1);
    CLOCK_EnableClock(kCLOCK_GatePORT1);
    RESET_ReleasePeripheralReset(kGPIO1_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kPORT1_RST_SHIFT_RSTn);

    GPIO_PinInit(MOUSE_JIGGLER_SW_GPIO, MOUSE_JIGGLER_SW_PIN, &swGpioConfig);
    PORT_SetPinConfig(MOUSE_JIGGLER_SW_PORT, MOUSE_JIGGLER_SW_PIN, &swPortConfig);
}

static void MouseJiggler_InitStatusLed(void)
{
    const gpio_pin_config_t ledGpioConfig = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = LOGIC_LED_OFF,
    };
    const port_pin_config_t ledPortConfig = {
        .pullSelect          = kPORT_PullDisable,
        .pullValueSelect     = kPORT_LowPullResistor,
        .slewRate            = kPORT_FastSlewRate,
        .passiveFilterEnable = kPORT_PassiveFilterDisable,
        .openDrainEnable     = kPORT_OpenDrainDisable,
        .driveStrength       = kPORT_LowDriveStrength,
        .driveStrength1      = kPORT_NormalDriveStrength,
        .mux                 = kPORT_MuxAsGpio,
        .inputBuffer         = kPORT_InputBufferEnable,
        .invertInput         = kPORT_InputNormal,
        .lockRegister        = kPORT_UnlockRegister,
    };

    CLOCK_EnableClock(kCLOCK_GateGPIO3);
    CLOCK_EnableClock(kCLOCK_GatePORT3);
    RESET_ReleasePeripheralReset(kGPIO3_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kPORT3_RST_SHIFT_RSTn);

    GPIO_PinInit(MOUSE_JIGGLER_LED_GPIO, MOUSE_JIGGLER_LED_PIN, &ledGpioConfig);
    PORT_SetPinConfig(MOUSE_JIGGLER_LED_PORT, MOUSE_JIGGLER_LED_PIN, &ledPortConfig);
}

static void MouseJiggler_SetEnabled(uint8_t enabled)
{
    s_JiggleEnabled = (0U != enabled) ? 1U : 0U;
    GPIO_PinWrite(MOUSE_JIGGLER_LED_GPIO, MOUSE_JIGGLER_LED_PIN,
                  (0U != s_JiggleEnabled) ? LOGIC_LED_ON : LOGIC_LED_OFF);
}

static void MouseJiggler_PollSwitch(void)
{
    static uint8_t lastRawState      = 1U;
    static uint8_t stableState       = 1U;
    static uint32_t stablePollCount  = 0U;
    uint8_t rawState                 = (uint8_t)GPIO_PinRead(MOUSE_JIGGLER_SW_GPIO, MOUSE_JIGGLER_SW_PIN);

    if (rawState == lastRawState)
    {
        if (stablePollCount < MOUSE_JIGGLER_DEBOUNCE_POLLS)
        {
            stablePollCount++;
        }
    }
    else
    {
        stablePollCount = 0U;
        lastRawState    = rawState;
    }

    if ((stablePollCount >= MOUSE_JIGGLER_DEBOUNCE_POLLS) && (rawState != stableState))
    {
        stableState = rawState;
        if (MOUSE_JIGGLER_SW_PRESSED == stableState)
        {
            MouseJiggler_SetEnabled((0U == s_JiggleEnabled) ? 1U : 0U);
            usb_echo("Mouse jiggler %s\r\n", (0U != s_JiggleEnabled) ? "enabled" : "disabled");
        }
    }
}

static void USB_DeviceApplicationInit(void)
{
    USB_DeviceClockInit();

#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
    SYSMPU_Enable(SYSMPU, 0);
#endif

    g_UsbDeviceHidMouse.deviceHandle = NULL;
    g_UsbDeviceHidMouse.buffer       = s_MouseBuffer;
    g_UsbDeviceHidMouse.attach       = 0U;
    MouseJiggler_InitStatusLed();
    MouseJiggler_SetEnabled(s_JiggleEnabled);
    MouseJiggler_InitSwitch();

    if (kStatus_USB_Success != USB_DeviceInit(CONTROLLER_ID, USB_DeviceCallback, &g_UsbDeviceHidMouse.deviceHandle))
    {
        usb_echo("USB mouse jiggler failed\r\n");
        return;
    }

    usb_echo("USB mouse jiggler\r\n");
    USB_DeviceIsrEnable();
    SDK_DelayAtLeastUs(5000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    USB_DeviceRun(g_UsbDeviceHidMouse.deviceHandle);
}

#if defined(__CC_ARM) || (defined(__ARMCC_VERSION)) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
    BOARD_InitHardware();
    USB_DeviceApplicationInit();

    while (1U)
    {
        MouseJiggler_PollSwitch();
#if USB_DEVICE_CONFIG_USE_TASK
        USB_DeviceTaskFn(g_UsbDeviceHidMouse.deviceHandle);
#endif
    }
}
