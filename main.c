#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include "board.h"
#include "clue.h"
#include "solve.h"

int generate_puzzle(void){
    struct Box ** board = general_tab() ;
    struct Perso * tab_perso = initialize_tab_perso() ;
    choose_a_victim(tab_perso) ;
    choose_a_murderer(tab_perso) ;

    if(place_a_character(rand()%BOARD_SIZE,rand()%BOARD_SIZE, tab_perso, find_victim(tab_perso), board) != 0){
        general_tab_free(board) ;
        tab_perso_free(tab_perso) ;
        return 1 ;
    }

    struct Pos * room_of_victim = the_room_of_the_victim(board, tab_perso) ;
    int range = rand()%9 ;
    int k_murderer = 0 ;
    while(place_a_character(room_of_victim[range].pos_x, room_of_victim[range].pos_y, tab_perso, find_murderer(tab_perso), board) != 0 && k_murderer < 100){
        range = rand()%9 ;
        k_murderer++ ;
    }
    free(room_of_victim) ;
    if(k_murderer == 100){
        general_tab_free(board) ;
        tab_perso_free(tab_perso) ;
        return 1 ;
    }

    for(int i = 0 ; i < NUMBER_OF_CHARACTERS ; i++){
        int k = 0 ;
        if(tab_perso[i].suspect == 1){
            while(place_a_character(rand()%BOARD_SIZE,rand()%BOARD_SIZE, tab_perso, i, board) != 0 && k < 100){
                k++ ;
            }
            if(k == 100){
                general_tab_free(board) ;
                tab_perso_free(tab_perso) ;
                return 1 ;
            }
        }
    }

    struct Clue ** tab_clue = initialize_tab_clue() ;
    for(int i = 0 ; i < NUMBER_OF_CHARACTERS ; i++){
        create_a_clue(board, &tab_perso[i], tab_clue) ;
        create_a_clue(board, &tab_perso[i], tab_clue) ;
    }

    int ok = all_clue_good(board, tab_perso, tab_clue) ;
    if(!ok){
        general_tab_free(board) ;
        tab_perso_free(tab_perso) ;
        clue_free(tab_clue) ;
        return 1 ;
    }

    for(int i = 0 ; i < NUMBER_OF_CHARACTERS ; i++){
        unplace_a_character(tab_perso, i, board) ;
    }

    int nb_solutions = solver(board, tab_perso, tab_clue, 0, 2) ;

    general_tab_free(board) ;
    tab_perso_free(tab_perso) ;
    clue_free(tab_clue) ;

    if(nb_solutions != 1)
        return 1 ;

    return 0 ;
}

int main(void){
    srand(time(NULL)) ;

    int result = 1 ;
    int attempts = 0 ;
    while(result != 0 && attempts < 1000){
        result = generate_puzzle() ;
        attempts++ ;
    }

    if(result != 0){
        printf("echec : impossible de generer un puzzle valide apres %d tentatives\n", attempts) ;
        return 1 ;
    }

    printf("puzzle genere avec succes apres %d tentative(s)\n", attempts) ;
    return 0 ;
}