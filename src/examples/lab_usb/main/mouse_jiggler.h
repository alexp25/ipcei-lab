#ifndef MOUSE_JIGGLER_H_
#define MOUSE_JIGGLER_H_

#include <stdbool.h>
#include <stdint.h>

#include "usb.h"

#define MOUSE_JIGGLER_REPORT_LENGTH (4U)

typedef usb_status_t (*mouse_jiggler_send_report_t)(const uint8_t report[MOUSE_JIGGLER_REPORT_LENGTH]);

void MouseJiggler_Init(mouse_jiggler_send_report_t sendReport);
void MouseJiggler_SetUsbConfigured(bool configured);
void MouseJiggler_SetTransferComplete(void);
void MouseJiggler_Task(void);
void MouseJiggler_Tick(void);

#endif /* MOUSE_JIGGLER_H_ */
