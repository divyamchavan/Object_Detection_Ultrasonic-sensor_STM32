/*
 * lcd.h
 *
 *  Created on: May 17, 2023
 *      Author: divya
 */

#ifndef LCD_H_
#define LCD_H_

void GPIO_Init();
void I2C_Init();
void LCD_Init();
void LCD_Cursor();
void LCD_Write_Data(uint8_t ,uint8_t ,uint8_t );
void LCD_Write_Cmd(uint8_t ,uint8_t ,uint8_t );
void TIM4_ms_Delay(uint16_t);
#define LCD_ADDR (0x27)
#define INST_REG (0x00)
#define DATA_REG (0x40)

#endif /* LCD_H_ */
