#include "stm32f10x.h"
#include "core_cm3.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "misc.h"
#include "lcd.h"
#include "touch.h"

int color[12]={WHITE,CYAN,BLUE,RED,MAGENTA,LGRAY,GREEN,YELLOW,BROWN,BRRED,GRAY};
int i = 0;
uint16_t adc_value;
uint16_t pos_x,pos_y;
uint16_t pix_x,pix_y;
int count=0;
int start=0;

void GPIO_Configure() {

   GPIO_InitTypeDef GPIO_InitStructure;
   
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_2 ); //LED1
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOD, &GPIO_InitStructure);

}

void RCC_Configure(void){
    /* TIM2 Clock Enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    /*TODO : APB2PeriphClockEnable */
     RCC_APB2PeriphClockCmd( // 사용하고자하는 port들의 clock을 enable 한다.
     RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOD, ENABLE); 
}

void NVIC_Configure(void){
      NVIC_InitTypeDef nvic;
      nvic.NVIC_IRQChannel = TIM2_IRQn;
      nvic.NVIC_IRQChannelPreemptionPriority = 0;
      nvic.NVIC_IRQChannelSubPriority = 1;
      nvic.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&nvic);
}

void TIM2_IRQHandler(void){
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    count+=start;
    if((count/100)%2 == 1){ //count가 10ms. count가 100이되면 1초. 따라서 100나누면 1s 
      GPIO_SetBits(GPIOD, GPIO_Pin_2);//1초될때마다 LED1 켜짐
    }
    else if( (count/100)%2 == 0){
      GPIO_ResetBits(GPIOD, GPIO_Pin_2); 
    }
  }
}

void TIM_Configuration(void){
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  //강의자료 6p 보고 설정
  TIM_TimeBaseStructure.TIM_Period = 10000 -1; 
  TIM_TimeBaseStructure.TIM_Prescaler = 72-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0; // clockdivision 1이 0x00 임
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  
  TIM_Cmd(TIM2, ENABLE);
  
  TIM_ARRPreloadConfig(TIM2, ENABLE);
  
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

int main(){
    SystemInit();
    RCC_Configure();
    GPIO_Configure();
    NVIC_Configure();  
    TIM_Configuration();    
   LCD_Init();
   Touch_Configuration();
   Touch_Adjust();
   LCD_Clear(WHITE);
   LCD_ShowString(50,50,"Tue_Team03", BLACK, WHITE); //팀 출력
   LCD_ShowString(30,120, "1s    1/10s    1/100s",BLACK, WHITE);
   
   //상자(버튼) 그리기
   LCD_DrawRectangle(10, 250, 70, 280); 
   LCD_ShowString(20,260, "Start", BLACK, WHITE);
   LCD_DrawRectangle(80, 250, 140, 280);
   LCD_ShowString(90,260, "Stop", BLACK, WHITE);
   LCD_DrawRectangle(150, 250, 210, 280);
   LCD_ShowString(170,260, "Reset", BLACK, WHITE);
   
   while(1){
     LCD_ShowNum(0, 90, count/100,4,BLACK, WHITE); //초는 네자리 출력
     LCD_ShowNum(90, 90, count/10,1, BLACK, WHITE); //한자리수
      LCD_ShowNum(130, 90, count,1, BLACK, WHITE); //한자리수

      Touch_GetXY(&pos_x,&pos_y,0); //터치 값 받아오기
      Convert_Pos(pos_x, pos_y,&pix_x,&pix_y); //변수에 넣기  

      
      if(10 < pix_x && pix_x < 70 && 250 < pix_y && pix_y < 280){ //START
          start = 1;
      }
      else if(80 < pix_x && pix_x < 140 && 250 < pix_y && pix_y < 280){ //STOP
          start = 0;
      }
      else if(150 < pix_x && pix_x < 210 && 250 < pix_y && pix_y < 280){ //RESET
          count = 0;
          start = 0;
          GPIO_ResetBits(GPIOD, GPIO_Pin_2);
      }
      
   }
}