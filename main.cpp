//----------------------------------------------------------------------
//EASTRISING TECHNOLOGY CO,.LTD.//
// Module    : 3.5 INCH TFT LCD  320RGB*480 DOTS
// Lanuage   : C51 Code
// Create    : JAVEN LIU
// Date      : 2015-12-15
// Drive IC  : LCD:ILI9488  TP:TSC2046  FLASH: W25Q32BV   FONT CHIP:GT21L16T1W
// INTERFACE : LCD 4WRIE SPI   TP:SPI  FLASH:SPI   FONT CHIP:SPI  SD CARD: SPI
// MCU 		 : STC12LE5C60S2
// VDD		 : 5V OR 3.3 V
// MCU VDD   : 3.3V
//----------------------------------------------------------------------

//#define USE_3P5_DISPLAY

#include <stdio.h>
#include "mbed.h"
#include "lvgl/lvgl.h"
#include "display.h"
//#include "lv_tutorial/2_objects/lv_tutorial_objects.h"
// #include "lv_tutorial_styles.h"
//#include "lv_tutorial/4_themes/lv_tutorial_themes.h"
//#include "lv_tutorial/5_antialiasing/lv_tutorial_antialiasing.h"
//#include "lv_tutorial/6_images/lv_tutorial_images.h"
//#include "lv_tutorial/7_fonts/lv_tutorial_fonts.h"
//#include "lv_tutorial/8_animations/lv_tutorial_animations.h"
#include "lv_tutorial/9_responsive/lv_tutorial_responsive.h"
//#include "lv_tutorial/10_keyboard/lv_tutorial_keyboard.h"
//=============================================================

// For debug printing
Serial pc(USBTX, USBRX);

// defined
DigitalOut togglePin(D2);

Ticker milliTicker;
volatile uint32_t milliSeconds=0;
void milliTick() {
   milliSeconds++;
}

void handleLvglTasks() {
   static uint32_t oldtime = 0;
   static uint8_t count = 5;

   if (oldtime != milliSeconds) {
      oldtime = milliSeconds;
      
      lv_tick_inc(1);

      if (count > 0) {
         count--;
      } else {
         count = 5;
         lv_task_handler();
      }
   }
}

int main(int argc, char ** argv)
{
   pc.baud(115200);
   pc.printf("Starting...\r\n");

   displayInit();

   milliTicker.attach(&milliTick, 0.001);

   //demo_create();
   //lv_tutorial_objects();
   //lv_tutorial_styles();
   //lv_tutorial_themes();
   //lv_tutorial_antialiasing();
   //lv_tutorial_image();
   //lv_tutorial_fonts();
   //lv_tutorial_animations();
   lv_tutorial_responsive();
   //lv_tutorial_keyboard(displayGetInputDevice());

   uint32_t oldMillis = 0;

   while(1)
   {
      handleLvglTasks();
      displayRun();

      if (milliSeconds - oldMillis >= 1000) {
         oldMillis += 1000;
         pc.printf("%d\r\n", oldMillis);
      }
   }
}

