#include "board.h"
#include <stdlib.h>

struct Object{
    int a ; // pour le moment on va dire ya un objet a=1 ou yen a pas a=0
};

struct Room {
    int position ; // on positionne la room sur le tableau general de 9 place (9 room)
    struct Object * object[GRID_SIZE][GRID_SIZE] ; // ya 9 cases dans chaque piece 
};


struct Object * initialize_object(void){ // initialisation d'un objet 
    struct Object * obj = malloc(sizeof(struct Object)) ;
    obj->a = 0 ;
    return obj ;
}

struct Room * initialize_room(void){
    struct Room * room = malloc(sizeof(struct Room)) ;
    for (int i = 0 ; i < GRID_SIZE ; i++){
        for (int j = 0 ; j < GRID_SIZE ; j++){
            room->object[i][j] = initialize_object() ;
        }
    }
    return room ;
}

struct Room ** initialize_board(){
    struct Room ** board = malloc(BOARD_SIZE*sizeof(struct Room*)) ; // un tableau de 9 pointeur vers des room
    for (int i = 0 ; i < BOARD_SIZE ; i++){
        board[i] = initialize_room() ;
        board[i]->position = i ;
    }
    return board ;
}
