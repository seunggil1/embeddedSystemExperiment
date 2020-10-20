#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "core_cm3.h"


void RCC_Configure() {

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	/*TODO : APB2PeriphClockEnable */

}
void GPIO_Configure() {

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/*TODO: USART1, JoyStick Config */


	/*TODO: GPIO EXTILineConfig*/


}

void USART_Configure() {
	/*TODO: USART1 configuration*/

	/*TODO: USART1 cmd ENABLE*/

	/*TODO: USART1 IT Config*/
}

void EXTI_Configure() {
	/*TODO: EXTI configuration [ mode interrupt ] [Trigger_falling] */

}

void NVIC_Configure() {
	/*TODO: NVIC_configuration */
}


/*TODO: IRQHandler */



int main() {

	SystemInit();
	RCC_Configure();
	GPIO_Configure();
	USART_Configure();
	EXTI_Configure();
	NVIC_Configure();

	while(1);

}
