#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include "board.h"
#include "clue.h"
#include "solve.h"

int main(){
    srand(time(NULL)) ;
    struct Box ** board = general_tab() ;
    struct Perso * tab_perso = initialize_tab_perso() ;
    choose_a_victim(tab_perso) ;
    for(int i = 0 ; i < NUMBER_OF_CHARACTERS ; i++){
        int k = 0 ;
        while(place_a_character(rand()%BOARD_SIZE,rand()%BOARD_SIZE, tab_perso, i, board) != 0 && k < 100){
            k++ ;
        }
        if(k == 100)
            printf("les personnages n'ont pas pu etre place avec succes") ;
    }
    struct Clue ** tab_clue = initialize_tab_clue() ;
    for(int i = 0 ; i < NUMBER_OF_CHARACTERS ; i++){
        create_a_clue(board, &tab_perso[i], tab_clue) ;
        create_a_clue(board, &tab_perso[i], tab_clue) ; // deuxième indice pour le même perso
    }
    int ok = all_clue_good(board, tab_perso, tab_clue) ;
    if(!ok)
        printf("stabilisation des indice a echoue") ;

    for(int i = 0 ; i < NUMBER_OF_CHARACTERS ; i++){
        unplace_a_character(tab_perso, i, board) ;
    }
    int nb_solutions = solver(board, tab_perso, tab_clue, 0, 2) ;
    printf("Nombre de solutions trouvees (max 2) : %d\n", nb_solutions) ;       


    for(int i = 0 ; i < NUMBER_OF_CHARACTERS ; i++){
        printf("Perso %d : position (%d, %d), suspect=%d\n", i, tab_perso[i].pos_x, tab_perso[i].pos_y, tab_perso[i].suspect) ;
    }

    for(int i = 0 ; i < 2*NUMBER_OF_CHARACTERS ; i++){
        if(tab_clue[i] != NULL){
            printf("Indice %d : perso %d, direction %d, objet %d\n", i, tab_clue[i]->id_perso, tab_clue[i]->position_id, tab_clue[i]->obj_id) ;
        } 
        else {
            printf("Indice %d : absent\n", i) ;
        }
    }

    general_tab_free(board) ;
    tab_perso_free(tab_perso) ;
    clue_free(tab_clue) ;

    return 0 ;

}