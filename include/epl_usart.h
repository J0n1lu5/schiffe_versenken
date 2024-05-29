#ifndef EPL_USART_H_
#define EPL_USART_H_
#include <string.h>
#include <stdlib.h>





void EPL_init_usart();

void receive_string_from_usart(char* buffer, int max_length);

#ifdef DEBUG
  #define LOG( msg... ) printf( msg );
#else
  #define LOG( msg... ) ;
#endif





#endif /* EPL_USART_H_ */
