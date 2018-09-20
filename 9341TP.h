
#ifndef LCD_9341TP_H
#define LCD_9341TP_H

void ILI9341_Initial(void);
void LCD_SetPos(unsigned char x0,unsigned char x1,unsigned  int y0,unsigned int y1);
void ClearScreen(unsigned int bColor);
void LCD_FULL(unsigned int i);
void Enter_Sleep(void);
void Exit_Sleep(void);

void Write_Data_U16(unsigned int y);

#endif // LCD_9341TP_H
