#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "core_cm3.h"

int flag = 0;
int flag2 = 0;
char input[2];
void RCC_Configure() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    /*TODO : APB2PeriphClockEnable */
     RCC_APB2PeriphClockCmd( // 사용하고자하는 port들의 clock을 enable 한다.
     RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD
           | RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
}
void GPIO_Configure() {

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
        
	/*TODO: USART1, JoyStick Config */
        // USART TX
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        
        // USART RX
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_InitTypeDef gpB, gpC, gpD, gpD_btn;
        
        // 버튼 PD11
        gpD_btn.GPIO_Mode = GPIO_Mode_IPD;
        gpD_btn.GPIO_Pin = GPIO_Pin_11;
        gpD_btn.GPIO_Speed = GPIO_Speed_50MHz;
        
        // 조이스틱 셀렉트
        gpB.GPIO_Mode = GPIO_Mode_IPD;
        gpB.GPIO_Pin = GPIO_Pin_8;
        gpB.GPIO_Speed = GPIO_Speed_50MHz;
        
        // 조이스틱 right, left
        gpC.GPIO_Mode = GPIO_Mode_IPD;
        gpC.GPIO_Pin = (GPIO_Pin_3 | GPIO_Pin_4);
        gpC.GPIO_Speed = GPIO_Speed_50MHz;
        
        GPIO_Init(GPIOD, &gpD_btn); // PD11 configuration
        GPIO_Init(GPIOB, &gpB); // 조이스틱 SELECT configuration
        GPIO_Init(GPIOC, &gpC); // 조이스틱 Right, Left configuration
        
        // init
	/*TODO: GPIO EXTILineConfig*/
        EXTI_InitTypeDef exti, exti_btn;
        //usarthandler랑 confighandler랑 동시에 들어가서 충돌일어남 
        //GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource10);
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3);
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource11);
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);
        // 인터럽트와 핀을 연결. 해당 핀을 통해 인터럽트
}

void USART_Configure() {
        USART_InitTypeDef usart;
	/*TODO: USART1 configuration*/
        usart.USART_BaudRate = 115200;
        usart.USART_WordLength = USART_WordLength_8b;
        usart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx ;
        usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        usart.USART_Parity = USART_Parity_No;
        usart.USART_StopBits = USART_StopBits_1;
        USART_Init(USART1, &usart);
	/*TODO: USART1 cmd ENABLE*/ 
        USART_Cmd(USART1, ENABLE);
	/*TODO: USART1 IT Config*/
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
        //USART 구조체에 설정한 값을
        //실제 MCU 레지스터에 설정하는 부분
}

void EXTI_Configure() {
	/*TODO: EXTI configuration [ mode interrupt ] [Trigger_falling] */
        EXTI_InitTypeDef exti, exti_btn;
        exti_btn.EXTI_LineCmd = ENABLE;
        exti_btn.EXTI_Mode = EXTI_Mode_Interrupt;
        exti_btn.EXTI_Trigger = EXTI_Trigger_Falling;
        exti_btn.EXTI_Line = (EXTI_Line3 | EXTI_Line4 | EXTI_Line11 |EXTI_Line8);
        EXTI_Init(&exti_btn);
        //3,4,11,8 버튼에 해당하는 인터럽트를 사용
}

void NVIC_Configure() {
	/*TODO: NVIC_configuration */
        NVIC_InitTypeDef nvic, nvic_btn, nvic_sel, nvic_left,nvic_right;

       //각 port의 n번 핀은 EXTI n에 연결된다
       NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
       nvic.NVIC_IRQChannel = USART1_IRQn;
       nvic.NVIC_IRQChannelCmd = ENABLE;
       nvic.NVIC_IRQChannelPreemptionPriority = 0;
       nvic.NVIC_IRQChannelSubPriority = 0;
       NVIC_Init(&nvic);
       //우선순위 설정 및 USART 인터럽트를 사용하기 위해 설정
           
       // 각 버튼 및 조이스틱 인터럽트를 사용하기 위해 설정
       nvic_btn.NVIC_IRQChannel = EXTI15_10_IRQn;
       nvic_btn.NVIC_IRQChannelCmd = ENABLE;
       nvic_btn.NVIC_IRQChannelPreemptionPriority = 0;
       nvic_btn.NVIC_IRQChannelSubPriority =0;
       NVIC_Init(&nvic_btn);

       nvic_sel.NVIC_IRQChannel = EXTI9_5_IRQn;
       nvic_sel.NVIC_IRQChannelCmd = ENABLE;
       nvic_sel.NVIC_IRQChannelPreemptionPriority = 0;
       nvic_sel.NVIC_IRQChannelSubPriority = 0;
       NVIC_Init(&nvic_sel);
       
       nvic_left.NVIC_IRQChannel = EXTI3_IRQn;
       nvic_left.NVIC_IRQChannelCmd = ENABLE;
       nvic_left.NVIC_IRQChannelPreemptionPriority = 0;
       nvic_left.NVIC_IRQChannelSubPriority = 0;
       NVIC_Init(&nvic_left);
       
       nvic_right.NVIC_IRQChannel = EXTI4_IRQn;
       nvic_right.NVIC_IRQChannelCmd = ENABLE;
       nvic_right.NVIC_IRQChannelPreemptionPriority = 0;
       nvic_right.NVIC_IRQChannelSubPriority = 0;
       NVIC_Init(&nvic_right);
}

void delay(int a){
  for(int i = 0; i < a; i++);
}

void LED2(){
  GPIO_SetBits(GPIOD, GPIO_Pin_3);
  delay(500000);
  GPIO_ResetBits(GPIOD, GPIO_Pin_3);
  delay(500000);
}
void LED1(){
  GPIO_SetBits(GPIOD, GPIO_Pin_2);
  delay(500000);
  GPIO_ResetBits(GPIOD, GPIO_Pin_2);
  delay(500000);
}
void turnOff(){
  GPIO_ResetBits(GPIOD, GPIO_Pin_3);
  GPIO_ResetBits(GPIOD, GPIO_Pin_2);
}
void SendData(uint16_t data) {
    /* Transmit Data */
   USART1->DR = data;

   /* Wait till TC is set */
   while ((USART1->SR & USART_SR_TC) == 0);
}

void printTeam(){
  SendData('T');
  SendData('E');
  SendData('A');
  SendData('M');
  SendData('0');
  SendData('3');
  SendData('\r');
  SendData('\n');
}

void printUP(){
       GPIO_SetBits(GPIOD, GPIO_Pin_7);
      delay(500000);
      GPIO_ResetBits(GPIOD, GPIO_Pin_7);
      GPIO_SetBits(GPIOD, GPIO_Pin_4);
      delay(500000);
      GPIO_ResetBits(GPIOD, GPIO_Pin_4);
      GPIO_SetBits(GPIOD, GPIO_Pin_3);
      delay(500000);
      GPIO_ResetBits(GPIOD, GPIO_Pin_3);
      GPIO_SetBits(GPIOD, GPIO_Pin_2);
      delay(500000);
      GPIO_ResetBits(GPIOD, GPIO_Pin_2);  
}

void printDN(){
      GPIO_SetBits(GPIOD, GPIO_Pin_2);
      delay(500000);
      GPIO_ResetBits(GPIOD, GPIO_Pin_2);
      GPIO_SetBits(GPIOD, GPIO_Pin_3);
      delay(500000);
      GPIO_ResetBits(GPIOD, GPIO_Pin_3);
      GPIO_SetBits(GPIOD, GPIO_Pin_4);
      delay(500000);
      GPIO_ResetBits(GPIOD, GPIO_Pin_4);
      GPIO_SetBits(GPIOD, GPIO_Pin_7);
      delay(500000);
      GPIO_ResetBits(GPIOD, GPIO_Pin_7);
}
/*TODO: IRQHandler */
void USART1_IRQHandler(void) {
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
    char get;
    get = USART_ReceiveData(USART1);
    if(get == 'u')
      input[0]=get;
    if(get == 'p' && input[0] == 'u')
      input[1]=get;  
    if(get == 'd')
      input[0]=get;
    if(get == 'n' && input[0] == 'd')
      input[1]=get;
    if(input[0] == 'u' && input[1] == 'p')
      flag = 3;
    if(input[0] == 'd' && input[1] == 'n')
      flag = 4;
  }
}
void EXTI3_IRQHandler(void) { //left 누르면 led2()
  if (EXTI_GetFlagStatus(EXTI_Line3) != RESET) {
      flag = 2;
      EXTI_ClearITPendingBit(EXTI_Line3);  
  }
}
void EXTI4_IRQHandler(void) { //right 누르면  led1()
  if (EXTI_GetFlagStatus(EXTI_Line4) != RESET) { 
      flag = 1;
      EXTI_ClearITPendingBit(EXTI_Line4);  
  }
}
void EXTI9_5_IRQHandler(void) { //select 누르면 turnoff()
   if (EXTI_GetFlagStatus(EXTI_Line8) != RESET) {
      flag = 0;
      EXTI_ClearITPendingBit(EXTI_Line8);  
  }
}
void EXTI15_10_IRQHandler(void)  { //PD11 누르면 team03 출력
  if(EXTI_GetFlagStatus(EXTI_Line11)  != RESET){
    flag2 = 11;
    EXTI_ClearITPendingBit(EXTI_Line11);
  }
}

int main() {
  SystemInit();
  RCC_Configure();
  GPIO_Configure();
  USART_Configure();
  EXTI_Configure();
  NVIC_Configure();

  SendData('\0');
  
  while(1){
    if(flag == 1)
      LED1();
    if(flag == 2)
      LED2();
    if(flag == 0)
      turnOff();
    if(flag == 3)
      printUP();
    if(flag == 4)
      printDN();
    if(flag2 == 11){
      printTeam();
      flag2=0;
    }
  };
}

