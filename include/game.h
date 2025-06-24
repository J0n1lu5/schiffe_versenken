#ifndef GAME_H
#define GAME_H
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "stm32f0xx.h"

uint8_t check_message (const char *received,uint8_t player);
uint8_t check_loss (const uint8_t *enemy_shot_map, const uint8_t size);
uint8_t check_win (const uint8_t *enemy_grid, const uint8_t size);

#endif