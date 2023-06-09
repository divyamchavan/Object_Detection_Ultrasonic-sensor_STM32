/*
 * ultrasonic.c
 *
 *  Created on: May 17, 2023
 *      Author: divya
 */


/*
 * PA5 - TRIGGER
 * PA6 - TIMER 3 INPUT CAPTURE MODE
 * TIMER 2 DELAY
 */


#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"


#define TIM2EN		(1U<<0)
#define TIM3EN		(1U<<1)

#define CR1_CEN		(1U<<0)
#define OC_TOGGLE	((1U<<4) | (1U<<5))
#define CCER_CC1E	(1U<<0)


#define GPIOAEN		(1U<<0)
#define AFR5_TIM	(1U<<20)
#define AFR6_TIM	(1U<<25)
#define CCER_CC1S   (1U<<0)
#define SR_CC1IF    (1U<<1)

void Delay_us(int);
void Tim3_Echo_Input_Capture();
void pin5_init();
void Trigger_init();

void Trigger_init()
{

	 /*Enable clock access to GPIOA*/
		RCC->AHB1ENR |= GPIOAEN;
	 /*Set mode to output*/
		GPIOA->MODER |= (1U<<10);
		GPIOA->MODER &=~(1U<<11);
}

void Delay_us(int delay)
{
	/*Enable clock access to tim2*/
	RCC->APB1ENR |= (1U<<0);
	/*set pre-scalar*/
	TIM2->PSC = 16 - 1;
	/*set reload value*/
	TIM2->ARR = delay - 1;
	/*clear current value*/
	TIM2->CNT = 0;
	/*enable tim2*/
	TIM2->CR1 = (1U<<0);
	/*wait until flag is set*/
	while(!(TIM2->SR & TIM_SR_UIF)){} //Polling the update interrupt flag
	TIM2->SR &= ~(0x0001); //Reset the update interrupt flag
}



void Tim3_Echo_Input_Capture()
{

	 /*Enable clock access to GPIOA*/
	RCC->AHB1ENR |=GPIOAEN;

	/*Set PA6 mode to alternate function*/
	GPIOA->MODER &=~(1U<<12);
	GPIOA->MODER |=(1U<<13);

	/*Set PA6 alternate function type to TIM3_CH1 (AF02)*/
	GPIOA->AFR[0]|=AFR6_TIM;

	/*Enable clock access to tim3*/
	RCC->APB1ENR |=TIM3EN;

	/*Set Pre-scaler*/
	TIM3->PSC = 16 -1; // 1MHZ frequency of Timer3

	/*Set CH1 to input capture*/
	TIM3->CCMR1  = CCER_CC1S;


	/* Enable Input capture mode */
	TIM3->CCER |= (TIM_CCER_CC1E);

	/*Set CH1 to capture at rising edge*/
	TIM3->CCER &= ~( TIM_CCER_CC1P );

	/*Enable TIM3*/
	TIM3->CR1 = CR1_CEN;
}

/* Sending a pulse of 10us to send Ultrasonic waves */
void Trigger()
{
	GPIOA->BSRR |= (1U<<21);  //PA5 is low
	Delay_us(2);
	GPIOA->BSRR |= (1U<<5);   //PA5 set to High
	Delay_us(10);              // wait for 10us
	GPIOA->BSRR |= (1U<<21);  // Make PA5 low again
}

