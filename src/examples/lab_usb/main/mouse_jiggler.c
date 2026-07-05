#include "mouse_jiggler.h"

#define MOUSE_JIGGLER_INTERVAL_MS (30000U)

typedef struct _mouse_jiggler_state
{
    mouse_jiggler_send_report_t sendReport;
    volatile uint32_t tickMs;
    uint32_t lastMoveMs;
    uint8_t patternIndex;
    bool configured;
    bool transferReady;
    bool enabled;
} mouse_jiggler_state_t;

static mouse_jiggler_state_t s_MouseJiggler;

void MouseJiggler_Init(mouse_jiggler_send_report_t sendReport)
{
    s_MouseJiggler.sendReport     = sendReport;
    s_MouseJiggler.tickMs         = 0U;
    s_MouseJiggler.lastMoveMs     = 0U;
    s_MouseJiggler.patternIndex   = 0U;
    s_MouseJiggler.configured     = false;
    s_MouseJiggler.transferReady  = true;
    s_MouseJiggler.enabled        = true;
}

void MouseJiggler_SetUsbConfigured(bool configured)
{
    s_MouseJiggler.configured    = configured;
    s_MouseJiggler.transferReady = configured;
    s_MouseJiggler.lastMoveMs    = s_MouseJiggler.tickMs;
}

void MouseJiggler_SetTransferComplete(void)
{
    s_MouseJiggler.transferReady = true;
}

void MouseJiggler_Tick(void)
{
    s_MouseJiggler.tickMs++;
}

void MouseJiggler_Task(void)
{
    static const int8_t movementPattern[][2] = {
        {1, 0},
        {-1, 0},
        {0, 1},
        {0, -1},
    };
    uint8_t report[MOUSE_JIGGLER_REPORT_LENGTH];
    uint32_t nowMs;

    if ((!s_MouseJiggler.enabled) || (!s_MouseJiggler.configured) || (!s_MouseJiggler.transferReady) ||
        (NULL == s_MouseJiggler.sendReport))
    {
        return;
    }

    nowMs = s_MouseJiggler.tickMs;
    if ((uint32_t)(nowMs - s_MouseJiggler.lastMoveMs) < MOUSE_JIGGLER_INTERVAL_MS)
    {
        return;
    }

    report[0] = 0U;
    report[1] = (uint8_t)movementPattern[s_MouseJiggler.patternIndex][0];
    report[2] = (uint8_t)movementPattern[s_MouseJiggler.patternIndex][1];
    report[3] = 0U;

    s_MouseJiggler.transferReady = false;
    if (kStatus_USB_Success == s_MouseJiggler.sendReport(report))
    {
        s_MouseJiggler.lastMoveMs = nowMs;
        s_MouseJiggler.patternIndex++;
        if (s_MouseJiggler.patternIndex >= (sizeof(movementPattern) / sizeof(movementPattern[0])))
        {
            s_MouseJiggler.patternIndex = 0U;
        }
    }
    else
    {
        s_MouseJiggler.transferReady = true;
    }
}
