#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void Delay(uint32_t);
void GPIO_Config(void);
void Clock_Config(void);

#define BUTTON1      GPIO_Pin_0
#define BUTTON2      GPIO_Pin_1
#define PORT_BUTTON  GPIOB

#define PORT_LED     GPIOC
#define Led0         GPIO_Pin_0
#define Led1         GPIO_Pin_1
#define Led2         GPIO_Pin_2
#define Led3         GPIO_Pin_3
#define Led4         GPIO_Pin_4
#define Led5         GPIO_Pin_5
#define Led6         GPIO_Pin_6
#define Led7         GPIO_Pin_7

int main(){
	Clock_Config();
	SystemCoreClockUpdate();
	GPIO_Config();
	
	GPIO_Write(PORT_LED, 0x00);
	while(1){
		if(GPIO_ReadInputDataBit(PORT_BUTTON, BUTTON1)==0){
			while(GPIO_ReadInputDataBit(PORT_BUTTON, BUTTON1)==0)
				GPIO_Write(PORT_LED, 0x00);
		}
		if(GPIO_ReadInputDataBit(PORT_BUTTON, BUTTON2)==0){
			while(GPIO_ReadInputDataBit(PORT_BUTTON, BUTTON2)==0)
				GPIO_Write(PORT_LED, 0xFF);
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
	GPIO_InitStructure.GPIO_Pin = BUTTON1 | BUTTON2;
	GPIO_Init(PORT_BUTTON, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = Led0 | Led1 | Led2 | Led3 | Led4 | Led5 | Led6 | Led7 ;
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

