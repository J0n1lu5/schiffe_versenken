#include "attack.h"

#include <stm32f0xx.h>
#include <stdbool.h>
#include <stdlib.h>

void ranndom_attack(uint8_t *grid_opponent){
    srand(684534);
    uint8_t x = rand()%10;
    uint8_t y = rand()%10;
    if (is_smart_shot(grid_opponent,x,y)) pewpew(x,y);
    
}

bool is_smart_shot (uint8_t *grid_opponent,uint8_t x, uint8_t y){
    if()
}

void pewpew (uint8_t x,uint8_t y){
    printf("BOOM%d%d\n",x,y);
}