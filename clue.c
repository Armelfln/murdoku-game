#include "clue.h"
#include "board.h"
#include <stdlib.h>
#include <stdio.h>

int create_a_clue(struct Box ** tab_general, struct Perso * perso, struct Clue ** tab_clue){ // tab_clue doit etre de taille 2*NUMBER_OF_CHARACTERS
    struct Clue * a_clue = malloc(sizeof(struct Clue)) ;
    a_clue->id_perso = perso->id_perso ;

    int k = 0 ;
    int obj, dir ;
    do {
        obj = rand()%6 ;
        dir = rand()%8 ;
        if(dir >= 4) dir++ ; // saute la valeur 4 (Center)
        a_clue->obj_id = (Object_id) obj ;
        a_clue->position_id = (Position_id) dir ;
        k++ ;
    } while(!place_an_object_oriented(tab_general, a_clue->obj_id, perso->pos_x, perso->pos_y, a_clue->position_id) && k < 10) ;

    if(k == 10){
        free(a_clue) ;
        return 0 ;
    }
    if(tab_clue[perso->id_perso] == NULL)
        tab_clue[perso->id_perso] = a_clue ;
    else
        tab_clue[perso->id_perso + NUMBER_OF_CHARACTERS] = a_clue ;
    return 1 ;
}

struct Clue ** initialize_tab_clue(void){
    struct Clue ** tab = malloc(2* NUMBER_OF_CHARACTERS *sizeof(struct Clue *)) ;
    for (int i = 0 ; i < 2* NUMBER_OF_CHARACTERS ;i++){
        tab[i] = NULL ;
    }

    return tab ;
}

void clue_free(struct Clue ** tab){
    for(int i = 0 ; i < 2* NUMBER_OF_CHARACTERS ; i++){
        free(tab[i]) ;
    }
    free(tab) ;
}

int all_clue_good(struct Box ** board, struct Perso * tab, struct Clue ** tab_clue){
    int p = 0 ;
    int all_ok = 0 ;

    while(!all_ok && p < 10){
        all_ok = 1 ;

        for(int i = 0 ; i < 2*NUMBER_OF_CHARACTERS ; i++){
            if(tab_clue[i] != NULL && !is_the_clue_respected(board, tab, tab_clue[i])){
                all_ok = 0 ;

                int id_perso_concerne = tab_clue[i]->id_perso ;

                erase_the_clue_object(board, tab, tab_clue[i]) ; // efface l'ancien objet AVANT de liberer le clue

                free(tab_clue[i]) ;
                tab_clue[i] = NULL ;

                int q = 0 ;
                while(create_a_clue(board, &tab[id_perso_concerne], tab_clue) == 0 && q < 10){
                    q++ ;
                }
            }
        }
        p++ ;
    }

    if(p == 10)
        return 0 ;
    return 1 ;
}

