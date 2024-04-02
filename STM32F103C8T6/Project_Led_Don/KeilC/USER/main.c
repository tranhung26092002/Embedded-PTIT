#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"                  

void Delay(uint32_t);
void Clock_Config(void);
void GPIO_Config(void);
uint32_t count = 0U;

int main()
{
	Clock_Config();
	SystemCoreClockUpdate();
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);
	
	// Config GPIO pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Clock_Config(void){
	// RCC system Reset
	RCC_DeInit();
	// HCLK = SYSCLK
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	// PCLK2 = HCLK
	RCC_PCLK2Config(RCC_SYSCLK_Div2);
	// PCLK1 = HCLK/2
	RCC_PCLK1Config(RCC_SYSCLK_Div2);
	// enable HSI sourse clock
	RCC_HSICmd(ENABLE);
	// wait till PLL is ready
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET){}
	// select PLL as system clock sourse
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	// wait till PLL is used  as system clock sourse
	while(RCC_GetSYSCLKSource() != 0x00) {}
}

