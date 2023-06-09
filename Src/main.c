#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "ultrasonic.h"
#include "lcd.h"



void Display_Distance(int);
void Interrupt_Config();

int Object_Detected=0,Prev_Distance,Distance;
int T1,T2,Time;

int main()
{
	Trigger_init(); // PA5 - TRIGGER INITIALISATION
	I2C_Init();
	LCD_Init();
	LCD_Cursor(0,0);
	Tim3_Echo_Input_Capture(); // PA6 - TIMER 3 INPUT CAPTURE MODE Initialization
	Interrupt_Config(); // Configuring TIMER3 for interrupt mode
	while(1)
	{
		TIM4_ms_Delay(10); //Delay of 10ms before next trigger
	    Trigger(); //Sending 10us pulse
	}
}


void TIM3_callback()
{
	if(Object_Detected==0)
	{
		T1=TIM3->CCR1;          /* Save the current timer value at sending Ultrasonic waves */
	    Object_Detected=1; 		/* make the object detected flag 1 */
		TIM3->CCER |= ( TIM_CCER_CC1P ); /*Set CH1 to capture at falling edge*/
	 }
	else
	{
	  T2=TIM3->CCR1;  /* Save the current timer value at receiving the reflected Ultrasonic waves  */
	  if(T2>T1) /*If no timer overflow */
	  {
	    Time=T2-T1;
	  }
	  else /*If timer overflow */
	  {
	  Time=(0xFFFF-T1)+T2; //if timer over flow happens
	  }
	  Distance=Time/58;    //convert time to distance
	  Object_Detected=0;
	  T2=T1=0;
	  TIM3->CCER &= ~( TIM_CCER_CC1P ); /*Set CH1 to capture at rising edge*/
	  if(Distance>0 && Prev_Distance!=Distance) /* If there is a new value of distance */
	  {
		  Prev_Distance=Distance;
		  Display_Distance(Distance);
	  }
    }
}

void Display_Distance(int dis)
{
	char str[10];
	int i=0;
    sprintf(str,"%d",dis); //converting integer to string
    LCD_Write_Cmd(LCD_ADDR,INST_REG,0x01); //Clearing the screen
    TIM4_ms_Delay(10);
    LCD_Cursor(0,0);
    LCD_Write_Data(LCD_ADDR,DATA_REG, 0x44); //Character Generation ROM Pattern for 'D'
    LCD_Write_Data(LCD_ADDR,DATA_REG, 0x49); //Character Generation ROM Pattern for 'I'
    LCD_Write_Data(LCD_ADDR,DATA_REG, 0x53); //Character Generation ROM Pattern for 'S'
    LCD_Write_Data(LCD_ADDR,DATA_REG, 0x54); //Character Generation ROM Pattern for 'T'
    LCD_Write_Data(LCD_ADDR,DATA_REG, 0x3D); //Character Generation ROM Pattern for '='
    while(str[i]!=NULL)
	{
	LCD_Write_Data(LCD_ADDR,DATA_REG, (0x30+(str[i])-48)); //displaying each character
	i++;
	}
    LCD_Write_Data(LCD_ADDR,DATA_REG, 0x43); //Character Generation ROM Pattern for 'C'
    LCD_Write_Data(LCD_ADDR,DATA_REG, 0x4D); //Character Generation ROM Pattern for 'M'
}

void Interrupt_Config()
{
	/* Enable Timer3 interrupt */
	TIM3->DIER |= (1U<<1);
	/* Enable Timer3 in NVIC */
	NVIC_EnableIRQ(TIM3_IRQn );
}

void TIM3_IRQHandler()
{
	if(TIM3->SR & SR_CC1IF)
	{
		/*Clear input capture flag */
		TIM3->SR &=  ~(SR_CC1IF);
		TIM3_callback();
	}
}
