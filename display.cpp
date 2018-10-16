#include "mbed.h"
#include "9341TP.h"
#include "lvgl/lvgl.h"
#ifdef USE_3P5_DISPLAY
#include<9488TP.h>
#else
#include<9341TP.h>
#endif
#include"Adafruit_FT6206.h"

static DigitalOut lcd_backlight(PD_2);
static InterruptIn touchInt(PA_15);
static bool touchIntOccurred = false;

Adafruit_FT6206 ctp = Adafruit_FT6206();

lv_disp_drv_t disp_drv;
static void ex_disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);
static void ex_disp_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);
static void ex_disp_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,  lv_color_t color);
static lv_indev_drv_t indev_drv;
static lv_indev_t * inputDevice;

/*
Ticker lvMillisTicker;
void lvMillisTick() {
   lv_tick_inc(1);
}

Ticker lvTaskTicker;
void lvTaskTick() {
   lv_task_handler();
}
*/

/* Flush the content of the internal buffer the specific area on the display
 * You can use DMA or any hardware acceleration to do this operation in the background but
 * 'lv_flush_ready()' has to be called when finished
 * This function is required only when LV_VDB_SIZE != 0 in lv_conf.h*/
static void ex_disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
   uint16_t x, y;

   LCD_SetPos(x1, x2, y1, y2);

   for(y=y1; y<=y2; y++) {
      for(x=x1; x<=x2; x++, color_p++) {
         Write_Data_U16(color_p->full);
      }
   }

   lv_flush_ready();
}


/* Write a pixel array (called 'map') to the a specific area on the display
 * This function is required only when LV_VDB_SIZE == 0 in lv_conf.h*/
static void ex_disp_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
   uint16_t x, y;

   LCD_SetPos(x1, x2, y1, y2);

   for(y=y1; y<=y2; y++) {
      for(x=x1; x<=x2; x++, color_p++) {
         Write_Data_U16(color_p->full);
      }
   }

   lv_flush_ready();
}

/* Fill a rectancular region with a color. */
static void ex_disp_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,  lv_color_t color)
{
   uint16_t x, y;

   LCD_SetPos(x1, x2, y1, y2);

   for(y=y1; y<=y2; y++) {
      for(x=x1; x<=x2; x++) {
         Write_Data_U16(color.full);
      }
   }

   lv_flush_ready();
}

static int lastX = 0;
static int lastY = 0;
static lv_indev_state_t lastState = LV_INDEV_STATE_REL;

void handleTouch() {
   static bool touched = false;

   if (ctp.touched()) {
      // Retrieve a point
      TS_Point p = ctp.getPoint();
      // lastY = p.y;
      // lastX = p.x;
      lastY = p.x;
      lastX = lcd_width - p.y;
      lastState = LV_INDEV_STATE_PR;
      touched = true;
   } 
   else if (touched) {
      touched = false;
      lastState = LV_INDEV_STATE_REL;
      printf("Last touch (%d, %d)\r\n", lastX, lastY);
   }

   touchIntOccurred = false;
}

void handleTouchInterrupt() {
   touchIntOccurred = true;
}

bool readTouchScreen(lv_indev_data_t *data)
{
   data->point.x = lastX;
   data->point.y = lastY;
   data->state = lastState;
   return false;
}

void displayInit(void) {
   lcd_backlight = 1;

#ifdef USE_3P5_DISPLAY
   ILI9488_Initial();//LCD initialization
#else
   ILI9341_Initial();//LCD initialization
#endif

   wait_ms(500);

   lv_init();

   wait_ms(300);
   printf("Starting the FT6206 driver\r\n");
   if (! ctp.begin(40)) {  // pass in 'sensitivity' coefficient
      printf("Couldn't start FT6206 touchscreen controller\r\n");
      while (1);
   }

   printf("Touchscreen started.\r\n");
   printf("Registering the touchscreen driver.\r\n");
   lv_indev_drv_init(&indev_drv);
   indev_drv.type = LV_INDEV_TYPE_POINTER;
   indev_drv.read = readTouchScreen;
   inputDevice = lv_indev_drv_register(&indev_drv);
   printf("Input device value: %p\r\n", inputDevice);

   lv_disp_drv_init(&disp_drv);           /*Basic initialization*/

   printf("Registering the callbacks.\r\n");
   /*Set up the functions to access to your display*/
   disp_drv.disp_flush = ex_disp_flush;            /*Used in buffered mode (LV_VDB_SIZE != 0  in lv_conf.h)*/
   disp_drv.disp_fill = ex_disp_fill;              /*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/
   disp_drv.disp_map = ex_disp_map;                /*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/

   printf("Registering the driver.\r\n");
   /*Finally register the driver*/
   lv_disp_drv_register(&disp_drv);

   // lvTaskTicker.attach(&lvTaskTick, 0.005);
   // lvMillisTicker.attach(&lvMillisTick, 0.001);

   touchInt.fall(&handleTouchInterrupt);
}

void displayRun(void) {
   if (touchIntOccurred) handleTouch();
}

lv_indev_t * displayGetInputDevice() {
   return inputDevice;
}
