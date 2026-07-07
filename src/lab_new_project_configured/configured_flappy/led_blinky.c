/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "peripherals.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app/uart.h"

#include "app/board_ili9341.h"
#include "app/ili9341.h"
#include "fsl_debug_console.h"
#include "fsl_lpadc.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ADC_SW2_CHANNEL 14U
#define ADC_COMMAND_ID 1U
#define ADC_TRIGGER_ID 0U
#define ADC_PRINT_TICKS 100U
#define PWM_FADE_DUTY_MAX 100U
#define GAME_FRAME_DELAY_US 12000U
#define GAME_BIRD_X 48
#define GAME_BIRD_W 22
#define GAME_BIRD_H 18
#define GAME_BIRD_BEAK_W 5
#define GAME_PIPE_WIDTH 28
#define GAME_PIPE_GAP 98
#define GAME_GRAVITY_Q8 40
#define GAME_FLAP_VELOCITY_Q8 (-520)
#define GAME_FLAP_HOLD_ACCEL_Q8 (-42)
#define GAME_RISE_VELOCITY_LIMIT_Q8 (-980)
#define GAME_MAX_FALL_Q8 1150
#define GAME_PIPE_SPEED 3
#define GAME_SW2_PRESSED_THRESHOLD 1000U
#define GAME_BACKGROUND_COLOR ILI9341_COLOR_CYAN
#define GAME_GROUND_COLOR ILI9341_COLOR_GREEN
#define GAME_PIPE_COLOR ILI9341_COLOR_GREEN
#define GAME_BIRD_COLOR ILI9341_COLOR_YELLOW
#define GAME_TEXT_COLOR ILI9341_COLOR_WHITE
#define GAME_SCORE_W 72U
#define GAME_SCORE_H 24U
#define GAME_OVER_X 66U
#define GAME_OVER_Y 88U
#define GAME_OVER_W 132U
#define GAME_OVER_H 48U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void PWM0_LED_InitOutput(void);
static void PWM0_LED_UpdateFade(void);
static void SW2_ADC_InitInput(void);
static uint16_t SW2_ADC_ReadRaw(void);
static bool SW2_IsPressed(void);
static uint16_t Game_RandomGapY(void);
static void Game_FillRectClipped(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
static void Game_Reset(void);
static void Game_DrawPipeAt(int16_t pipeX, uint16_t gapY, uint16_t color);
static void Game_DrawPipeStripAt(int16_t pipeX, uint16_t gapY, int16_t stripX, uint16_t stripW, uint16_t color);
static void Game_ErasePipeTrail(int16_t oldPipeX, int16_t newPipeX, uint16_t oldGapY);
static void Game_DrawPipeLeadingEdge(int16_t oldPipeX, int16_t newPipeX, uint16_t gapY);
static void Game_DrawBirdAt(int16_t birdY, uint16_t color);
static void Game_DrawScore(void);
static void Game_DrawGameOverOverlay(void);
static void Game_DrawScene(void);
static bool Game_CheckCollision(void);
static void Game_Update(bool flapPressedEdge, bool flapHeld);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile uint16_t counterPrintTick = 0;
static volatile uint32_t printCounter = 0;
static volatile bool flag = false;
static volatile uint8_t pwmDutyCycle = 0;
static ili9341_t s_lcd;
static uint16_t s_lcdWidth;
static uint16_t s_lcdHeight;
static int32_t s_birdYQ8;
static int32_t s_birdVelocityQ8;
static int16_t s_pipeX;
static uint16_t s_pipeGapY;
static uint32_t s_score;
static uint32_t s_frame;
static bool s_gameOver;
static bool s_sw2WasPressed;
static int16_t s_prevBirdY;
static int16_t s_prevPipeX;
static uint16_t s_prevPipeGapY;
static uint32_t s_prevScore;
static bool s_prevGameOver;
static bool s_fullRedrawNeeded;

/*******************************************************************************
 * Code
 ******************************************************************************/

void SysTick_Handler(void)
{
    counterPrintTick++;
    if (counterPrintTick >= ADC_PRINT_TICKS)
    {
        counterPrintTick = 0;
        flag = true;
        GPIO_PortToggle(BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN);
    }

    PWM0_LED_UpdateFade();
}


static void PWM0_LED_InitOutput(void)
{
    PWM_SetupFaultDisableMap(FLEXPWM0_PERIPHERAL, FLEXPWM0_SM0, FLEXPWM0_SM0_A, kPWM_faultchannel_0, 0U);
    PWM_OutputEnable(FLEXPWM0_PERIPHERAL, FLEXPWM0_SM0_A, FLEXPWM0_SM0);
    PWM_SetPwmLdok(FLEXPWM0_PERIPHERAL, kPWM_Control_Module_0, true);
}

static void PWM0_LED_UpdateFade(void)
{
    PWM_UpdatePwmDutycycle(FLEXPWM0_PERIPHERAL, FLEXPWM0_SM0, FLEXPWM0_SM0_A, kPWM_SignedCenterAligned,
                           pwmDutyCycle);
    PWM_SetPwmLdok(FLEXPWM0_PERIPHERAL, kPWM_Control_Module_0, true);

    pwmDutyCycle++;
    if (pwmDutyCycle >= PWM_FADE_DUTY_MAX)
    {
        pwmDutyCycle = 0;
    }
}

static void SW2_ADC_InitInput(void)
{
    lpadc_conv_command_config_t commandConfig;
    lpadc_conv_trigger_config_t triggerConfig;

    LPADC_GetDefaultConvCommandConfig(&commandConfig);
    commandConfig.channelNumber = ADC_SW2_CHANNEL;
    LPADC_SetConvCommandConfig(ADC0_PERIPHERAL, ADC_COMMAND_ID, &commandConfig);

    LPADC_GetDefaultConvTriggerConfig(&triggerConfig);
    triggerConfig.targetCommandId = ADC_COMMAND_ID;
    triggerConfig.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(ADC0_PERIPHERAL, ADC_TRIGGER_ID, &triggerConfig);
}

static uint16_t SW2_ADC_ReadRaw(void)
{
    lpadc_conv_result_t result;
    uint32_t timeout = 10000U;

    LPADC_DoSoftwareTrigger(ADC0_PERIPHERAL, 1UL << ADC_TRIGGER_ID);
    while (!LPADC_GetConvResult(ADC0_PERIPHERAL, &result))
    {
        if (timeout-- == 0U)
        {
            return 4095U;
        }
    }

    return (uint16_t)result.convValue;
}

static bool SW2_IsPressed(void)
{
    return SW2_ADC_ReadRaw() < GAME_SW2_PRESSED_THRESHOLD;
}

static uint16_t Game_RandomGapY(void)
{
    const uint16_t minY = (GAME_PIPE_GAP / 2U) + 16U;
    const uint16_t maxY = (uint16_t)(s_lcdHeight - (GAME_PIPE_GAP / 2U) - 24U);
    const uint16_t range = (uint16_t)((maxY > minY) ? (maxY - minY) : 1U);
    const uint32_t seed = (s_frame * 37U) + (s_score * 71U) + 53U;

    return (uint16_t)(minY + (seed % range));
}

static void Game_FillRectClipped(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    int16_t x0 = x;
    int16_t y0 = y;
    int16_t x1 = (int16_t)(x + w);
    int16_t y1 = (int16_t)(y + h);

    if ((w <= 0) || (h <= 0) || (x1 <= 0) || (y1 <= 0) || (x0 >= (int16_t)s_lcdWidth) ||
        (y0 >= (int16_t)s_lcdHeight))
    {
        return;
    }

    if (x0 < 0)
    {
        x0 = 0;
    }
    if (y0 < 0)
    {
        y0 = 0;
    }
    if (x1 > (int16_t)s_lcdWidth)
    {
        x1 = (int16_t)s_lcdWidth;
    }
    if (y1 > (int16_t)s_lcdHeight)
    {
        y1 = (int16_t)s_lcdHeight;
    }

    if ((x1 > x0) && (y1 > y0))
    {
        (void)ili9341_fill_rect(&s_lcd, (uint16_t)x0, (uint16_t)y0, (uint16_t)(x1 - x0), (uint16_t)(y1 - y0), color);
    }
}

static void Game_Reset(void)
{
    s_birdYQ8 = (int32_t)(s_lcdHeight / 2U) << 8;
    s_birdVelocityQ8 = 0;
    s_pipeX = (int16_t)s_lcdWidth;
    s_pipeGapY = Game_RandomGapY();
    s_score = 0;
    s_frame = 0;
    s_gameOver = false;
    s_prevBirdY = (int16_t)(s_birdYQ8 >> 8);
    s_prevPipeX = s_pipeX;
    s_prevPipeGapY = s_pipeGapY;
    s_prevScore = s_score;
    s_prevGameOver = s_gameOver;
    s_fullRedrawNeeded = true;
    (void)ili9341_fill_screen(&s_lcd, GAME_BACKGROUND_COLOR);
}

static void Game_DrawPipeAt(int16_t pipeX, uint16_t gapY, uint16_t color)
{
    Game_DrawPipeStripAt(pipeX, gapY, pipeX, GAME_PIPE_WIDTH, color);
}

static void Game_DrawPipeStripAt(int16_t pipeX, uint16_t gapY, int16_t stripX, uint16_t stripW, uint16_t color)
{
    const int16_t pipeRight = (int16_t)(pipeX + GAME_PIPE_WIDTH);
    const int16_t stripRight = (int16_t)(stripX + stripW);
    int16_t clippedLeft = (int16_t)stripX;
    int16_t clippedRight = stripRight;
    uint16_t drawX;
    uint16_t drawW;
    uint16_t topPipeHeight;
    uint16_t lowerPipeY;

    if ((pipeRight <= 0) || (pipeX >= (int16_t)s_lcdWidth) || (stripW == 0U))
    {
        return;
    }

    if (clippedLeft < pipeX)
    {
        clippedLeft = pipeX;
    }
    if (clippedRight > pipeRight)
    {
        clippedRight = pipeRight;
    }
    if (clippedLeft < 0)
    {
        clippedLeft = 0;
    }
    if (clippedRight > (int16_t)s_lcdWidth)
    {
        clippedRight = (int16_t)s_lcdWidth;
    }
    if (clippedRight <= clippedLeft)
    {
        return;
    }

    drawX = (uint16_t)clippedLeft;
    drawW = (uint16_t)(clippedRight - clippedLeft);
    topPipeHeight = (uint16_t)(gapY - (GAME_PIPE_GAP / 2U));
    lowerPipeY = (uint16_t)(gapY + (GAME_PIPE_GAP / 2U));

    if ((drawW > 0U) && (topPipeHeight > 0U))
    {
        (void)ili9341_fill_rect(&s_lcd, drawX, 0U, drawW, topPipeHeight, color);
    }
    if ((drawW > 0U) && (lowerPipeY < s_lcdHeight))
    {
        (void)ili9341_fill_rect(&s_lcd, drawX, lowerPipeY, drawW, (uint16_t)(s_lcdHeight - lowerPipeY), color);
    }
}

static void Game_ErasePipeTrail(int16_t oldPipeX, int16_t newPipeX, uint16_t oldGapY)
{
    const int16_t delta = (int16_t)(oldPipeX - newPipeX);

    if ((delta > 0) && (delta <= GAME_PIPE_WIDTH))
    {
        Game_DrawPipeStripAt(oldPipeX,
                             oldGapY,
                             (int16_t)(oldPipeX + GAME_PIPE_WIDTH - delta),
                             (uint16_t)delta,
                             GAME_BACKGROUND_COLOR);
    }
    else
    {
        Game_DrawPipeAt(oldPipeX, oldGapY, GAME_BACKGROUND_COLOR);
    }
}

static void Game_DrawPipeLeadingEdge(int16_t oldPipeX, int16_t newPipeX, uint16_t gapY)
{
    const int16_t delta = (int16_t)(oldPipeX - newPipeX);

    if ((delta > 0) && (delta <= GAME_PIPE_WIDTH))
    {
        Game_DrawPipeStripAt(newPipeX, gapY, newPipeX, (uint16_t)delta, GAME_PIPE_COLOR);
    }
    else
    {
        Game_DrawPipeAt(newPipeX, gapY, GAME_PIPE_COLOR);
    }
}

static void Game_DrawBirdAt(int16_t birdY, uint16_t color)
{
    uint16_t drawY;
    uint16_t drawH;

    if (((birdY + GAME_BIRD_H) <= 0) || (birdY >= (int16_t)s_lcdHeight))
    {
        return;
    }

    drawY = (birdY < 0) ? 0U : (uint16_t)birdY;
    drawH = (uint16_t)((((birdY + GAME_BIRD_H) > (int16_t)s_lcdHeight) ? (int16_t)s_lcdHeight :
                                                                         (birdY + GAME_BIRD_H)) -
                       (int16_t)drawY);

    if (drawH > 0U)
    {
        (void)ili9341_fill_rect(&s_lcd, GAME_BIRD_X, drawY, (uint16_t)(GAME_BIRD_W + GAME_BIRD_BEAK_W), drawH, color);
        if (color == GAME_BIRD_COLOR)
        {
            Game_FillRectClipped(GAME_BIRD_X, (int16_t)(birdY + 3), GAME_BIRD_W, 12, GAME_BIRD_COLOR);
            Game_FillRectClipped((int16_t)(GAME_BIRD_X + 3), (int16_t)(birdY + 10), 9, 5, ILI9341_COLOR_ORANGE);
            Game_FillRectClipped((int16_t)(GAME_BIRD_X + GAME_BIRD_W),
                                 (int16_t)(birdY + 7),
                                 GAME_BIRD_BEAK_W,
                                 5,
                                 ILI9341_COLOR_ORANGE);
            Game_FillRectClipped((int16_t)(GAME_BIRD_X + 15), (int16_t)(birdY + 5), 5, 5, ILI9341_COLOR_WHITE);
            Game_FillRectClipped((int16_t)(GAME_BIRD_X + 18), (int16_t)(birdY + 7), 2, 2, ILI9341_COLOR_BLACK);
        }
    }
}

static void Game_DrawScore(void)
{
    char scoreText[16];

    (void)ili9341_fill_rect(&s_lcd, 0U, 0U, GAME_SCORE_W, GAME_SCORE_H, GAME_BACKGROUND_COLOR);
    (void)snprintf(scoreText, sizeof(scoreText), "%lu", (unsigned long)s_score);
    (void)ili9341_write_string(&s_lcd, 6U, 6U, scoreText, GAME_TEXT_COLOR, GAME_BACKGROUND_COLOR, 2U);
}

static void Game_DrawGameOverOverlay(void)
{
    (void)ili9341_fill_rect(&s_lcd, GAME_OVER_X, GAME_OVER_Y, GAME_OVER_W, GAME_OVER_H, GAME_BACKGROUND_COLOR);
    (void)ili9341_write_string(&s_lcd, 72U, 88U, "GAME OVER", ILI9341_COLOR_RED, GAME_BACKGROUND_COLOR, 2U);
    (void)ili9341_write_string(&s_lcd, 66U, 112U, "PRESS SW2", GAME_TEXT_COLOR, GAME_BACKGROUND_COLOR, 2U);
}

static void Game_DrawScene(void)
{
    const int16_t birdY = (int16_t)(s_birdYQ8 >> 8);

    if (s_fullRedrawNeeded)
    {
        (void)ili9341_fill_screen(&s_lcd, GAME_BACKGROUND_COLOR);
        Game_DrawPipeAt(s_pipeX, s_pipeGapY, GAME_PIPE_COLOR);
        Game_DrawBirdAt(birdY, GAME_BIRD_COLOR);
        Game_DrawScore();
        if (s_gameOver)
        {
            Game_DrawGameOverOverlay();
        }
        s_fullRedrawNeeded = false;
    }
    else
    {
        const bool pipeWrapped = (s_pipeX > s_prevPipeX) || (s_pipeGapY != s_prevPipeGapY);
        const bool scoreTouched = ((s_pipeX < (int16_t)GAME_SCORE_W) &&
                                   ((s_pipeX + GAME_PIPE_WIDTH) > 0));

        Game_DrawBirdAt(s_prevBirdY, GAME_BACKGROUND_COLOR);
        if (pipeWrapped)
        {
            Game_DrawPipeAt(s_prevPipeX, s_prevPipeGapY, GAME_BACKGROUND_COLOR);
            Game_DrawPipeAt(s_pipeX, s_pipeGapY, GAME_PIPE_COLOR);
        }
        else
        {
            Game_ErasePipeTrail(s_prevPipeX, s_pipeX, s_prevPipeGapY);
            Game_DrawPipeLeadingEdge(s_prevPipeX, s_pipeX, s_pipeGapY);
        }

        Game_DrawBirdAt(birdY, GAME_BIRD_COLOR);
        if ((s_score != s_prevScore) || scoreTouched || ((s_frame & 3U) == 0U))
        {
            Game_DrawScore();
        }
        if (s_gameOver && !s_prevGameOver)
        {
            Game_DrawGameOverOverlay();
        }
    }

    s_prevBirdY = birdY;
    s_prevPipeX = s_pipeX;
    s_prevPipeGapY = s_pipeGapY;
    s_prevScore = s_score;
    s_prevGameOver = s_gameOver;
}

static bool Game_CheckCollision(void)
{
    const int16_t birdY = (int16_t)(s_birdYQ8 >> 8);
    const int16_t birdTop = (int16_t)(birdY + 2);
    const int16_t birdBottom = (int16_t)(birdY + GAME_BIRD_H - 2);
    const int16_t birdLeft = (int16_t)(GAME_BIRD_X + 2);
    const int16_t birdRight = (int16_t)(GAME_BIRD_X + GAME_BIRD_W - 3);
    const int16_t gapTop = (int16_t)(s_pipeGapY - (GAME_PIPE_GAP / 2U));
    const int16_t gapBottom = (int16_t)(s_pipeGapY + (GAME_PIPE_GAP / 2U));

    if ((birdY < 0) || ((birdY + GAME_BIRD_H) >= (int16_t)s_lcdHeight))
    {
        return true;
    }

    if ((birdRight >= s_pipeX) && (birdLeft <= (s_pipeX + GAME_PIPE_WIDTH)))
    {
        return (birdTop < gapTop) || (birdBottom > gapBottom);
    }

    return false;
}

static void Game_Update(bool flapPressedEdge, bool flapHeld)
{
    if (s_gameOver)
    {
        if (flapPressedEdge)
        {
            Game_Reset();
        }
        return;
    }

    if (flapPressedEdge)
    {
        if (s_birdVelocityQ8 > GAME_FLAP_VELOCITY_Q8)
        {
            s_birdVelocityQ8 = GAME_FLAP_VELOCITY_Q8;
        }
    }
    if (flapHeld)
    {
        s_birdVelocityQ8 += GAME_FLAP_HOLD_ACCEL_Q8;
        if (s_birdVelocityQ8 < GAME_RISE_VELOCITY_LIMIT_Q8)
        {
            s_birdVelocityQ8 = GAME_RISE_VELOCITY_LIMIT_Q8;
        }
    }

    s_birdVelocityQ8 += GAME_GRAVITY_Q8;
    if (s_birdVelocityQ8 > GAME_MAX_FALL_Q8)
    {
        s_birdVelocityQ8 = GAME_MAX_FALL_Q8;
    }
    s_birdYQ8 += s_birdVelocityQ8;

    s_pipeX = (int16_t)(s_pipeX - GAME_PIPE_SPEED);
    if ((s_pipeX + GAME_PIPE_WIDTH) < 0)
    {
        s_pipeX = (int16_t)s_lcdWidth;
        s_pipeGapY = Game_RandomGapY();
        s_score++;
    }

    if (Game_CheckCollision())
    {
        s_gameOver = true;
    }

    s_frame++;
}


/*!
 * @brief Main function
 */
int main(void)
{
    ili9341_config_t lcdConfig;
    status_t status;

    /* Board pin init */
    BOARD_InitHardware();
    PWM0_LED_InitOutput();
    SW2_ADC_InitInput();

    BOARD_GetIli9341Config(&lcdConfig);
    status = ili9341_init(&s_lcd, &lcdConfig);
    if (status != kStatus_Success)
    {
        PRINTF("ILI9341 init failed: %ld\r\n", (long)status);
        while (1)
        {
        }
    }

    status = ili9341_set_rotation(&s_lcd, 1U);
    if (status != kStatus_Success)
    {
        PRINTF("ILI9341 rotation failed: %ld\r\n", (long)status);
        while (1)
        {
        }
    }

    s_lcdWidth = s_lcd.width;
    s_lcdHeight = s_lcd.height;
    s_sw2WasPressed = SW2_IsPressed();
    Game_Reset();

    while (1)
    {
        const bool sw2Pressed = SW2_IsPressed();
        const bool flapPressedEdge = sw2Pressed && !s_sw2WasPressed;

        s_sw2WasPressed = sw2Pressed;
        Game_Update(flapPressedEdge, sw2Pressed);
        Game_DrawScene();
        SDK_DelayAtLeastUs(GAME_FRAME_DELAY_US, CLOCK_GetCoreSysClkFreq());
    }
}
