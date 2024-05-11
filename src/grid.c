#include "grid.h"

#include <stm32f0xx.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define MAX_RETRIES 1000
#define FIELD_SZ 10


void build_grid_stupid (uint8_t *grid_player){
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


void generate_grid(uint8_t *grid_player) {
    srand(time(NULL));

    for (uint8_t length = 2; length <= 5; length++) {
        for (uint8_t i = 0; i < length; i++) {
            place_ship(grid_player, length);
        }
    }
}

void place_ship(uint8_t *grid_player, uint8_t length) {
    bool placed = false;
    uint8_t retry_counter = 0;

    while (!placed && retry_counter < MAX_RETRIES) {
        uint8_t x = rand() % FIELD_SZ;
        uint8_t y = rand() % FIELD_SZ;
        bool vertical = rand() % 2 == 0;

        if (is_valid_position(grid_player, x, y, length, vertical)) {
            if (vertical) {
                for (uint8_t i = 0; i < length; i++) {
                    grid_player[(y + i) * FIELD_SZ + x] = length;
                }
            } else {
                for (uint8_t i = 0; i < length; i++) {
                    grid_player[y * FIELD_SZ + (x + i)] = length;
                }
            }
            placed = true;
        }
        retry_counter++;
    }
}

bool is_valid_position(uint8_t *grid_player, uint8_t x, uint8_t y, uint8_t length, bool vertical) {
    if (vertical) {
        if (y + length > FIELD_SZ) {
            return false;
        }
        for (uint8_t i = 0; i < length; i++) {
            if (grid_player[(y + i) * FIELD_SZ + x] != 0) {
                return false;
            }
        }
    } else {
        if (x + length > FIELD_SZ) {
            return false;
        }
        for (uint8_t i = 0; i < length; i++) {
            if (grid_player[y * FIELD_SZ + (x + i)] != 0) {
                return false;
            }
        }
    }
    return true;
}


void print_grid(uint8_t *grid_player) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            printf("%d ", grid_player[i * 10 + j]);
        }
        printf("\n");
    }
}

uint8_t grid_checksum (uint8_t *grid_player){
    uint8_t checksum [9];
    for (int x=0;x<9;x++){
        for (int y=0; y<9;y++){
            if (grid_player [y*10+x]!=0){
                checksum[x]=checksum[x]+1;
            }
        }
    }
    return checksum;
}