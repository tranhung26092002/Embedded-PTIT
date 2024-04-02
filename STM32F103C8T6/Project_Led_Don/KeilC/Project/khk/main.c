#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

void delay_ms(uint32_t time){
	uint8_t i , j ;
	for(i=0;i< time;i++){
		for( j=0;j < 10000;j++){
		}
	}
}
void gpioinit(void){
		GPIO_InitTypeDef gpio;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		gpio.GPIO_Mode = GPIO_Mode_Out_PP;
		gpio.GPIO_Pin = GPIO_Pin_13; 
		gpio.GPIO_Speed = GPIO_Speed_50MHz ;
		GPIO_Init(GPIOC,&gpio);
}
int main(){
		gpioinit();
	while(1){
		
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		delay_ms(500);
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		delay_ms(500);
	}
}