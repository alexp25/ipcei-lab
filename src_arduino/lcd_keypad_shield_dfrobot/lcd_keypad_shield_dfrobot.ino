/*!
   @file HelloWorld.ino
   @brief Show helloworld.
   @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
   @licence     The MIT License (MIT)
   @maintainer [yangfeng](feng.yang@dfrobot.com)
   @version  V1.0
   @date  2021-09-24
   @url https://github.com/DFRobot/DFRobot_RGBLCD1602
*/
#include "DFRobot_RGBLCD1602.h"

int colorR = 255;
int colorG = 0;
int colorB = 0;

/*
  Change the RGBaddr value based on the hardware version
  -----------------------------------------
       Moudule        | Version| RGBAddr|
  -----------------------------------------
  LCD1602 Module      |  V1.0  | 0x60   |
  -----------------------------------------
  LCD1602 Module      |  V1.1  | 0x6B   |
  -----------------------------------------
  LCD1602 RGB Module  |  V1.0  | 0x60   |
  -----------------------------------------
  LCD1602 RGB Module  |  V2.0  | 0x2D   |
  -----------------------------------------
*/

DFRobot_RGBLCD1602 lcd(/*RGBAddr*/0x60 ,/*lcdCols*/16,/*lcdRows*/2);  //16 characters and 2 lines of show

// 0: fade in G, 1: fade out R, 2: fade in B, 3: fade out G, 4: fade in R, 5: fade out B
int stage = 0;
int speed = 5; // Higher number = faster color change

void updateColor() {
  switch (stage) {
    case 0:
      colorG += speed;
      if (colorG >= 255) {
        colorG = 255;
        stage = 1;
      }
      break;
    case 1:
      colorR -= speed;
      if (colorR <= 0) {
        colorR = 0;
        stage = 2;
      }
      break;
    case 2:
      colorB += speed;
      if (colorB >= 255) {
        colorB = 255;
        stage = 3;
      }
      break;
    case 3:
      colorG -= speed;
      if (colorG <= 0) {
        colorG = 0;
        stage = 4;
      }
      break;
    case 4:
      colorR += speed;
      if (colorR >= 255) {
        colorR = 255;
        stage = 5;
      }
      break;
    case 5:
      colorB -= speed;
      if (colorB <= 0) {
        colorB = 0;
        stage = 6;
      }
      break;
  }

  // Reset loop back to stage 0
  if (stage == 6) {
    stage = 0;
  }
}

void setup() {
  /**
      @brief initialize the LCD and master IIC
  */
  lcd.init();

  lcd.setRGB(colorR, colorG, colorB);

  // Print a message to the LCD.
  lcd.print("hello, world!");

  delay(1000);
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  /**
      @brief set cursor position
      @param col columns optional range 0-15
      @param row rows optional range 0-1，0 is the first row, 1 is the second row
  */
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
  updateColor();
  lcd.setRGB(colorR, colorG, colorB);

  delay(100);
}
