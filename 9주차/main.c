#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "core_cm3.h"


void RCC_Configure() {

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 |  RCC_APB2Periph_GPIOA, ENABLE);
  
  
}
void GPIO_Configure() {

   GPIO_InitTypeDef GPIO_InitStructure;
        
   /*TODO: USART1, JoyStick Config */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        // USART TX

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        // USART RX


}

void USART_Configure() {
        USART_InitTypeDef usart;

      //USART 공통설정
        usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        usart.USART_BaudRate = 9600;
        usart.USART_WordLength = USART_WordLength_8b;
        usart.USART_StopBits = USART_StopBits_1;
        usart.USART_Parity = USART_Parity_No;
        usart.USART_Mode = (USART_Mode_Rx | USART_Mode_Tx);
        
        //USART1 설정
        USART_Init(USART1, &usart);
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
        EXTI_ClearITPendingBit(USART_IT_RXNE);
        USART_Cmd(USART1, ENABLE);
        
        //USART2 설정
        USART_Init(USART2, &usart);
        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
        EXTI_ClearITPendingBit(USART_IT_RXNE);
        USART_Cmd(USART2, ENABLE);
        
}
void NVIC_Configure() {
   /*TODO: NVIC_configuration */
        NVIC_InitTypeDef nvic;
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
       
       nvic.NVIC_IRQChannel = USART1_IRQn;
       nvic.NVIC_IRQChannelCmd = ENABLE;
       nvic.NVIC_IRQChannelPreemptionPriority = 0;
       nvic.NVIC_IRQChannelSubPriority = 0;
       NVIC_Init(&nvic);
        
        
       nvic.NVIC_IRQChannel = USART2_IRQn;
       nvic.NVIC_IRQChannelCmd = ENABLE;
       nvic.NVIC_IRQChannelPreemptionPriority = 1;
       nvic.NVIC_IRQChannelSubPriority = 1;
       NVIC_Init(&nvic);
       //우선순위 설정 및 USART 인터럽트를 사용하기 위해 설정
           

       
}



/*TODO: IRQHandler */
void USART1_IRQHandler(void) {
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET );
    uint16_t data = USART_ReceiveData(USART1);
    USART_SendData(USART2, data);
    
  }
  USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}

void USART2_IRQHandler(void) {
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
    uint16_t data = USART_ReceiveData(USART2);
    USART_SendData(USART1, data);
    
  }
  USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}


int main() {

   SystemInit();
   RCC_Configure();
   GPIO_Configure();
   USART_Configure();
   EXTI_Configure();
   NVIC_Configure();

   while(1){
          
        };

}