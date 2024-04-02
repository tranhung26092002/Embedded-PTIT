#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void Delay(uint32_t);
void GPIO_Config(void);
void Clock_Config(void);

#define BUTTON       GPIO_Pin_0
#define PORT_BUTTON  GPIOB

#define PORT_LED     GPIOC
#define Led          GPIO_Pin_13

int main(){
	Clock_Config();
	SystemCoreClockUpdate();
	GPIO_Config();
	
	GPIO_SetBits(PORT_LED, Led);
	while(1){
		if(GPIO_ReadInputDataBit(PORT_BUTTON, BUTTON)==0){
			GPIO_ResetBits(PORT_LED, Led);
			Delay(50);
		}
		else{
			GPIO_SetBits(PORT_LED, Led);
			Delay(50);
		}
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
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = BUTTON;
	GPIO_Init(PORT_BUTTON, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = Led  ;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_2MHz;
	GPIO_Init(PORT_LED, &GPIO_InitStructure);
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

