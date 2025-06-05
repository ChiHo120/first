#include "headfile.h"

uint8_t B1_state;
uint8_t B1_last_state;
uint8_t B2_state;
uint8_t B2_last_state;
uint8_t B3_state;
uint8_t B3_last_state=1;
uint8_t B4_state;
uint8_t B4_last_state;

uint8_t B3_pressed;
uint8_t B3_time;

void key_scan()
{
	
	static uint32_t last_tick;
	uint32_t now_tick =HAL_GetTick();
	if(now_tick-last_tick<20)
	last_tick=now_tick;
	
	
	
	B1_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
	B2_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
	B3_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
	B4_state=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
	
	
	if(B1_state==0&&B1_last_state==1)
	{
		led1_flag=1;
		uart_flag=1;
		switch_range() ;
	
	}
	
	
	if(B2_state==0&&B2_last_state==1)
	{
		led2_flag++;
		led2_flag=led2_flag%2;
	
	}
	
	
	if(B3_state==0&&B3_last_state==1)
	{
		B3_pressed=1;
		B3_time=0;
	
	}
	
	else if(B3_state==1&&B3_last_state==0)
	{
		if(B3_time>=15)
		{
			led_show(4,1);
			B3_time=0;
			B3_pressed=0;
		}
		else 
		{
			led_show(3,1);
			B3_time=0;
			B3_pressed=0;
		
		}
	
	}
	
	
	if(B4_state==0&&B4_last_state==1)
	{
	
	
	}
	
	
		B1_last_state=B1_state;
		B2_last_state=B2_state;
		B3_last_state=B3_state;
		B4_last_state=B4_state;
}
