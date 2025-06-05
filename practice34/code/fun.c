#include "headfile.h"

uint8_t led1_flag;
uint16_t led1_20s;
uint8_t led2_flag;
uint16_t led2_state;
uint32_t fre1;
uint32_t capture1;
uint32_t fre2;
uint32_t capture2;
uint8_t uart_flag;

char text[20];

void led_show(uint8_t led,uint8_t mode)
{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
		if(mode)
		{
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8<<(led-1),GPIO_PIN_RESET);	
		}
		else 
		{
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8<<(led-1),GPIO_PIN_SET);	
		}
		
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

void change()
{
	led_show(1,led1_flag);
	led_show(2,led2_state);
	lcd_show();
	 uart_rx();
}


void lcd_show()
{
	
	sprintf(text,"        test");
	LCD_DisplayStringLine(Line1,(uint8_t*)text);
	sprintf(text,"    adc1:%.2f",get_vol1());
	LCD_DisplayStringLine(Line3,(uint8_t*)text);
	sprintf(text,"    adc2:%.2f   ",get_vol2());
	LCD_DisplayStringLine(Line4,(uint8_t*)text);
	
	


}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM3)
	{
		capture1=HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_1)+1;
		TIM3->CNT=0;
		fre1=1000000/capture1;
	
	
	}
		
	
	if(htim->Instance==TIM2)
	{
		capture2=HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_1)+1;
		TIM2->CNT=0;
		fre2=1000000/capture2;
	
	
	}


}

float get_vol1()
{
	HAL_ADC_Start(&hadc1);
	uint32_t capture=HAL_ADC_GetValue(&hadc1);
	return 3.3*capture/4096;

}

float get_vol2()
{
	HAL_ADC_Start(&hadc2);
	uint32_t capture=HAL_ADC_GetValue(&hadc2);
	return 3.3*capture/4096;

}






void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM16)
	{
		if(led1_flag)
		{
			led1_20s++;
			if(led1_20s>=20)
			{
				led1_flag=0;
				led1_20s=0;
			}
		
		}
		
		
		
		
		if(led2_flag)
		{
				led2_state=!led2_state;
		}
		else
		{
		led2_state=0;
		}
		
		
		if(B3_pressed)
		{
			B3_time++;
		
		}
	
	}


}
















void uart_rx()
{
	if(uart_flag)
{
		sprintf(text,"tang yu bo");
		HAL_UART_Transmit(&huart1,(uint8_t*)text,sizeof(text),1);
		uart_flag=0;
	
	}



}



void eeprom_write(uint8_t addr,uint8_t data)
{
	I2CStart();
	I2CSendByte(0xA0);
	I2CWaitAck();
	I2CSendByte(addr);
	I2CWaitAck();
	
	I2CSendByte(data);
	I2CWaitAck();
	I2CStop();
	HAL_Delay(20);


}


uint16_t eeprom_read(uint8_t addr)
{
	I2CStart();
	I2CSendByte(0xA0);
	I2CWaitAck();
	I2CSendByte(addr);
	I2CWaitAck();
	
	I2CStart();
	I2CSendByte(0xA1);
	I2CWaitAck();
  uint16_t data=I2CReceiveByte();
	I2CSendNotAck();
	I2CStop();
	return data;

}
