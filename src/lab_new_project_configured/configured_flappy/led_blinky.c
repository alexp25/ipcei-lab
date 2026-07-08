/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "peripherals.h"
#include "pin_mux.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

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
#define GAME_FRAME_DELAY_US 0U
#define GAME_SW2_PRESSED_THRESHOLD 1000U
#define SW3_PRESSED_LEVEL 0U
#define GAME_SKY_COLOR ILI9341_COLOR_CYAN
#define GAME_FLOOR_COLOR 0x4208U
#define GAME_ROAD_COLOR ILI9341_COLOR_BLACK
#define GAME_ROAD_EDGE_COLOR ILI9341_COLOR_WHITE
#define GAME_PLAYER_COLOR ILI9341_COLOR_YELLOW
#define GAME_OBSTACLE_COLOR ILI9341_COLOR_RED
#define GAME_TEXT_COLOR ILI9341_COLOR_WHITE
#define GAME_HORIZON_Y 54U
#define GAME_ROAD_NEAR_HALF_W 118U
#define GAME_ROAD_FAR_HALF_W 18U
#define GAME_LANE_COUNT 3U
#define GAME_CENTER_LANE 1U
#define GAME_PLAYER_BASE_W 22
#define GAME_PLAYER_BASE_H 32
#define GAME_OBSTACLE_COUNT 3U
#define GAME_OBSTACLE_MIN_Z_Q8 (70 * 256)
#define GAME_OBSTACLE_MAX_Z_Q8 (240 * 256)
#define GAME_OBSTACLE_BASE_SPEED_Q8 1300U
#define GAME_OBSTACLE_MAX_SPEED_Q8 3200U
#define GAME_DIRTY_PAD 8
#define GAME_SCORE_W 120U
#define GAME_SCORE_H 24U
#define GAME_OVER_X 72U
#define GAME_OVER_Y 82U
#define GAME_OVER_W 150U
#define GAME_OVER_H 54U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void PWM0_LED_InitOutput(void);
static void PWM0_LED_UpdateFade(void);
static void SW2_ADC_InitInput(void);
static uint16_t SW2_ADC_ReadRaw(void);
static bool SW2_IsPressed(void);
static bool SW3_IsPressed(void);
static void Game_FillRectClipped(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
static uint16_t Game_CurrentSpeedQ8(void);
static uint16_t Game_RoadHalfWidthAtY(uint16_t y);
static int16_t Game_LaneCenterAtY(uint8_t lane, uint16_t y);
static uint16_t Game_ProjectTQ8(int32_t zQ8);
static uint16_t Game_ProjectY(int32_t zQ8);
static uint16_t Game_ProjectScale(int32_t zQ8);
static uint8_t Game_RandomLane(uint32_t seed);
static void Game_GetObstacleMainRect(int32_t zQ8, uint8_t lane, int16_t *x, int16_t *y, int16_t *w, int16_t *h);
static void Game_GetObstacleRect(int32_t zQ8, uint8_t lane, int16_t *x, int16_t *y, int16_t *w, int16_t *h);
static void Game_ResetObstacle(uint32_t index, uint32_t seed);
static void Game_Reset(void);
static void Game_DrawRoad(void);
static void Game_GetRectUnion(int16_t ax,
                              int16_t ay,
                              int16_t aw,
                              int16_t ah,
                              int16_t bx,
                              int16_t by,
                              int16_t bw,
                              int16_t bh,
                              int16_t *x,
                              int16_t *y,
                              int16_t *w,
                              int16_t *h);
static void Game_ClearGameplayRect(int16_t x, int16_t y, int16_t w, int16_t h);
static void Game_GetPlayerRect(uint8_t lane, int16_t *x, int16_t *y, int16_t *w, int16_t *h);
static void Game_ErasePlayerRect(void);
static void Game_DrawPlayerAtLane(uint8_t lane);
static void Game_DrawPlayer(void);
static void Game_DrawObstacle(uint32_t index);
static void Game_DrawScore(void);
static void Game_DrawGameOverOverlay(void);
static void Game_DrawScene(void);
static bool Game_HandleInput(bool moveLeftEdge, bool moveRightEdge);
static bool Game_CheckCollision(void);
static void Game_UpdateWorld(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile uint16_t counterPrintTick = 0;
static volatile uint32_t printCounter = 0;
static volatile bool flag = false;
static volatile uint8_t pwmDutyCycle = 0;
static uint16_t s_lastSw2Raw = 4095U;
static ili9341_t s_lcd;
static uint16_t s_lcdWidth;
static uint16_t s_lcdHeight;
static uint16_t s_centerX;
static uint16_t s_nearY;
static int32_t s_obstacleZQ8[GAME_OBSTACLE_COUNT];
static int32_t s_prevObstacleZQ8[GAME_OBSTACLE_COUNT];
static uint8_t s_obstacleLane[GAME_OBSTACLE_COUNT];
static uint8_t s_prevObstacleLane[GAME_OBSTACLE_COUNT];
static int16_t s_prevObstacleX[GAME_OBSTACLE_COUNT];
static int16_t s_prevObstacleY[GAME_OBSTACLE_COUNT];
static int16_t s_prevObstacleW[GAME_OBSTACLE_COUNT];
static int16_t s_prevObstacleH[GAME_OBSTACLE_COUNT];
static uint8_t s_playerLane;
static uint8_t s_prevPlayerLane;
static int16_t s_prevPlayerX;
static int16_t s_prevPlayerY;
static int16_t s_prevPlayerW;
static int16_t s_prevPlayerH;
static uint32_t s_score;
static uint32_t s_frame;
static bool s_gameOver;
static bool s_sw2WasPressed;
static bool s_sw3WasPressed;
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
        printCounter++;
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
    uint32_t timeout = 1000U;

    LPADC_DoSoftwareTrigger(ADC0_PERIPHERAL, 1UL << ADC_TRIGGER_ID);
    while (!LPADC_GetConvResult(ADC0_PERIPHERAL, &result))
    {
        if (timeout-- == 0U)
        {
            return s_lastSw2Raw;
        }
    }

    s_lastSw2Raw = (uint16_t)result.convValue;
    return s_lastSw2Raw;
}

static bool SW2_IsPressed(void)
{
    return SW2_ADC_ReadRaw() < GAME_SW2_PRESSED_THRESHOLD;
}

static bool SW3_IsPressed(void)
{
    return GPIO_PinRead(BOARD_INITPINS_SW3_GPIO, BOARD_INITPINS_SW3_GPIO_PIN) == SW3_PRESSED_LEVEL;
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

static uint16_t Game_CurrentSpeedQ8(void)
{
    uint32_t speed = GAME_OBSTACLE_BASE_SPEED_Q8 + (s_score * 12U);

    if (speed > GAME_OBSTACLE_MAX_SPEED_Q8)
    {
        speed = GAME_OBSTACLE_MAX_SPEED_Q8;
    }

    return (uint16_t)speed;
}

static uint16_t Game_RoadHalfWidthAtY(uint16_t y)
{
    const uint16_t span = (uint16_t)((s_nearY > GAME_HORIZON_Y) ? (s_nearY - GAME_HORIZON_Y) : 1U);
    uint16_t offset;
    uint32_t width;

    if (y <= GAME_HORIZON_Y)
    {
        return GAME_ROAD_FAR_HALF_W;
    }
    if (y >= s_nearY)
    {
        return GAME_ROAD_NEAR_HALF_W;
    }

    offset = (uint16_t)(y - GAME_HORIZON_Y);
    width = GAME_ROAD_FAR_HALF_W;
    width += (((uint32_t)(GAME_ROAD_NEAR_HALF_W - GAME_ROAD_FAR_HALF_W) * offset) / span);

    return (uint16_t)width;
}

static int16_t Game_LaneCenterAtY(uint8_t lane, uint16_t y)
{
    const int16_t halfWidth = (int16_t)Game_RoadHalfWidthAtY(y);
    const int16_t laneWidth = (int16_t)((halfWidth * 2) / (int16_t)GAME_LANE_COUNT);
    const int16_t left = (int16_t)s_centerX - halfWidth;

    if (lane >= GAME_LANE_COUNT)
    {
        lane = GAME_CENTER_LANE;
    }

    return (int16_t)(left + ((int16_t)laneWidth * (int16_t)lane) + (laneWidth / 2));
}

static uint16_t Game_ProjectTQ8(int32_t zQ8)
{
    const int32_t zRange = GAME_OBSTACLE_MAX_Z_Q8 - GAME_OBSTACLE_MIN_Z_Q8;
    int32_t tQ8;

    if (zQ8 < GAME_OBSTACLE_MIN_Z_Q8)
    {
        zQ8 = GAME_OBSTACLE_MIN_Z_Q8;
    }
    if (zQ8 > GAME_OBSTACLE_MAX_Z_Q8)
    {
        zQ8 = GAME_OBSTACLE_MAX_Z_Q8;
    }

    tQ8 = (int32_t)(((uint32_t)(GAME_OBSTACLE_MAX_Z_Q8 - zQ8) * 256U) / (uint32_t)zRange);
    return (uint16_t)(((uint32_t)tQ8 * (uint32_t)tQ8) / 256U);
}

static uint16_t Game_ProjectY(int32_t zQ8)
{
    const uint16_t yRange = (uint16_t)(s_nearY - GAME_HORIZON_Y);
    const uint16_t perspectiveQ8 = Game_ProjectTQ8(zQ8);
    const uint32_t y = GAME_HORIZON_Y + (((uint32_t)perspectiveQ8 * yRange) / 256U);

    return (uint16_t)y;
}

static uint16_t Game_ProjectScale(int32_t zQ8)
{
    const uint16_t perspectiveQ8 = Game_ProjectTQ8(zQ8);
    return (uint16_t)(1U + (((uint32_t)perspectiveQ8 * 4U) / 256U));
}

static uint8_t Game_RandomLane(uint32_t seed)
{
    return (uint8_t)(seed % GAME_LANE_COUNT);
}

static void Game_GetObstacleMainRect(int32_t zQ8, uint8_t lane, int16_t *x, int16_t *y, int16_t *w, int16_t *h)
{
    const uint16_t screenY = Game_ProjectY(zQ8);
    const uint16_t scale = Game_ProjectScale(zQ8);
    const int16_t centerX = Game_LaneCenterAtY(lane, screenY);

    *w = (int16_t)(10U * scale);
    *h = (int16_t)(9U * scale);
    *x = (int16_t)(centerX - (*w / 2));
    *y = (int16_t)screenY - *h;
}

static void Game_GetObstacleRect(int32_t zQ8, uint8_t lane, int16_t *x, int16_t *y, int16_t *w, int16_t *h)
{
    int16_t mainX;
    int16_t mainY;
    int16_t mainW;
    int16_t mainH;
    const int16_t scale = (int16_t)Game_ProjectScale(zQ8);

    Game_GetObstacleMainRect(zQ8, lane, &mainX, &mainY, &mainW, &mainH);

    if (scale <= 1)
    {
        *x = mainX;
        *y = mainY;
        *w = mainW;
        *h = mainH;
        return;
    }

    const int16_t barX = (int16_t)(mainX - (2 * scale));
    const int16_t barY = (int16_t)(mainY + (3 * scale));
    const int16_t barW = (int16_t)(mainW + (4 * scale));
    const int16_t barH = (int16_t)(2 * scale);
    const int16_t right = (int16_t)(((mainX + mainW) > (barX + barW)) ? (mainX + mainW) : (barX + barW));
    const int16_t bottom = (int16_t)(((mainY + mainH) > (barY + barH)) ? (mainY + mainH) : (barY + barH));

    *x = (mainX < barX) ? mainX : barX;
    *y = (mainY < barY) ? mainY : barY;
    *w = (int16_t)(right - *x);
    *h = (int16_t)(bottom - *y);
}

static void Game_ResetObstacle(uint32_t index, uint32_t seed)
{
    const int32_t spacingQ8 = ((GAME_OBSTACLE_MAX_Z_Q8 - GAME_OBSTACLE_MIN_Z_Q8) / (int32_t)GAME_OBSTACLE_COUNT);
    int32_t zQ8 = GAME_OBSTACLE_MAX_Z_Q8 + ((int32_t)index * spacingQ8);

    zQ8 += (int32_t)((seed % 45U) * 256U);
    s_obstacleZQ8[index] = zQ8;
    s_obstacleLane[index] = Game_RandomLane((seed * 37U) + (index * 11U));
}

static void Game_Reset(void)
{
    s_centerX = (uint16_t)(s_lcdWidth / 2U);
    s_nearY = (uint16_t)(s_lcdHeight - 10U);
    s_playerLane = GAME_CENTER_LANE;
    s_prevPlayerLane = s_playerLane;
    s_score = 0;
    s_frame = 0;
    s_gameOver = false;
    s_prevScore = s_score;
    s_prevGameOver = s_gameOver;
    s_fullRedrawNeeded = true;

    for (uint32_t i = 0U; i < GAME_OBSTACLE_COUNT; i++)
    {
        Game_ResetObstacle(i, (i * 23U) + 5U);
        s_prevObstacleZQ8[i] = s_obstacleZQ8[i];
        s_prevObstacleLane[i] = s_obstacleLane[i];
        Game_GetObstacleRect(s_obstacleZQ8[i],
                             s_obstacleLane[i],
                             &s_prevObstacleX[i],
                             &s_prevObstacleY[i],
                             &s_prevObstacleW[i],
                             &s_prevObstacleH[i]);
    }
    Game_GetPlayerRect(s_playerLane, &s_prevPlayerX, &s_prevPlayerY, &s_prevPlayerW, &s_prevPlayerH);

    (void)ili9341_fill_screen(&s_lcd, GAME_SKY_COLOR);
}

static void Game_DrawRoad(void)
{
    (void)ili9341_fill_rect(&s_lcd, 0U, 0U, s_lcdWidth, GAME_HORIZON_Y, GAME_SKY_COLOR);
    Game_FillRectClipped(0, (int16_t)GAME_HORIZON_Y, (int16_t)s_lcdWidth, (int16_t)(s_lcdHeight - GAME_HORIZON_Y),
                         GAME_FLOOR_COLOR);
    for (uint16_t y = GAME_HORIZON_Y; y < s_lcdHeight; y = (uint16_t)(y + 10U))
    {
        const uint16_t halfWidth = Game_RoadHalfWidthAtY(y);
        const uint16_t stripH = ((uint16_t)(y + 10U) > s_lcdHeight) ? (uint16_t)(s_lcdHeight - y) : 10U;
        Game_FillRectClipped((int16_t)(s_centerX - halfWidth), (int16_t)y, (int16_t)(halfWidth * 2U), (int16_t)stripH,
                             GAME_ROAD_COLOR);
    }
}

static void Game_GetRectUnion(int16_t ax,
                              int16_t ay,
                              int16_t aw,
                              int16_t ah,
                              int16_t bx,
                              int16_t by,
                              int16_t bw,
                              int16_t bh,
                              int16_t *x,
                              int16_t *y,
                              int16_t *w,
                              int16_t *h)
{
    const int16_t ar = (int16_t)(ax + aw);
    const int16_t ab = (int16_t)(ay + ah);
    const int16_t br = (int16_t)(bx + bw);
    const int16_t bb = (int16_t)(by + bh);
    const int16_t right = (ar > br) ? ar : br;
    const int16_t bottom = (ab > bb) ? ab : bb;

    *x = (ax < bx) ? ax : bx;
    *y = (ay < by) ? ay : by;
    *w = (int16_t)(right - *x);
    *h = (int16_t)(bottom - *y);
}

static void Game_ClearGameplayRect(int16_t x, int16_t y, int16_t w, int16_t h)
{
    int16_t clearX = (int16_t)(x - GAME_DIRTY_PAD);
    int16_t clearY = (int16_t)(y - GAME_DIRTY_PAD);
    int16_t clearW = (int16_t)(w + (2 * GAME_DIRTY_PAD));
    int16_t clearH = (int16_t)(h + (2 * GAME_DIRTY_PAD));
    int16_t clearBottom;

    if ((clearW <= 0) || (clearH <= 0))
    {
        return;
    }

    clearBottom = (int16_t)(clearY + clearH);
    if (clearBottom <= (int16_t)GAME_HORIZON_Y)
    {
        Game_FillRectClipped(clearX, clearY, clearW, clearH, GAME_SKY_COLOR);
        return;
    }

    if (clearY < (int16_t)GAME_HORIZON_Y)
    {
        const int16_t skyH = (int16_t)(GAME_HORIZON_Y - clearY);
        Game_FillRectClipped(clearX, clearY, clearW, skyH, GAME_SKY_COLOR);
        clearY = (int16_t)GAME_HORIZON_Y;
    }

    for (int16_t stripY = clearY; stripY < clearBottom; stripY = (int16_t)(stripY + 5))
    {
        int16_t stripH = 5;
        const uint16_t halfWidth = Game_RoadHalfWidthAtY((uint16_t)stripY);
        const int16_t roadLeft = (int16_t)(s_centerX - halfWidth);
        const int16_t roadRight = (int16_t)(s_centerX + halfWidth);
        const int16_t clearRight = (int16_t)(clearX + clearW);
        const int16_t drawLeft = (clearX > roadLeft) ? clearX : roadLeft;
        const int16_t drawRight = (clearRight < roadRight) ? clearRight : roadRight;

        if ((stripY + stripH) > clearBottom)
        {
            stripH = (int16_t)(clearBottom - stripY);
        }

        Game_FillRectClipped(clearX, stripY, clearW, stripH, GAME_FLOOR_COLOR);
        if (drawRight > drawLeft)
        {
            Game_FillRectClipped(drawLeft, stripY, (int16_t)(drawRight - drawLeft), stripH, GAME_ROAD_COLOR);
        }
    }
}

static void Game_GetPlayerRect(uint8_t lane, int16_t *x, int16_t *y, int16_t *w, int16_t *h)
{
    const int16_t playerX = Game_LaneCenterAtY(lane, s_nearY);
    const int16_t baseY = (int16_t)(s_nearY - 6U);

    *w = GAME_PLAYER_BASE_W;
    *h = GAME_PLAYER_BASE_H;
    *x = (int16_t)(playerX - (GAME_PLAYER_BASE_W / 2));
    *y = (int16_t)(baseY - GAME_PLAYER_BASE_H);
}

static void Game_ErasePlayerRect(void)
{
    Game_ClearGameplayRect(s_prevPlayerX, s_prevPlayerY, s_prevPlayerW, s_prevPlayerH);
}

static void Game_DrawPlayerAtLane(uint8_t lane)
{
    int16_t left;
    int16_t bodyTop;
    int16_t w;
    int16_t h;

    Game_GetPlayerRect(lane, &left, &bodyTop, &w, &h);
    (void)w;
    (void)h;

    Game_FillRectClipped((int16_t)(left + 6), (int16_t)(bodyTop + 9), 10, 15, GAME_PLAYER_COLOR);
    Game_FillRectClipped((int16_t)(left + 7), bodyTop, 8, 8, GAME_PLAYER_COLOR);
    Game_FillRectClipped((int16_t)(left + 2), (int16_t)(bodyTop + 13), 5, 10, GAME_PLAYER_COLOR);
    Game_FillRectClipped((int16_t)(left + 15), (int16_t)(bodyTop + 13), 5, 10, GAME_PLAYER_COLOR);
    Game_FillRectClipped((int16_t)(left + 5), (int16_t)(bodyTop + 24), 5, 8, GAME_PLAYER_COLOR);
    Game_FillRectClipped((int16_t)(left + 13), (int16_t)(bodyTop + 24), 5, 8, GAME_PLAYER_COLOR);
}

static void Game_DrawPlayer(void)
{
    Game_DrawPlayerAtLane(s_playerLane);
}

static void Game_DrawObstacle(uint32_t index)
{
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
    const uint16_t scale = Game_ProjectScale(s_obstacleZQ8[index]);

    Game_GetObstacleMainRect(s_obstacleZQ8[index], s_obstacleLane[index], &x, &y, &w, &h);
    Game_FillRectClipped(x, y, w, h, GAME_OBSTACLE_COLOR);
    if (scale > 1U)
    {
        Game_FillRectClipped((int16_t)(x - (2 * (int16_t)scale)), (int16_t)(y + (3 * (int16_t)scale)),
                             (int16_t)(w + (4 * (int16_t)scale)), (int16_t)(2U * scale), GAME_OBSTACLE_COLOR);
    }
}

static void Game_DrawScore(void)
{
    char scoreText[16];

    (void)ili9341_fill_rect(&s_lcd, 0U, 0U, GAME_SCORE_W, GAME_SCORE_H, GAME_SKY_COLOR);
    (void)snprintf(scoreText, sizeof(scoreText), "%lu", (unsigned long)s_score);
    (void)ili9341_write_string(&s_lcd, 6U, 6U, scoreText, GAME_TEXT_COLOR, GAME_SKY_COLOR, 2U);
}

static void Game_DrawGameOverOverlay(void)
{
    (void)ili9341_fill_rect(&s_lcd, GAME_OVER_X, GAME_OVER_Y, GAME_OVER_W, GAME_OVER_H, GAME_ROAD_COLOR);
    (void)ili9341_write_string(&s_lcd, 90U, 88U, "GAME OVER", ILI9341_COLOR_RED, GAME_ROAD_COLOR, 2U);
    (void)ili9341_write_string(&s_lcd, 90U, 112U, "SW2/SW3", GAME_TEXT_COLOR, GAME_ROAD_COLOR, 2U);
}

static void Game_DrawScene(void)
{
    bool drawn[GAME_OBSTACLE_COUNT] = {false};

    if (s_fullRedrawNeeded)
    {
        Game_DrawRoad();
        Game_DrawScore();
        s_fullRedrawNeeded = false;
    }
    else
    {
        for (uint32_t i = 0U; i < GAME_OBSTACLE_COUNT; i++)
        {
            int16_t currentX;
            int16_t currentY;
            int16_t currentW;
            int16_t currentH;
            int16_t unionX;
            int16_t unionY;
            int16_t unionW;
            int16_t unionH;

            Game_GetObstacleRect(s_obstacleZQ8[i], s_obstacleLane[i], &currentX, &currentY, &currentW, &currentH);
            Game_GetRectUnion(s_prevObstacleX[i],
                              s_prevObstacleY[i],
                              s_prevObstacleW[i],
                              s_prevObstacleH[i],
                              currentX,
                              currentY,
                              currentW,
                              currentH,
                              &unionX,
                              &unionY,
                              &unionW,
                              &unionH);
            Game_ClearGameplayRect(unionX, unionY, unionW, unionH);
        }
        if (s_playerLane != s_prevPlayerLane)
        {
            Game_ErasePlayerRect();
        }
        if (s_score != s_prevScore)
        {
            Game_DrawScore();
        }
    }

    for (uint32_t pass = 0U; pass < GAME_OBSTACLE_COUNT; pass++)
    {
        int32_t farthestZ = -1;
        uint32_t farthestIndex = 0U;

        for (uint32_t i = 0U; i < GAME_OBSTACLE_COUNT; i++)
        {
            if (!drawn[i] && (s_obstacleZQ8[i] > farthestZ))
            {
                farthestZ = s_obstacleZQ8[i];
                farthestIndex = i;
            }
        }
        Game_DrawObstacle(farthestIndex);
        drawn[farthestIndex] = true;
    }

    Game_DrawPlayer();
    if (s_gameOver && !s_prevGameOver)
    {
        Game_DrawGameOverOverlay();
    }

    for (uint32_t i = 0U; i < GAME_OBSTACLE_COUNT; i++)
    {
        s_prevObstacleZQ8[i] = s_obstacleZQ8[i];
        s_prevObstacleLane[i] = s_obstacleLane[i];
        Game_GetObstacleRect(s_obstacleZQ8[i],
                             s_obstacleLane[i],
                             &s_prevObstacleX[i],
                             &s_prevObstacleY[i],
                             &s_prevObstacleW[i],
                             &s_prevObstacleH[i]);
    }
    s_prevPlayerLane = s_playerLane;
    Game_GetPlayerRect(s_playerLane, &s_prevPlayerX, &s_prevPlayerY, &s_prevPlayerW, &s_prevPlayerH);
    s_prevScore = s_score;
    s_prevGameOver = s_gameOver;
}

static bool Game_HandleInput(bool moveLeftEdge, bool moveRightEdge)
{
    uint8_t oldLane = s_playerLane;

    if (s_gameOver)
    {
        if (moveLeftEdge || moveRightEdge)
        {
            Game_Reset();
            return true;
        }
        return false;
    }

    if (moveLeftEdge && (s_playerLane > 0U))
    {
        s_playerLane--;
    }
    if (moveRightEdge && (s_playerLane < (GAME_LANE_COUNT - 1U)))
    {
        s_playerLane++;
    }

    return s_playerLane != oldLane;
}

static bool Game_CheckCollision(void)
{
    for (uint32_t i = 0U; i < GAME_OBSTACLE_COUNT; i++)
    {
        if ((s_obstacleLane[i] == s_playerLane) &&
            (s_obstacleZQ8[i] < (GAME_OBSTACLE_MIN_Z_Q8 + (18 * 256))))
        {
            return true;
        }
    }

    return false;
}

static void Game_UpdateWorld(void)
{
    if (s_gameOver)
    {
        return;
    }

    for (uint32_t i = 0U; i < GAME_OBSTACLE_COUNT; i++)
    {
        s_obstacleZQ8[i] -= (int32_t)Game_CurrentSpeedQ8();
        if (s_obstacleZQ8[i] < GAME_OBSTACLE_MIN_Z_Q8)
        {
            s_score++;
            Game_ResetObstacle(i, s_frame + s_score + i);
        }
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
    s_sw3WasPressed = SW3_IsPressed();
    Game_Reset();

    while (1)
    {
        const bool sw3Pressed = SW3_IsPressed();
        const bool sw2Pressed = SW2_IsPressed();
        const bool moveLeftEdge = sw2Pressed && !s_sw2WasPressed;
        const bool moveRightEdge = sw3Pressed && !s_sw3WasPressed;

        s_sw2WasPressed = sw2Pressed;
        s_sw3WasPressed = sw3Pressed;
        (void)Game_HandleInput(moveLeftEdge, moveRightEdge);
        Game_UpdateWorld();
        Game_DrawScene();

        const bool lateSw3Pressed = SW3_IsPressed();
        const bool lateSw2Pressed = SW2_IsPressed();
        const bool lateMoveLeftEdge = lateSw2Pressed && !s_sw2WasPressed;
        const bool lateMoveRightEdge = lateSw3Pressed && !s_sw3WasPressed;

        s_sw2WasPressed = lateSw2Pressed;
        s_sw3WasPressed = lateSw3Pressed;
        if (Game_HandleInput(lateMoveLeftEdge, lateMoveRightEdge) && !s_fullRedrawNeeded)
        {
            Game_ErasePlayerRect();
            Game_DrawPlayer();
            s_prevPlayerLane = s_playerLane;
            Game_GetPlayerRect(s_playerLane, &s_prevPlayerX, &s_prevPlayerY, &s_prevPlayerW, &s_prevPlayerH);
        }

        if (GAME_FRAME_DELAY_US > 0U)
        {
            SDK_DelayAtLeastUs(GAME_FRAME_DELAY_US, CLOCK_GetCoreSysClkFreq());
        }
    }
}
