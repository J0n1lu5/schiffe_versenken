#include <stm32f0xx.h>
#include "mci_clock.h"
#include <stdio.h>

#define DEBUG

// This is a simple macro to print debug messages of DEBUG is defined
#ifdef DEBUG
  #define LOG( msg... ) printf( msg );
#else
  #define LOG( msg... ) ;
#endif

// Select the Baudrate for the UART
#define BAUDRATE 9600


void init (){
    // Configure the system clock to 48MHz
    EPL_SystemClock_Config();

    
    // Enable clock for GPIOC and ADC
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;

    // Set PC0 as analog
    GPIOC->MODER |= GPIO_MODER_MODER0;

    // Enable ADC
    ADC1->CR |= ADC_CR_ADEN;
    while(!(ADC1->ISR & ADC_ISR_ADRDY));

    // Select channel 10
    ADC1->CHSELR = ADC_CHSELR_CHSEL10;


    // Enable peripheral  GPIOA clock
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    // Enable peripheral  USART2 clock
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // Configure PA2 as USART2_TX using alternate function 1
    GPIOA->MODER |= GPIO_MODER_MODER2_1;
    GPIOA->AFR[0] |= 0b0001 << (4*2);


    // Configure PA3 as USART2_RX using alternate function 1
    GPIOA->MODER |= GPIO_MODER_MODER3_1;
    GPIOA->AFR[0] |= 0b0001 << (4*3);

    // Configure the UART Baude rate Register 
    USART2->BRR = (APB_FREQ / BAUDRATE);
    // Enable the UART using the CR1 register
    USART2->CR1 |= ( USART_CR1_RE | USART_CR1_TE | USART_CR1_UE );

    //set PA0,A9,A1 to output
    GPIOA->MODER |= GPIO_MODER_MODER9_0;
    GPIOA->MODER |= GPIO_MODER_MODER0_0;
    GPIOA->MODER |= GPIO_MODER_MODER1_0;
    
    //alle LEDs aus
    GPIOA->ODR  |= GPIO_ODR_9|GPIO_ODR_1|GPIO_ODR_0;
}

// For supporting printf function we override the _write function to redirect the output to UART
int _write( int handle, char* data, int size ) {
    int count = size;
    while( count-- ) {
        while( !( USART2->ISR & USART_ISR_TXE ) ) {};
        USART2->TDR = *data++;
    }
    return size;
}

int putint32(int32_t num) {
    // Schleife über die Bytes des 32-Bit-Ganzzahls
    for (int i = 0; i < 4; ++i) {
        // Byte extrahieren und übertragen
        int8_t byte = (num >> (i * 8)) & 0xFF;
        while (!(USART2->ISR & USART_ISR_TXE)); // warten bis zur Übertragungsmöglichkeit
        USART2->TDR = byte; // Byte übertragen
    }
    return num;
    }

 int putint32s(const int32_t *nums, int count) {
    // Schleife über die 32-Bit-Ganzzahlen
    for (int i = 0; i < count; ++i) {
        putint32(nums[i]); // 32-Bit-Ganzzahl senden
    }
    return count;
    }


  void RGBControl (uint8_t *rxb){
    
            // Print the data to the console using the LOG macro which is defined above and calls printf which uses _write, which is overriden to redirect the output to UART
            //LOG("[DEBUG-LOG]: %d\r\n", *rxb );
            //LOG("%d\r\n",rxb)
            //USART2->TDR = 1;


            if (*rxb==114) {               //R key
              GPIOA->ODR &= ~GPIO_ODR_9; //LED an
              GPIOA->ODR  |= GPIO_ODR_1|GPIO_ODR_0; //LED aus

            }
            else if (*rxb==98)  {          //B key
              GPIOA->ODR &= ~GPIO_ODR_0; //LED an
              GPIOA->ODR  |= GPIO_ODR_9|GPIO_ODR_1; //LED aus
            }
            else if (*rxb==103) {          //G key
              GPIOA->ODR &= ~GPIO_ODR_1; //LED an
              GPIOA->ODR  |= GPIO_ODR_9|GPIO_ODR_0; //LED aus
            }
            else {
              GPIOA->ODR  |= GPIO_ODR_9|GPIO_ODR_1|GPIO_ODR_0; //alle LEDs aus
            }

  }

  void ADCReadout (int *adcTemp){ 
      
          // Start ADC conversion
        ADC1->CR |= ADC_CR_ADSTART;

        // Wait for end of conversion
        while(!(ADC1->ISR & ADC_ISR_EOC));

        // Read ADC value
        uint32_t adcValue = ADC1->DR;       

        if (*adcTemp != adcValue) {
        *adcTemp=adcValue;
        // Print ADC value
        LOG("ADC value: %d\r\n", *adcTemp);

        }
        
       

  }


int main(void){
    init();

  uint8_t rxb;
  static uint8_t state = 0;
  int adcTemp = 0;
  //bool newinput = false;

  while(1){
    if(USART2->ISR & USART_ISR_RXNE){
      rxb = USART2->RDR;
      LOG("[DEBUG-LOG]: %d\r\n", rxb );
    }
    if (rxb == 49) {
      state = 1;
    }
    if (rxb == 50) {
      state = 2;
    }

    if (state == 1){
      RGBControl(&rxb);
    }

    if (state ==2){
      ADCReadout(&adcTemp);
    }

  }

   
    
}
