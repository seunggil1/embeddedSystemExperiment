#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "core_cm3.h"

int LED1_flag=0,
    LED2_flag=0,
    U_flag = 0,
    LED_Up=0,
    LED_Down=0;

int LED_Index = 0;
uint16_t LED[] = {GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_4,GPIO_Pin_7};

char data = 0;
char before_data = 0;

void RCC_Configure() {

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
   /*TODO : APB2PeriphClockEnable */
        RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPBEN, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPCEN, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPDEN, ENABLE);
}
void GPIO_Configure() {

   GPIO_InitTypeDef GPIO_InitStructure_Led;
   GPIO_InitStructure_Led.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure_Led.GPIO_Pin = (GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7);
   GPIO_InitStructure_Led.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOD, &GPIO_InitStructure_Led);

   /*TODO: USART1, JoyStick Config */
   GPIO_InitTypeDef GPIO_InitStructure_Sel;
   GPIO_InitStructure_Sel.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_InitStructure_Sel.GPIO_Pin = (GPIO_Pin_11);
   GPIO_InitStructure_Sel.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure_Sel);
        
        GPIO_InitTypeDef GPIO_InitStructure_Joy_L;
   GPIO_InitStructure_Joy_L.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_InitStructure_Joy_L.GPIO_Pin = (GPIO_Pin_3);
   GPIO_InitStructure_Joy_L.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOC, &GPIO_InitStructure_Joy_L);
        
        
   GPIO_InitTypeDef GPIO_InitStructure_Joy_R;
   GPIO_InitStructure_Joy_R.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_InitStructure_Joy_R.GPIO_Pin = (GPIO_Pin_4);
   GPIO_InitStructure_Joy_R.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOC, &GPIO_InitStructure_Joy_R);
        
   GPIO_InitTypeDef GPIO_InitStructure_U;
   GPIO_InitStructure_U.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_InitStructure_U.GPIO_Pin = (GPIO_Pin_8);
   GPIO_InitStructure_U.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOD, &GPIO_InitStructure_U);
        
        GPIO_InitTypeDef GPIO_InitStructure_usart_9;
   GPIO_InitStructure_usart_9.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_InitStructure_usart_9.GPIO_Pin = (GPIO_Pin_9);
   GPIO_InitStructure_usart_9.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure_usart_9);
        
        GPIO_InitTypeDef GPIO_InitStructure_usart_10;
   GPIO_InitStructure_usart_10.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_InitStructure_usart_10.GPIO_Pin = (GPIO_Pin_9);
   GPIO_InitStructure_usart_10.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure_usart_10);
   /*TODO: GPIO EXTILineConfig*/
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource11);
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource3);
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource4);
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource8);

}

void USART_Configure() {
   /*TODO: USART1 configuration*/
        USART_InitTypeDef USART_InitStructure;
        USART_InitStructure.USART_BaudRate = 9600;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_Even;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_Init(USART1, &USART_InitStructure);
   /*TODO: USART1 cmd ENABLE*/
        USART_Cmd(USART1,ENABLE); 
   /*TODO: USART1 IT Config*/
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
}

void EXTI_Configure() {
   /*TODO: EXTI configuration [ mode interrupt ] [Trigger_falling] */
        EXTI_InitTypeDef EXTI_InitStructure_U;
        EXTI_InitStructure_U.EXTI_Line = EXTI_Line11;
        EXTI_InitStructure_U.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure_U.EXTI_Trigger = EXTI_Trigger_Falling;
        EXTI_InitStructure_U.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure_U);
        
        EXTI_InitTypeDef EXTI_InitStructure_S;
        EXTI_InitStructure_S.EXTI_Line = EXTI_Line8;
        EXTI_InitStructure_S.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure_S.EXTI_Trigger = EXTI_Trigger_Falling;
        EXTI_InitStructure_S.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure_S);
        
        EXTI_InitTypeDef EXTI_InitStructure_Left;
        EXTI_InitStructure_Left.EXTI_Line = EXTI_Line3;
        EXTI_InitStructure_Left.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure_Left.EXTI_Trigger = EXTI_Trigger_Falling;
        EXTI_InitStructure_Left.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure_Left);
        
        EXTI_InitTypeDef EXTI_InitStructure_Right;
        EXTI_InitStructure_Right.EXTI_Line = EXTI_Line4;
        EXTI_InitStructure_Right.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure_Right.EXTI_Trigger = EXTI_Trigger_Falling;
        EXTI_InitStructure_Right.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure_Right);
        
  
}

void NVIC_Configure() {
   /*TODO: NVIC_configuration */
   NVIC_InitTypeDef NVIC_InitStructure_U,
                    NVIC_InitStructure_sel,
                    NVIC_InitStructure_Lef,
                    NVIC_InitStructure_Right,
                    NVIC_InitStructure_usart;
   
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
   
   NVIC_InitStructure_U.NVIC_IRQChannel = EXTI15_10_IRQn;
   NVIC_InitStructure_U.NVIC_IRQChannelPreemptionPriority = 0x00;
   NVIC_InitStructure_U.NVIC_IRQChannelSubPriority = 0x00;
   NVIC_InitStructure_U.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure_U);
   
   NVIC_InitStructure_sel.NVIC_IRQChannel = EXTI9_5_IRQn;
   NVIC_InitStructure_sel.NVIC_IRQChannelPreemptionPriority = 0x00;
   NVIC_InitStructure_sel.NVIC_IRQChannelSubPriority = 0x01;
   NVIC_InitStructure_sel.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure_sel);
   
   NVIC_InitStructure_Lef.NVIC_IRQChannel = EXTI3_IRQn;
   NVIC_InitStructure_Lef.NVIC_IRQChannelPreemptionPriority = 0x00;
   NVIC_InitStructure_Lef.NVIC_IRQChannelSubPriority = 0x01;
   NVIC_InitStructure_Lef.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure_Lef);

   NVIC_InitStructure_Right.NVIC_IRQChannel = EXTI4_IRQn;
   NVIC_InitStructure_Right.NVIC_IRQChannelPreemptionPriority = 0x00;
   NVIC_InitStructure_Right.NVIC_IRQChannelSubPriority = 0x01;
   NVIC_InitStructure_Right.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure_Right);
   
   NVIC_InitStructure_usart.NVIC_IRQChannel = USART1_IRQn;
   NVIC_InitStructure_Right.NVIC_IRQChannelPreemptionPriority = 0x00;
   NVIC_InitStructure_Right.NVIC_IRQChannelSubPriority = 0x00;
   NVIC_InitStructure_Right.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure_usart);
   
}

/*TODO: IRQHandler */

void EXTI3_IRQHanlder(void){
  GPIO_SetBits(GPIOD, GPIO_Pin_3);
  if(EXTI_GetITStatus(EXTI_Line3) != RESET){
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3) == Bit_RESET){
      LED1_flag = 0;
      LED2_flag = 1;
      LED_Up=0;
      LED_Down=0;
    }
    EXTI_ClearITPendingBit(EXTI_Line3);
  }
}
void EXTI4_IRQHanlder(void){
  if(EXTI_GetITStatus(EXTI_Line4) != RESET){
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == Bit_RESET){
      LED1_flag = 1;
      LED2_flag = 0;
      LED_Up=0;
      LED_Down=0;
    }
    EXTI_ClearITPendingBit(EXTI_Line4);
  }
}
void EXTI9_5_IRQHanlder(void){
  if(EXTI_GetITStatus(EXTI_Line5) != RESET){
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == Bit_RESET){
      LED1_flag = 0;
      LED2_flag = 0;
      LED_Up=0;
      LED_Down=0;
    }
    EXTI_ClearITPendingBit(EXTI_Line5);
  }
}

void EXTI15_10_IRQHandler(void){
  if(EXTI_GetITStatus(EXTI_Line11) != RESET){
    if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11) == Bit_RESET){
      U_flag = 1;
    }
    EXTI_ClearITPendingBit(EXTI_Line11);
  }
}

void USART1_IRQHandler(void){
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
    data = USART_ReceiveData(USART1);
    if(before_data =='u' && data =='p'){
      LED2_flag = 0;
      LED1_flag = 0;
      LED_Up=1;
      LED_Down=0;
      U_flag = 0;
      data = 0;
    }
    else if(before_data =='d' && data =='n'){
      LED2_flag = 0;
      LED1_flag = 0;
      LED_Up=0;
      LED_Down=1;
      U_flag = 0;
      data = 0;
    }
    before_data = data;
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  }
}

void delay(){
  int i;
  for(i=0; i<100000;i++){}
}
int main() {
   int i;
   SystemInit();
   RCC_Configure();
   GPIO_Configure();
   USART_Configure();
   EXTI_Configure();
   NVIC_Configure();
   char msg[] = "Hello Team08\r\n";
   while(1){
          while(LED1_flag==1&&U_flag==0&&LED_Up==0&& LED_Down==0){
            GPIO_SetBits(GPIOD, GPIO_Pin_2);
            delay();
            GPIO_ResetBits(GPIOD, GPIO_Pin_2);
            delay();
          }
          
          while( LED2_flag==1&&U_flag==0&&LED_Up==0&& LED_Down==0){
            GPIO_SetBits(GPIOD, GPIO_Pin_3);
            delay();
            GPIO_ResetBits(GPIOD, GPIO_Pin_3);
            delay();
          }
          while( LED2_flag==0&&U_flag==0&&LED_Up==1&& LED_Down==0){
            for(LED_Index=3;LED_Index>=0;LED_Index--){
              GPIO_SetBits(GPIOD,LED[LED_Index]);
              delay();
              GPIO_ResetBits(GPIOD,LED[LED_Index]);
            }
          }
          
          while( LED2_flag==0&&U_flag==0&&LED_Up==0&& LED_Down==1){
            for(LED_Index=0;LED_Index<4;LED_Index++){
              GPIO_SetBits(GPIOD,LED[LED_Index]);
              delay();
              GPIO_ResetBits(GPIOD,LED[LED_Index]);
            }
          }
          if(U_flag == 1){
       for(i=0;msg[i]!='\0'; i++){
              USART_SendData(USART1,msg[i]);
              delay();
            }
            U_flag=0;
          }
        }
}