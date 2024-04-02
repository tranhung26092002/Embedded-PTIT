#include "stm32f4xx.h"                  // Device header

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"                  

void Delay(uint32_t);
void GPIO_Config(void);

int main()
{
	GPIO_Config();
	
	while(1){
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		Delay(100);
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		Delay(100);
	}
}
// Delay tuong doi
void Delay(uint32_t t)
{
	unsigned int i,j;
	
	for( i=0;i<t;i++)
	{
		for( j=0;j< 5000 ;j++);
	}
}

void GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// enable clock for GPIOC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	// Config GPIO pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}