
#ifndef LCD_9341TP_H
#define LCD_9341TP_H

void ILI9341_Initial(void);
void LCD_SetPos(unsigned short x0, unsigned short x1, unsigned short y0, unsigned short y1);
void ClearScreen(unsigned int bColor);
void LCD_FULL(unsigned int i);
void Enter_Sleep(void);
void Exit_Sleep(void);

void Write_Data_U16(unsigned int y);

extern const int lcd_width;
extern const int lcd_height;
#endif // LCD_9341TP_H
