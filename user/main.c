#include "stm32f10x.h"
#include "core_cm3.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "misc.h"
#include "lcd.h"
#include "touch.h"

//#define ADC1_DR_Address ((u32)0x4001244C)
ADC_
int color[12]={WHITE,CYAN,BLUE,RED,MAGENTA,LGRAY,GREEN,YELLOW,BROWN,BRRED,GRAY};
int i = 0;
volatile uint32_t adc_value[1];

void GPIO_Configure() {

   GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_InitTypeDef PA3;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_2); //LED
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOD, &GPIO_InitStructure);
   
   PA3.GPIO_Mode = GPIO_Mode_AIN;
   PA3.GPIO_Pin = GPIO_Pin_3; //ADC
   GPIO_Init(GPIOA,&PA3);

}

void RCC_Configure(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    /*TODO : APB2PeriphClockEnable */
     RCC_APB2PeriphClockCmd( // 사용하고자하는 port들의 clock을 enable 한다.
     RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD | RCC_APB2Periph_ADC1, ENABLE); //ADC1 레퍼런스 146p에 있음
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

void DMA_Configure(void){
    DMA_InitTypeDef DMA_InitStructure;
    DMA_DeInit(DMA1_Channel1); // DMA 채널 1 reset
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
    
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&adc_value[0];
    
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 1;       //한개의 값을 입력받음
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word; // uint32_t는 4byte, Data 사이즈는 word
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; // 지속적인 값 update
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    /* Enable DMA1 channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);

} 
void ADC_Configure(void){
   
   ADC_InitTypeDef ADCch3;

   ADCch3.ADC_Mode = ADC_Mode_Independent;
   ADCch3.ADC_ScanConvMode = ENABLE;
   ADCch3.ADC_ContinuousConvMode = ENABLE;
   ADCch3.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
   ADCch3.ADC_DataAlign = ADC_DataAlign_Right;
   ADCch3.ADC_NbrOfChannel = 1;
   ADC_RegularChannelConfig(ADC1,ADC_Channel_3,1,ADC_SampleTime_239Cycles5);
   ADC_Init(ADC1,&ADCch3);

   ADC_DMACmd(ADC1, ENABLE);
   ADC_Cmd(ADC1,ENABLE);
  
   ADC_ResetCalibration(ADC1);
   while(ADC_GetResetCalibrationStatus(ADC1));

   ADC_StartCalibration(ADC1);
   while(ADC_GetCalibrationStatus(ADC1));

   ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


int main(){
   int adc = 0;
   SystemInit();
  RCC_Configure();
  GPIO_Configure();
  ADC_Configure(); 
   DMA_Configure();
   
   LCD_Init();
   LCD_Clear(WHITE);
   
   while(1){
      adc = adc_value[0]; //조도센서값 받아오기
      i++;
      if(i == 1000){
        i = 0;
        LCD_ShowNum(50, 130, adc, 4, RED, WHITE); //폴링 방식
      }
      
     if(adc  > 4000){
        GPIO_SetBits(GPIOD, GPIO_Pin_2);
     }
     else{
      GPIO_ResetBits(GPIOD, GPIO_Pin_2);
     }
      
     
   }
   
}