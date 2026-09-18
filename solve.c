#include "board.h"
#include "solve.h"
#include<stdlib.h>

int solver(struct Box ** board, struct Perso * tab_perso, struct Clue ** tab_clue, int id_perso, int limite){

    if( id_perso == NUMBER_OF_CHARACTERS) // tous les perso ont ete place avec succes
        return 1 ;

    int total = 0 ;

    for(int i = 0 ; i < BOARD_SIZE ; i++){
        for(int j = 0 ; j < BOARD_SIZE ; j++){
            if(place_a_character(i,j,tab_perso, id_perso, board) == 0){

                int clue1_ok = (tab_clue[id_perso] == NULL) || is_the_clue_respected(board, tab_perso, tab_clue[id_perso]) ;
                int clue2_ok = (tab_clue[id_perso + NUMBER_OF_CHARACTERS] == NULL) || is_the_clue_respected(board, tab_perso, tab_clue[id_perso + NUMBER_OF_CHARACTERS]) ;

                if(clue1_ok && clue2_ok){
                    total += solver(board, tab_perso, tab_clue, id_perso+1, limite) ;
                }

                unplace_a_character(tab_perso, id_perso, board) ;

                if(total >= limite)
                    return total ; // arret anticipe, pas la peine d'explorer plus
            }
        }
    }
    return total ;

}