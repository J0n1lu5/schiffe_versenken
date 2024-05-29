#include <stm32f0xx.h>
#include "epl_clock.h"
#include "epl_usart.h"
#include <stdio.h>
#include <string.h>
#include "grid.h"
#include "attack.h"
#include "game.h"


void init (void){
    // Configure the system clock to 48MHz
    EPL_SystemClock_Config();

    // Enable peripheral  GPIOA clock
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    //set PA0,A9,A1 to output
    GPIOA->MODER |= GPIO_MODER_MODER9_0;
    //GPIOA->MODER &= ~(0b11 << 0);
   // GPIOA->PUPDR |=GPIO_PUPDR_PUPDR0_0;
    GPIOA->MODER |= GPIO_MODER_MODER1_0;
    
    //alle LEDs aus
    GPIOA->ODR  |= GPIO_ODR_9|GPIO_ODR_1|GPIO_ODR_0;

    //blue button
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN; // Aktiviere Clock für GPIOC
    GPIOC->MODER &= ~(0b11 << 13); // Clear MODER for PC13

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
    EPL_init_usart();

    uint8_t rxb;
    static uint8_t state = 0;
    uint8_t grid_player [10*10] = {0};
    uint8_t grid_opponent [10*10] = {0};
    
    bool start_game = false;


    printf("generating grid\n");

    pewpew(1,2);

    build_grid_stupid(&grid_player);
    
    //generate_grid(&grid_player);

    printf("generated new grid\n");

    //printf("Spielfeld:\n");
    print_grid(grid_player);
   
   for(;;){
      char received_string[20]={0};

      USART_receiveString(&received_string,sizeof(received_string));

      state=check_message(received_string);
    }
      
   } 
  

      

