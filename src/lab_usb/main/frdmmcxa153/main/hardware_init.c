/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "mouse.h"
#include "board.h"
/*${header:end}*/

extern usb_hid_mouse_struct_t g_UsbDeviceHidMouse;

/*${function:start}*/
void BOARD_InitHardware(void)
{
    RESET_PeripheralReset(kLPUART0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kPORT0_RST_SHIFT_RSTn);

    CLOCK_SetClockDiv(kCLOCK_DivLPUART0, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

#if (defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U))
void USB0_DriverIRQHandler(void)
{
    USB_DeviceKhciIsrFunction(g_UsbDeviceHidMouse.deviceHandle);
}

void USB0_IRQHandler(void)
{
    USB0_DriverIRQHandler();
}
#endif

void USB_DeviceClockInit(void)
{
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    RESET_PeripheralReset(kUSB0_RST_SHIFT_RSTn);
    CLOCK_EnableUsbfsClock();
#endif
}

void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber = 0U;

#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    uint8_t usbDeviceKhciIrq[] = USBFS_IRQS;
    irqNumber                  = usbDeviceKhciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];
#endif

    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ((IRQn_Type)irqNumber);
}

#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    USB_DeviceKhciTaskFunction(deviceHandle);
#else
    (void)deviceHandle;
#endif
}
#endif
/*${function:end}*/
