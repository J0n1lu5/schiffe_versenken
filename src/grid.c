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
    srand(684534);
    uint8_t j=0;
    for (uint8_t length = 5; length >= 2; length--) {
        
        uint8_t ammount;

           if (length==2) ammount=  4;
           if (length==3) ammount=  3;
           if (length==4) ammount=  2;
           if (length==5) ammount=  1;

        for (uint8_t i = 0; i < ammount; i++) {
            printf("%d",place_ship(grid_player, length));
            printf("\nplaced ship %d\n",j);
            print_grid(grid_player);
            j++;
        }
    }
}

uint16_t place_ship(uint8_t *grid_player, uint8_t length) {
    bool placed = false;
    uint16_t retry_counter = 0;
    

    while (!placed && retry_counter < MAX_RETRIES) {
        printf("%d\n",retry_counter);
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
return retry_counter;    
}

/*
bool is_valid_position (uint8_t *grid_player,uint8_t x, uint8_t y,uint8_t length, bool vertical){
    //ship within bolunderies 
    if (vertical){
        if (y + length > FIELD_SZ){
            return false;
        }  
    }else {
        if (x + length > FIELD_SZ){
            return false;
        }
    }

    //ship colliding with other ship
    if (vertical){
        for (uint8_t i = 0; i < length; i++) {
            if (grid_player[(y+i) * FIELD_SZ + x] != 0) return false;
        }
    }else{
        for (uint8_t i = 0; i < length; i++){
            if (grid_player[y * FIELD_SZ + x+i] != 0) return false;
        }
    }

    //ship to close to sides
    //irgendwoo in dem teil scheiße ist
    if (vertical){
        if (y-1>=0){
            for (uint8_t i=0;i<length+2;i++){
                if(i+y-1>FIELD_SZ) continue;
                if (x+1<=FIELD_SZ){
                    if (grid_player[((y-1)+i)*FIELD_SZ+x+1]!=0) return false;
                }
                if(x-1>=0){
                    if (grid_player[((y-1)+i)*FIELD_SZ+x-1]!=0) return false;
                }
            }
        }else{
            for (uint8_t i=0;i<length+2;i++){
                if(i+y>FIELD_SZ) continue;
                if (x+1<=FIELD_SZ){
                    if (grid_player[(y+i)*FIELD_SZ+x+1]!=0) return false;
                }
                if(x-1>=0){
                    if (grid_player[(y+i)*FIELD_SZ+x-1]!=0) return false;
                }
            }
        }
    }else{
        if (x-1>=0){
            for (uint8_t i=0;i<length+2;i++){
                if(i+x-1>FIELD_SZ) continue;
                if (x+1<=FIELD_SZ){
                    if (grid_player[(y+1)*FIELD_SZ+((x-1)+i)]!=0) return false;
                }
                if(x-1>=0){
                    if (grid_player[(y-1)*FIELD_SZ+((x-1)+i)]!=0) return false;
                }
            }
        }else{
            for (uint8_t i=0;i<length+1;i++){
                if(i+x>FIELD_SZ) continue;
                if (y+1<=FIELD_SZ){
                    if (grid_player[(y+1)*FIELD_SZ+((x-1)+i)]!=0) return false;
                }
                if(y-1>=0){
                    if (grid_player[(y-i)*FIELD_SZ+((x-1)+i)]!=0) return false;
                }
            }
        }
    }

    //ships to close on ends
    if (vertical){
        if (y+length!=FIELD_SZ){
            if (grid_player[y+length+1*FIELD_SZ+x]!=0) return false;
        }
        if (y>=0){
            if (grid_player[y-1*FIELD_SZ+x]!=0) return false;
        }
    }else{
        if (x+length!=FIELD_SZ){
            if (grid_player[y*FIELD_SZ+x+length+1]!=0) return false;
        }
        if (x>=0){
            if (grid_player[y*FIELD_SZ+x-1]!=0) return false;
        }
    }
    
    return true;
}

*/

bool is_valid_position(uint8_t *grid_player, uint8_t x, uint8_t y, uint8_t length, bool vertical) {
    // Überprüfen, ob das Schiff innerhalb der Spielfeldgrenzen liegt
    if (vertical) {
        if (y + length > FIELD_SZ) {
            return false; // Vertikale Platzierung überschreitet die Spielfeldgrenzen
        }
    } else {
        if (x + length > FIELD_SZ) {
            return false; // Horizontale Platzierung überschreitet die Spielfeldgrenzen
        }
    }

    // Überprüfen, ob das Schiff mit anderen Schiffen kollidiert
    if (vertical) {
        for (uint8_t i = 0; i < length; i++) {
            if (grid_player[(y + i) * FIELD_SZ + x] != 0) {
                return false; // Kollision mit einem anderen Schiff
            }
        }
    } else {
        for (uint8_t i = 0; i < length; i++) {
            if (grid_player[y * FIELD_SZ + (x + i)] != 0) {
                return false; // Kollision mit einem anderen Schiff
            }
        }
    }

    // Überprüfen, ob das Schiff zu nah an anderen Schiffen liegt
    if (vertical) {
        for (int i = -1; i < length + 1; i++) {
            if (x - 1 >= 0 && y + i >= 0 && y + i < FIELD_SZ && grid_player[(y + i) * FIELD_SZ + (x - 1)] != 0) {
                return false; // Zu nah an der linken Seite
            }
            if (x + 1 < FIELD_SZ && y + i >= 0 && y + i < FIELD_SZ && grid_player[(y + i) * FIELD_SZ + (x + 1)] != 0) {
                return false; // Zu nah an der rechten Seite
            }
        }
    } else {
        for (int i = -1; i < length + 1; i++) {
            if (y - 1 >= 0 && x + i >= 0 && x + i < FIELD_SZ && grid_player[(y - 1) * FIELD_SZ + (x + i)] != 0) {
                return false; // Zu nah an der oberen Seite
            }
            if (y + 1 < FIELD_SZ && x + i >= 0 && x + i < FIELD_SZ && grid_player[(y + 1) * FIELD_SZ + (x + i)] != 0) {
                return false; // Zu nah an der unteren Seite
            }
        }
    }

    return true; // Position ist gültig
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
    uint8_t checksum [10];
    for (int x=0;x<10;x++){
        for (int y=0; y<10;y++){
            if (grid_player [y*10+x]!=0){
                checksum[x]=checksum[x]+1;
            }
        }
    }
    return checksum;
}