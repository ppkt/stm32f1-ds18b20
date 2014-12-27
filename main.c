#include <stdio.h>
#include <stm32f10x_exti.h>
#include "utils.h"
#include "usart.h"
#include "one_wire.h"
#include "ds18b20.h"

void BTN_Interrupts() {
	// Enable clock and its interrupts
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

int main(void)
{
	USART1_Init(921600);
	BTN_Init();
	BTN_Interrupts();
	LED_Init();

	// Tick every 1 ms
	if (SysTick_Config(SystemCoreClock / 1000))  while (1);

	printf("Hello, World!\r\n");
	ds18b20_init(GPIOC, GPIO_Pin_6, TIM2);
    while(1)
    {
    	ds18b20_read_temperature_all();
    	ds18b20_wait_for_conversion();
    	printf("%d---\r\n", ds18b20_get_precission());
    	ds18b20_convert_temperature_all();
    }
}

void SysTick_Handler(void)
{
  delay_decrement();
}

void EXTI0_IRQHandler(void)
{
	static u8 i = 0;

    if (EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        unsigned int j = 0;
        while (j != 200000) ++j;
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) != Bit_RESET) {
			return;
		}

    	i = (i + 1) % 4;
		ds18b20_set_precission(i);
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

