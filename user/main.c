#include <stdbool.h>

#define RCC_APB2 (*(volatile unsigned int*)0x40021018) // RCC_APB2 clock enable register 

#define GPIOd_CRL (*(volatile unsigned int*)0x40011400) // GPIO D port configuration register 
#define GPIOd_CRH (*(volatile unsigned int*)0x40011404) // GPIO D port configuration register 
#define GPIOd_ODR (*(volatile unsigned int*)0x4001140C) //  GPIO D output register
#define GPIOd_IDR (*(volatile unsigned int*)0x40011408)

#define GPIOc_CRL (*(volatile unsigned int*)0x40011000) // GPIO C port configuration register
#define GPIOc_CRH (*(volatile unsigned int*)0x40011004) // GPIO C port configuration register
#define GPIOc_IDR (*(volatile unsigned int*)0x40011008) // GPIO C input register
#define GPIOc_BSRR (*(volatile unsigned int*)0x40011010) // GPIO C input register

void delay(){
  int i;
  for(i=0 ; i < 10000000;i++){}
}

int main(){

   RCC_APB2 = 0x30;             // port C,D clock enable 설정
   
   GPIOd_CRL = 0x10011100;      //LED 2347 사용설정 
   GPIOd_CRH = 0x88000;           //User input PD11,12  input설정
   
   GPIOc_CRL = 0x888800;        // 조이스틱 2345 사용설정   
   GPIOc_CRH = 0x11;        // 릴레이 모듈 PC 8,9 사용설정      

   bool state11 = false;
   bool state12 = false;
   
   
   while (1) {
          if((GPIOc_IDR & 0x04) == 0 ){       // 조이스틱이 down인 경우 prot C의 2GPIO C input register번 임으로 0x04로 & 해서 알 수 있다.
            GPIOd_ODR &= ~ 0x9C;           // PortD 2,3,4,7  off     
          }
           else if((GPIOc_IDR & 0x08) == 0 ){    // 조이스틱이 Left인 경우 
            GPIOc_BSRR |= ~0x200; 
          }
          else if((GPIOc_IDR & 0x10) == 0 ){     // 조이스틱이 Right인 경우 
            GPIOc_BSRR |= ~0x100;
          }
          else if((GPIOc_IDR & 0x20) == 0 ) {    // 조이스틱이 UP인 경우 
            GPIOc_BSRR |= ~0x300;
          }
          else{
            GPIOc_BSRR = 0x300;
          }
          
          if((GPIOd_IDR & 0x800) == 0)
            state11 = !state11;
          if((GPIOd_IDR & 0x1000) == 0)
            state12 = !state12;
          
          if(state11)  //user input  PD11
            GPIOd_ODR |= 0x0C;  //  PortD 2, 3 on
          if(state12)
            GPIOd_ODR |= 0x90;  //  PortD 4, 7 on
 
          delay();
          GPIOd_ODR &= ~0x0C;  //  PortD 2, 3 off
          GPIOd_ODR &= ~0x90;  //  PortD 4, 7 off
          delay();
        }
    
    
        return 0;
}