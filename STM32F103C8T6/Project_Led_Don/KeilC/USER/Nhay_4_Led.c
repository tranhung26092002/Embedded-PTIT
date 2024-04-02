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
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		Delay(10);
		GPIO_SetBits(GPIOA, GPIO_Pin_1);
		Delay(10);
		
		GPIO_ResetBits(GPIOA, GPIO_Pin_3);
		Delay(20);
		GPIO_SetBits(GPIOA, GPIO_Pin_3);
		Delay(10);
		
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		Delay(30);
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		Delay(10);
		
		GPIO_ResetBits(GPIOA, GPIO_Pin_7);
		Delay(40);
		GPIO_SetBits(GPIOA, GPIO_Pin_7);
		Delay(10);
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
	
	// enable clock for GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// configuration GPIO Pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Clock_Config(void){
	// RCC system reset
	RCC_DeInit();
	// HCLK = SYSCLK
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	// PCLK2 = HCLK
	RCC_PCLK2Config(RCC_HCLK_Div2);
	// PCLK1 = HCLK/2
	RCC_PCLK1Config(RCC_HCLK_Div2);
	// ENABLE HSI SOURSE CLOCK
	RCC_HSICmd(ENABLE);
	// WAIT TILL PLL IS READY
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY)== RESET){}
	// select PLL is system clock sourse
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	// wait till PLL is used as system clock sourse
	while(RCC_GetSYSCLKSource() != 0x00){}
}
