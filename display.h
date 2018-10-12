#ifndef DISPLAY_H
#define DISPLAY_H

#include "lvgl/lvgl.h"

void displayInit(void);
void displayRun(void);
lv_indev_t * displayGetInputDevice();

#endif // DISPLAY_H

