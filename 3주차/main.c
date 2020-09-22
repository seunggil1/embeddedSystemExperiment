#include "stm32f10x.h"
#define GPIO_enable (*(volatile unsigned int*)0x40021018) 
#define GPIOd_BSRR (*(volatile unsigned int*)0x40011410)
#define GPIOd_CRL (*(volatile unsigned int*)0x40011400)
#define GPIOd_ODR (*(volatile unsigned int*)0x4001140C)
#define GPIOc_CRL (*(volatile unsigned int*)0x40011000)
#define GPIOc_IDR (*(volatile unsigned int*)0x40011008)

int main(){
	GPIO_enable = 0x38;
	GPIOd_CRL = 0x10011100;
	GPIOd_BSRR = 0x10011100;
        
        GPIOc_CRL = 0x888800;
        
	while (1) {
          if((GPIOc_IDR & 0x04) == 0 ){
            GPIOd_ODR |= 0x90;  // PD4 7 on
          }
          else if((GPIOc_IDR & 0x08) == 0 ){
            GPIOd_ODR &= ~0x90;  // PD 4 7 off
          }
          else if((GPIOc_IDR & 0x10) == 0 ){
            GPIOd_ODR &= ~0x0C;  // PD 2 3 off
          }
          else if((GPIOc_IDR & 0x20) == 0 ) {
            GPIOd_ODR |= 0x0C;  //  PD 2 3 on
          }
        }
        return 0;
}