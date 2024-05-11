#include <stm32f0xx.h>
#include "mci_clock.h"
#include <stdio.h>
#include <string.h>


#include "grid.h"
#include "attack.h"

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




int main(void){
    init();

    uint8_t rxb;
    static uint8_t state = 0;
    uint8_t grid_player [10*10] = {0};
    uint8_t grid_opponent [10*10];

    printf("generating grid\r\n");

    pewpew(1,2);

    //build_grid_stupid(&grid_player);
    
    generate_grid(&grid_player);

    printf("generated new grid\r\n");

    printf("Spielfeld:\n");
    print_grid(grid_player);
   
    
  

   
    
}
