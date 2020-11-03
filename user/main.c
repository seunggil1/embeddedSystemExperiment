#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "misc.h"
#include "lcd.h"
#include "Touch.h"

int color[12]={WHITE,CYAN,BLUE,RED,MAGENTA,LGRAY,GREEN,YELLOW,BROWN,BRRED,GRAY};

void GPIO_Configure() {

   GPIO_InitTypeDef GPIO_InitStructure;

   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7);
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOD, &GPIO_InitStructure);
        
  
}

void RCC_Configure(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    /*TODO : APB2PeriphClockEnable */
     RCC_APB2PeriphClockCmd( // 사용하고자하는 port들의 clock을 enable 한다.
     RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD
           | RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOE, ENABLE);
}

        
void ADC_Configure(void){
   GPIO_InitTypeDef PA3;
   ADC_InitTypeDef ADCch3;

   PA3.GPIO_Pin = GPIO_Pin_3;
   PA3.GPIO_Mode = GPIO_Mode_AIN;
   GPIO_Init(GPIOA,&PA3);

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

   ADCch3.ADC_Mode = ADC_Mode_Independent;
   ADCch3.ADC_ScanConvMode = DISABLE;
   ADCch3.ADC_ContinuousConvMode = ENABLE;
   ADCch3.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
   ADCch3.ADC_DataAlign = ADC_DataAlign_Right;
   ADCch3.ADC_NbrOfChannel = 3;
   ADC_RegularChannelConfig(ADC1,ADC_Channel_3,1,ADC_SampleTime_55Cycles5);
   ADC_Init(ADC1,&ADCch3);

   ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);

   ADC_Cmd(ADC1,ENABLE);

   ADC_ResetCalibration(ADC1);
   while(ADC_GetResetCalibrationStatus(ADC1));

   ADC_StartCalibration(ADC1);
   while(ADC_GetCalibrationStatus(ADC1));

   ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void NVIC_Configure(void){
      NVIC_InitTypeDef nvic;
      nvic.NVIC_IRQChannel = ADC1_2_IRQn;
      nvic.NVIC_IRQChannelPreemptionPriority = 0;
      nvic.NVIC_IRQChannelSubPriority = 0;
      nvic.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&nvic);
}


void ADC1_2_IRQHandler(void) {
      int adc_value;
      uint16_t pos_x,pos_y;
         uint16_t pix_x,pix_y;

      while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);
      adc_value = ADC_GetConversionValue(ADC1);
      LCD_ShowNum(50, 110, adc_value, 4, BLACK, WHITE);
      ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);

      Touch_GetXY(&pos_x,&pos_y,1);
      Convert_Pos(pos_x, pos_y,&pix_x,&pix_y);

      LCD_ShowNum(80, 70, pix_x, 3, BLACK, WHITE);
      LCD_ShowNum(80, 90, pix_y, 3, BLACK, WHITE);

      if(pix_x<5){
         pix_x = 5;
      } else if(pix_x>235){
         pix_x = 235;
      }

      if(pix_y<5){
         pix_y = 5;
      } else if(pix_y>315){
         pix_y = 315;
      }

      LCD_DrawRectangle(pix_x-5,pix_y-5,pix_x+5,pix_y+5);
}


int main(){
    uint16_t pos_x,pos_y;
    uint16_t pix_x,pix_y;
        
    int count=0,touch=0;
    SystemInit();
    RCC_Configure();
    GPIO_Configure();
    ADC_Configure();
    NVIC_Configure();  
        
   LCD_Init();
   Touch_Configuration();
   Touch_Adjust();
   LCD_Clear(WHITE);
    
        
   while(1){
      LCD_ShowString(50,50,"Tue_Team03", BLACK, WHITE);
      Touch_GetXY(&pos_x,&pos_y,1);
      Convert_Pos(pos_x, pos_y,&pix_x,&pix_y);
      Draw_Big_Point(pix_x,pix_y);
      if(touch ==10){
         LCD_Fill(0,0,240,320,color[count]);
         touch = 0;
      }
      count++;
      touch++;
      count = count%11;
      
   }
}
