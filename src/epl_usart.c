#include "main.h"
#include "epl_clock.h"
#include "epl_usart.h"


#define Baudrate 9600


void EPL_init_usart(){

    /*This is a static implementation of the USART2 peripheral*/

    // Use 4 bits for 'priority' and 0 bits for 'subpriority'.
    NVIC_SetPriorityGrouping( 0 );
    // UART receive interrupts should be high priority.
    uint32_t uart_pri_encoding = NVIC_EncodePriority( 0, 1, 0 );
    NVIC_SetPriority( USART2_IRQn, uart_pri_encoding );
    NVIC_EnableIRQ( USART2_IRQn );

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
    USART2->BRR = (APB_FREQ / Baudrate);
    // Enable the UART using the CR1 register
    USART2->CR1 |= ( USART_CR1_RE | USART_CR1_TE | USART_CR1_UE | USART_CR1_RXNEIE);
    
}





void USART_receiveString(char *buffer, uint16_t maxSize) {
    uint16_t i = 0;
    while (i < maxSize - 1) {
        while (!(USART2->ISR & USART_ISR_RXNE));
        char c = USART2->RDR;
        buffer[i++] = c;
        if (c == '\n') {
            break;
        }
    }
    
}

void USART_receive(char* buffer,uint8_t size){

    for (int i=0;i<size-1;i++){    
                    char received_char = USART2->RDR;
                    buffer[i] = received_char;
                    i++;
                    if (received_char == '\n'){
                    LOG("%s",buffer);
                     
            break;
        }
                    
    }
}

