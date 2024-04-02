#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include<stdio.h>

uint16_t LED7SEG[10]={0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0xF8, 0x80, 0x90};
void Delay(uint32_t);
void GPIO_Config(void);
void Clock_Config(void);

#define PORT_LED7SEG  GPIOC 
#define LED7SEG_A    	GPIO_Pin_0
#define LED7SEG_B    	GPIO_Pin_1
#define LED7SEG_C    	GPIO_Pin_2
#define LED7SEG_D    	GPIO_Pin_3
#define LED7SEG_E   	GPIO_Pin_4
#define LED7SEG_F    	GPIO_Pin_5
#define LED7SEG_G    	GPIO_Pin_6
#define LED7SEG_DP   	GPIO_Pin_7

#define LED1   				GPIO_Pin_1
#define LED2   				GPIO_Pin_2
#define LED3					GPIO_Pin_5
#define LED4					GPIO_Pin_6
#define PORT_LED   		GPIOA


int main(){
	Clock_Config();
	SystemCoreClockUpdate();
	GPIO_Config();
	
	GPIO_ResetBits(PORT_LED, LED1); // led1=0
	GPIO_ResetBits(PORT_LED, LED2); // led2=0

	while(1){
		uint8_t i, j, x;
		for(i=0;i<100;i++){
			for(j=0;j<24;j++){
				
				GPIO_Write(PORT_LED7SEG, LED7SEG[i/10]);
				GPIO_SetBits(PORT_LED, LED1);  // led1 = 1
				Delay(1);
				GPIO_ResetBits(PORT_LED, LED1);
				
				GPIO_SetBits(PORT_LED, LED3);
				GPIO_SetBits(PORT_LED, LED4);
				
				x = i%10;
				if(x %2 == 0){
				GPIO_Write(PORT_LED7SEG, LED7SEG[x]);
				GPIO_SetBits(PORT_LED, LED2);  // led2 =1
				GPIO_ResetBits(PORT_LED, LED3);
				Delay(1);
				GPIO_ResetBits(PORT_LED, LED2);
				}
				else{
				GPIO_Write(PORT_LED7SEG, LED7SEG[x]);
				GPIO_SetBits(PORT_LED, LED2);  // led2 =1
				GPIO_ResetBits(PORT_LED, LED4);
				Delay(1);
				GPIO_ResetBits(PORT_LED, LED2);
				}
			}
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
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LED1 | LED2 | LED3 | LED4;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_2MHz;
	GPIO_Init(PORT_LED, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LED7SEG_A | LED7SEG_B | LED7SEG_C | LED7SEG_D | LED7SEG_E | LED7SEG_F | LED7SEG_G | LED7SEG_DP ;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_2MHz;
	GPIO_Init(PORT_LED7SEG, &GPIO_InitStructure);
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
