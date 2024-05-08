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

void build_grid (uint8_t *grid_player){
    //spielfeld erstellen static
    //zeile 0
    grid_player [0*10+0]=2;
    grid_player [0*10+1]=2;
    grid_player [0*10+4]=2;
    grid_player [0*10+5]=2;
    grid_player [0*10+7]=2;
    grid_player [0*10+8]=2;
    //zeile2
    grid_player [2*10+5]=3;
    grid_player [2*10+6]=3;
    grid_player [2*10+7]=3;
    //zeile 3
    grid_player [3*10+0]=3;
    grid_player [3*10+1]=3;
    grid_player [3*10+2]=3;
    //Zeile 4
    grid_player [4*10+5]=3;
    grid_player [4*10+6]=3;
    grid_player [4*10+7]=3;
    //zeile 5
    grid_player [5*10+0]=4;
    grid_player [5*10+1]=4;
    grid_player [5*10+2]=4;
    grid_player [5*10+3]=4;
    //zeile 6
    grid_player [6*10+6]=4;
    grid_player [6*10+7]=4;
    grid_player [6*10+8]=4;
    grid_player [6*10+9]=4;
    //ziele 8
    grid_player [8*10+5]=5;
    grid_player [8*10+6]=5;
    grid_player [8*10+7]=5;
    grid_player [8*10+8]=5;
    grid_player [8*10+9]=5;
    //zeile 9
    grid_player [9*10+1]=2;
    grid_player [9*10+2]=2;
}

int grid_checksum (){

}

void pewpew (){

}

int main(void){
    init();

  uint8_t rxb;
  static uint8_t state = 0;
  uint8_t grid_player [100];
  uint8_t grid_opponent [9][9];


   
    
}
