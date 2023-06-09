/*
 * PB6 - I2C1_SCL -brown -ch1
 * PB7 - I2C1_SDA - white - ch5
 *
 *
 */


#include "stm32f4xx.h"


//Register Definitions for 1602A I2C LCD
#define LCD_ADDR (0x27)
#define INST_REG (0x00)
#define DATA_REG (0x40)

void SCL_SDA_Init()
{
    //Enable GPIOB clock
    RCC->AHB1ENR |= 1UL<<1;

    // Configuring PB6 and PB7 in Alternate function
    GPIOB->MODER |= ( (2UL<<(6*2)) | (2UL<<(7*2)) );

    // Selecting PB6 and PB7 as Pull up pins
    GPIOB->PUPDR |= ( (1UL<<(6*2)) | (1UL<<(7*2)));

    // Setting PB6 and PB7 as open drain
    GPIOB->OTYPER |= ( (1UL<<6) | (1UL<<7) );

    // Setting PB6 and PB7 at high speed
    GPIOB->OSPEEDR |= ( (2UL<<(6*2)) | (2UL<<(7*2)) );

    // Selecting the Alternate function (AF4)
    GPIOB->AFR[0] |= ( (4UL<<(6*4)) | (4UL)<<(7*4));
}

void I2C_Init()
{
	/* Initializing PB6 and PB7 for SCL and SDA */
	SCL_SDA_Init();
	/* Enable clock access to I2C1*/
	RCC->APB1ENR |= (1U<<21);
	/* Reset I2C */
	I2C1->CR1 |=  (1U<<15);
	I2C1->CR1 &= ~(1U<<15);
	/* Set I2C clock at 16MHz */
	I2C1->CR2 |= (16UL<<0);
	/* Needs to be set high by software for I2C */
	I2C1->OAR1 |= (1U<<14);
	/* Set SCL clock at 100KHz */
	I2C1->CCR |= (0x50UL<<0);
	/* Configure rise time as 1000ns */
	I2C1->TRISE |= (17U<<0);
	/* Enable I2C */
	I2C1->CR1 |= (1U<<0);
}


void TIM4_ms_Delay(uint16_t delay){
    RCC->APB1ENR |= 1<<2; //Start the clock for the timer peripheral
    TIM4->PSC = 16000-1; //Setting the clock frequency to 1kHz.
    TIM4->ARR = delay; // Total period of the timer
    TIM4->CNT = 0;
    TIM4->CR1 |= 1; //Start the Timer
    while(!(TIM4->SR & TIM_SR_UIF)){} //Polling the update interrupt flag
    TIM4->SR &= ~(0x0001); //Reset the update interrupt flag
}



void I2C_Start()
{
   int clc=0;
  /* Enable the acknowledgment bit */
	I2C1->CR1 |= (1U<<10);
  /* Generate the start bit  */
	I2C1->CR1 |= (1U<<8);
  /* Wait until start flag is set */
	while(!((I2C1->SR1)&(1U<<0)));
	clc=I2C1->SR1;
}

void I2C_Write(uint8_t data)
{
  /* Wait until data register is empty*/
	while(!((I2C1->SR1)&(1U<<7)));
  /* Put the data in data register */
	I2C1->DR = data;
  /* Poll the BTF(Byte transfer bit) to confirm byte transfer */
	while(!((I2C1->SR1)&(1U<<2)));
}

void I2C_Send_Addr(uint8_t addr)
{
	int clc=0;
	/* Put the address in data register */
	I2C1->DR = (addr<<1);
	/* Poll the ADDR bit */
	while(!((I2C1->SR1)&(1U<<1)));
	/* clear the ADDR bit - done by reading status register */
	clc = (I2C1->SR1)|(I2C1->SR2);
}

void I2C_Stop()
{
	//stop the communication
	I2C1->CR1 |= (1U<<9);
}

void LCD_Write_Cmd(uint8_t Device_Addr,uint8_t Slave_Reg_Addr,uint8_t data)
{
	uint8_t d_h,d_l,d1,d2,d3,d4;

	d_h =  data & (0xF0);
	d_l = (data<<4) & (0xF0);

   //For writing to instruction register and send upper 4 bits
    d1 = d_h | 0x0C; // EN=1 RS=0 R/W=0
    d2 = d_h | 0x08; // EN=0 RS=0 R/W=0

    //For writing to instruction register and send lower 4 bits
       d3 = d_l | 0x0C; // EN=1 RS=0 R/W=0
       d4 = d_l | 0x08; // EN=0 RS=0 R/W=0

    I2C_Start();
    I2C_Send_Addr(Device_Addr);
    I2C_Write(Slave_Reg_Addr);
    I2C_Write(d1);
    TIM4_ms_Delay(2);
    I2C_Write(d2);
    I2C_Write(d3);
    TIM4_ms_Delay(2);
    I2C_Write(d4);
    I2C_Stop();
}

void LCD_Write_Data(uint8_t Device_Addr,uint8_t Slave_Reg_Addr,uint8_t data)
{
	uint8_t d_h,d_l,d1,d2,d3,d4;

	d_h =  data & (0xF0);
	d_l = (data<<4) & (0xF0);

   //For writing to data register and send upper 4 bits
       d1 = d_h | 0x0D; // EN=1 RS=1 R/W=0
       d2 = d_h | 0x09; // EN=0 RS=1 R/W=0

    //For writing to data register and send lower 4 bits
       d3 = d_l | 0x0D; // EN=1 RS=1 R/W=0
       d4 = d_l | 0x09; // EN=0 RS=1 R/W=0

    I2C_Start();
    I2C_Send_Addr(Device_Addr);
    I2C_Write(Slave_Reg_Addr);
    I2C_Write(d1);
    TIM4_ms_Delay(1);
    I2C_Write(d2);
    I2C_Write(d3);
    TIM4_ms_Delay(1);
    I2C_Write(d4);
    TIM4_ms_Delay(20);
    I2C_Stop();
}

void LCD_Cursor(int r,int c)
{
	if(r==1)
	{
		c |= 0xC0;
		LCD_Write_Cmd(LCD_ADDR,INST_REG,c);

	}
	else
	{
		c |= 0x80;
		LCD_Write_Cmd(LCD_ADDR,INST_REG,c);
	}
}
void LCD_Write_Cmd_8(uint8_t Device_Addr,uint8_t Slave_Reg_Addr,uint8_t data)
{
	uint8_t d_h,d1,d2;

	d_h =  data & (0xF0);

   //For writing to instruction register and send upper 4 bits
    d1 = d_h | 0x0C; // EN=1 RS=0 R/W=0
    d2 = d_h | 0x08; // EN=0 RS=0 R/W=0



    I2C_Start();
    I2C_Send_Addr(Device_Addr);
    I2C_Write(Slave_Reg_Addr);
    I2C_Write(d1);
    TIM4_ms_Delay(2);
    I2C_Write(d2);
    I2C_Stop();
}


void LCD_Init()
{
	    // 1. Initializing the LCD in 4-bit mode
	    TIM4_ms_Delay(150);
	    LCD_Write_Cmd_8(LCD_ADDR,INST_REG,0x30);
	    TIM4_ms_Delay(10);

	    LCD_Write_Cmd_8(LCD_ADDR,INST_REG,0x30);
	    TIM4_ms_Delay(10);

	    LCD_Write_Cmd_8(LCD_ADDR,INST_REG,0x30);
	    TIM4_ms_Delay(10);

	    LCD_Write_Cmd_8(LCD_ADDR,INST_REG,0x20); // Set the LCD in 4-bit Mode
	    TIM4_ms_Delay(150);

	    // 2. Initializing the Display

	    // Function Set (DL=0 for 4-bit mode; N=1 for 2-line display;
	    // F=0 for 5x8 characters)
	    LCD_Write_Cmd(LCD_ADDR,INST_REG,0x28);
	    TIM4_ms_Delay(10);

	    // Display Control (D=0;C=0;B=0 - Display is off)
	    LCD_Write_Cmd(LCD_ADDR,INST_REG,0x08);
	    TIM4_ms_Delay(10);

	    // Clear the display
	    LCD_Write_Cmd(LCD_ADDR,INST_REG,0x01);
	    TIM4_ms_Delay(1000);

	    TIM4_ms_Delay(1); // Wait for some time

	    // Set Entry Mode (ID=1 for incrementing cursor and S=0 for no shift)
	    LCD_Write_Cmd(LCD_ADDR,INST_REG,0x06);
	    TIM4_ms_Delay(10);

	    // Display Control (D=1;C=0;B=0 - Cursor blinks)
         LCD_Write_Cmd(LCD_ADDR,INST_REG,0x0E);
	    TIM4_ms_Delay(5);

	   // LCD_Write_Cmd(LCD_ADDR,INST_REG,0x00);
	    	    TIM4_ms_Delay(5);
	}


