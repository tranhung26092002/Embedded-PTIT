#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void Delay(uint32_t);
void GPIO_Config(void);
void Clock_Config(void);

int main(){
	Clock_Config();
	SystemCoreClockUpdate();
	GPIO_Config();
	
	while(1){
		GPIO_Write(GPIOC, 0xAA);
		Delay(100);
		GPIO_Write(GPIOC, 0x55);
		Delay(100);
	}
}

void Delay(uint32_t t){
	unsigned int i,j;
	for(i=0;i<t;i++){
		for(j=0;j<0x2AFF;j++);
	}
}

void GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void Clock_Config(void){
	RCC_DeInit();
	
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK2Config(RCC_HCLK_Div2);
	RCC_PCLK1Config(RCC_HCLK_Div2);
	RCC_HSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY)==RESET){}
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	while(RCC_GetSYSCLKSource() != 0x00){}
}
