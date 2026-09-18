#include "board.h"
#include "clue.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

struct Box{
    Object_id obj_id ;
    Room_id room_id ;
    struct Perso * perso ;
    Position_id position_id ;
};


int object_weights[6] = {
    5,  // Plant
    5,  // Chair
    5,  // Library
    5,  // Table
    5,  // Computer
    75  // Empty - grande majorite des cases vides
};

Object_id random_object(void){
    int total = 0;
    for (int i = 0; i < 6; i++) total += object_weights[i];
    int r = rand() % total ;
    int cumulative = 0 ;
    for (int i = 0; i < 6; i++){
        cumulative += object_weights[i];
        if (r < cumulative) return (Object_id) i ;
    }
    return Empty ;
}

struct Perso * initialize_tab_perso(void){ //retourne un tableau de perso pas encore place sur le plateau 
    struct Perso * tab = malloc(NUMBER_OF_CHARACTERS* sizeof(struct Perso)) ;
    for (int i = 0 ; i < NUMBER_OF_CHARACTERS ; i++){
        tab[i].id_perso = i ;
        tab[i].pos_x = -1 ;
        tab[i].pos_y = -1 ; // pas encore sur le plateau
        tab[i].suspect = 1 ;
    }
    return tab ;
}

int place_a_character(int pos_x, int pos_y, struct Perso * tab, int id, struct Box ** tab_general){
    if(tab_general[pos_x][pos_y].obj_id != Empty) // on verifie que la case n'est pas occupee par un objet
        return -1 ;
    if(tab_general[pos_x][pos_y].perso != NULL) // on verifie qu'aucun perso n'occupe deja la case
        return -1 ;
    for(int i = 0 ; i < NUMBER_OF_CHARACTERS ; i++){ // on verifie que aucun perso n'est dans la colonne et ligne
        if((pos_x == tab[i].pos_x && i != id) || (pos_y == tab[i].pos_y && i != id))
            return -1 ;
    }
    tab[id].pos_x = pos_x ;
    tab[id].pos_y = pos_y ;
    tab_general[pos_x][pos_y].perso = &tab[id] ; // on lie la case au perso qui l'occupe
    return 0 ; 
}

void shuffle_rooms(Room_id *tab, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1); // indice aléatoire entre 0 et i inclus
        // échange tab[i] et tab[j]
        Room_id temp = tab[i];
        tab[i] = tab[j];
        tab[j] = temp;
    }
}

Room_id *initialize_shuffled_rooms(void) {
    Room_id *tab = malloc(BOARD_SIZE * sizeof(Room_id));
    for (int i = 0; i < BOARD_SIZE; i++) {
        tab[i] = i; // remplissage initial dans l'ordre (Kitchen=0, Bedroom=1, ...)
    }
    shuffle_rooms(tab, BOARD_SIZE);
    return tab;
}
//-1 pour case vide, 0 pour objet, 1 pour perso 
struct Box ** general_tab(void){
    Room_id *shuffled = initialize_shuffled_rooms() ; // un seul mélange, une seule fois

    struct Box ** tab = malloc(BOARD_SIZE * sizeof(struct Box*)) ;
    for(int i = 0 ; i < BOARD_SIZE ; i++){
        tab[i] = malloc(BOARD_SIZE * sizeof(struct Box)) ;
    }

    for(int i = 0 ; i < BOARD_SIZE ; i++){
        for(int j = 0 ; j < BOARD_SIZE ; j++){
            int room_index = (i / GRID_SIZE) * GRID_SIZE + (j / GRID_SIZE) ;
            int position_index = (i % GRID_SIZE) * GRID_SIZE + (j % GRID_SIZE) ;
            tab[i][j].room_id = shuffled[room_index] ;
            tab[i][j].position_id = (Position_id) position_index ;
            tab[i][j].obj_id = Empty ;//random_object() ; // a supp car on va remplir le tableau d'objet de perso dans un second temps
            tab[i][j].perso = NULL ;
        }
    }

    free(shuffled) ; // plus besoin, chaque case a déjà sa valeur copiée
    return tab ;
}

void general_tab_free(struct Box ** board){
    for(int i = 0 ; i < BOARD_SIZE ; i++){
        free(board[i]) ; // libère chaque ligne (un bloc de BOARD_SIZE Box)
    }
    free(board) ; // libère le tableau de pointeurs
}

void tab_perso_free(struct Perso * tab){ // free le tableau de perso 
    free(tab) ;
}

struct Perso choose_a_victim(struct Perso * tab_perso){
    int victim_id = rand()%NUMBER_OF_CHARACTERS ;
    tab_perso[victim_id].suspect = 0 ;
    return tab_perso[victim_id] ;
}

struct Perso choose_a_murderer(struct Perso * tab_perso){
    int murderer_id = rand()%NUMBER_OF_CHARACTERS ;
    while( tab_perso[murderer_id].suspect != 1){
        murderer_id = rand()%NUMBER_OF_CHARACTERS ;
    }
    tab_perso[murderer_id].suspect = 2 ; // 2 c'est pour le meurtrier
    return tab_perso[murderer_id] ;
}

int find_victim(struct Perso * tab_perso){
    for(int i = 0 ; i < NUMBER_OF_CHARACTERS ; i++){
        if(tab_perso[i].suspect == 0)
            return i ;
    }
    return -1;
}

int find_murderer(struct Perso * tab_perso){
    for(int i = 0 ; i < NUMBER_OF_CHARACTERS ; i++){
        if(tab_perso[i].suspect == 2)
            return i ;
    }
    return -1;
}

int pos_x_perso(struct Perso * tab_perso, int id_perso){
    return tab_perso[id_perso].pos_x ;
}

int pos_y_perso(struct Perso * tab_perso, int id_perso){
    return tab_perso[id_perso].pos_y ;
}

int is_perso_suspect(struct Perso * tab_perso, int id_perso){
    return tab_perso[id_perso].suspect == 1 ;
}

int get_id_perso(struct Perso perso){
    return perso.id_perso ;
}

Room_id find_which_room(struct Box ** tab_general, int pos_x, int pos_y){
    return tab_general[pos_x][pos_y].room_id ;
}

Position_id find_which_position(struct Box ** tab_general, int pos_x, int pos_y){
    return tab_general[pos_x][pos_y].position_id ;
}

Object_id find_what_object(struct Box ** tab_general, int pos_x, int pos_y){
    return tab_general[pos_x][pos_y].obj_id ;
}

struct Perso * who_perso(struct Box ** tab_general, int pos_x, int pos_y){
    return tab_general[pos_x][pos_y].perso ;
}
//fonction a utiliser pour placer les suspect
int is_a_room_empty(struct Box ** tab_general, int pos_x, int pos_y){ // fonction a verifier claude
    for(int i = 0 ; i < BOARD_SIZE ; i++){
        for(int j = 0 ; j < BOARD_SIZE ; j++){
            if((tab_general[i][j].room_id == tab_general[pos_x][pos_y].room_id) && tab_general[i][j].perso != NULL)
                return 0 ; // retourne faux
        }
    }
    return 1 ;
}
//position id c est l'orientation est ouest ect 
int place_an_object_oriented(struct Box ** tab_general, Object_id obj_id, int pos_x, int pos_y, Position_id pos_id){
    if(pos_id == Center){
        tab_general[pos_x][pos_y].obj_id = obj_id ;
        return 1 ;
    }
    if(pos_id == North_West){
        int k = pos_x -1;
        int p = pos_y -1;
        while(k >= 0 && p >= 0 && (tab_general[k][p].obj_id != Empty || tab_general[k][p].perso != NULL)){
            k-- ;
            p-- ;
        }
        if(k < 0 || p < 0)
            return 0 ;
        tab_general[k][p].obj_id = obj_id ;
        return 1 ;
    }
    if(pos_id == North){
        int k = pos_x -1;
        int p = pos_y ;
        while(k >= 0 && (tab_general[k][p].obj_id != Empty || tab_general[k][p].perso != NULL)){
            k-- ;
        }
        if(k < 0)
            return 0 ;
        tab_general[k][p].obj_id = obj_id ;
        return 1 ;
    }
    if(pos_id == North_East){
        int k = pos_x +1 ;
        int p = pos_y -1 ;
        while(k < BOARD_SIZE && p >= 0 && (tab_general[k][p].obj_id != Empty || tab_general[k][p].perso != NULL)){
            k++ ;
            p-- ;
        }
        if(k == BOARD_SIZE || p < 0)
            return 0 ;
        tab_general[k][p].obj_id = obj_id ;
        return 1 ;
    }
    if(pos_id == West){
        int k = pos_x ;
        int p = pos_y -1 ;
        while(p >= 0 && (tab_general[k][p].obj_id != Empty || tab_general[k][p].perso != NULL)){
            p-- ;
        }
        if(p < 0)
            return 0 ;
        tab_general[k][p].obj_id = obj_id ;
        return 1 ;
    }
    if(pos_id == East){
        int k = pos_x +1 ;
        int p = pos_y ;
        while(k < BOARD_SIZE && (tab_general[k][p].obj_id != Empty || tab_general[k][p].perso != NULL)){
            k++ ;
        }
        if(k == BOARD_SIZE)
            return 0 ;
        tab_general[k][p].obj_id = obj_id ;
        return 1 ;
    }
    if(pos_id == South_West){
        int k = pos_x -1 ;
        int p = pos_y +1 ;
        while(k >= 0 && p < BOARD_SIZE && (tab_general[k][p].obj_id != Empty || tab_general[k][p].perso != NULL)){
            k-- ;
            p++ ;
        }
        if(k < 0 || p == BOARD_SIZE)
            return 0 ;
        tab_general[k][p].obj_id = obj_id ;
        return 1 ;
    }
    if(pos_id == South){
        int k = pos_x +1;
        int p = pos_y ;
        while(k < BOARD_SIZE && (tab_general[k][p].obj_id != Empty || tab_general[k][p].perso != NULL)){
            k++ ;
        }
        if(k == BOARD_SIZE)
            return 0 ;
        tab_general[k][p].obj_id = obj_id ;
        return 1 ;
    }
    if(pos_id == South_East){
        int k = pos_x +1;
        int p = pos_y +1;
        while(k < BOARD_SIZE && p < BOARD_SIZE && (tab_general[k][p].obj_id != Empty || tab_general[k][p].perso != NULL)){
            k++ ;
            p++ ;
        }
        if(k == BOARD_SIZE || p == BOARD_SIZE)
            return 0 ;
        tab_general[k][p].obj_id = obj_id ;
        return 1 ;
    }
    return 0 ;
}

int unplace_a_character(struct Perso * tab, int id, struct Box ** tab_general){
    if(tab[id].pos_x == -1 || tab[id].pos_y == -1) // le perso n'est pas placé, rien a annuler
        return 0 ;

    tab_general[tab[id].pos_x][tab[id].pos_y].perso = NULL ;
    tab[id].pos_x = -1 ;
    tab[id].pos_y = -1 ;
    return 1 ;
}

int is_the_clue_respected(struct Box ** tab_general, struct Perso * tab, struct Clue * a_clue){
    int pos_x = tab[a_clue->id_perso].pos_x ;
    int pos_y = tab[a_clue->id_perso].pos_y ;

    if(a_clue->position_id == Center){
        return tab_general[pos_x][pos_y].obj_id == a_clue->obj_id ;
    }
    if(a_clue->position_id == North_West){
        int k = pos_x -1;
        int p = pos_y -1;
        while(k >= 0 && p >= 0 && tab_general[k][p].obj_id == Empty){
            k-- ;
            p-- ;
        }
        if(k < 0 || p < 0)
            return 0 ;
        return tab_general[k][p].obj_id == a_clue->obj_id ;
    }
    if(a_clue->position_id == North){
        int k = pos_x -1;
        int p = pos_y ;
        while(k >= 0 && tab_general[k][p].obj_id == Empty){
            k-- ;
        }
        if(k < 0)
            return 0 ;
        return tab_general[k][p].obj_id == a_clue->obj_id ;
    }
    if(a_clue->position_id == North_East){
        int k = pos_x +1 ;
        int p = pos_y -1 ;
        while(k < BOARD_SIZE && p >= 0 && tab_general[k][p].obj_id == Empty){
            k++ ;
            p-- ;
        }
        if(k == BOARD_SIZE || p < 0)
            return 0 ;
        return tab_general[k][p].obj_id == a_clue->obj_id ;
    }
    if(a_clue->position_id == West){
        int k = pos_x ;
        int p = pos_y -1 ;
        while(p >= 0 && tab_general[k][p].obj_id == Empty){
            p-- ;
        }
        if(p < 0)
            return 0 ;
        return tab_general[k][p].obj_id == a_clue->obj_id ;
    }
    if(a_clue->position_id == East){
        int k = pos_x +1 ;
        int p = pos_y ;
        while(k < BOARD_SIZE && tab_general[k][p].obj_id == Empty){
            k++ ;
        }
        if(k == BOARD_SIZE)
            return 0 ;
        return tab_general[k][p].obj_id == a_clue->obj_id ;
    }
    if(a_clue->position_id == South_West){
        int k = pos_x -1 ;
        int p = pos_y +1 ;
        while(k >= 0 && p < BOARD_SIZE && tab_general[k][p].obj_id == Empty){
            k-- ;
            p++ ;
        }
        if(k < 0 || p == BOARD_SIZE)
            return 0 ;
        return tab_general[k][p].obj_id == a_clue->obj_id ;
    }
    if(a_clue->position_id == South){
        int k = pos_x +1;
        int p = pos_y ;
        while(k < BOARD_SIZE && tab_general[k][p].obj_id == Empty){
            k++ ;
        }
        if(k == BOARD_SIZE)
            return 0 ;
        return tab_general[k][p].obj_id == a_clue->obj_id ;
    }
    if(a_clue->position_id == South_East){
        int k = pos_x +1;
        int p = pos_y +1;
        while(k < BOARD_SIZE && p < BOARD_SIZE && tab_general[k][p].obj_id == Empty){
            k++ ;
            p++ ;
        }
        if(k == BOARD_SIZE || p == BOARD_SIZE)
            return 0 ;
        return tab_general[k][p].obj_id == a_clue->obj_id ;
    }
    return 0 ;
}

// Room_id find_which_room(struct Box ** tab_general, int pos_x, int pos_y){
//     return tab_general[pos_x][pos_y].room_id ;
// }

struct Pos * the_room_of_the_victim(struct Box ** tab_general, struct Perso * tab_perso){
    int i = find_victim(tab_perso) ;
    Room_id room_id = find_which_room(tab_general, tab_perso[i].pos_x, tab_perso[i].pos_y) ;

    struct Pos * pos = malloc(9 * sizeof(struct Pos)) ;
    int k = 0 ;
    for(int p = 0 ; p < BOARD_SIZE ; p++){
        for(int q = 0 ; q < BOARD_SIZE ; q++){
            if(tab_general[p][q].room_id == room_id){
                pos[k].pos_x = p ;
                pos[k].pos_y = q ;
                k++ ;
            }
        }
    }
    return pos ;
}
void erase_the_clue_object(struct Box ** tab_general, struct Perso * tab, struct Clue * a_clue){

    int pos_x = tab[a_clue->id_perso].pos_x ;
    int pos_y = tab[a_clue->id_perso].pos_y ;

    if(a_clue->position_id == Center){
        tab_general[pos_x][pos_y].obj_id = Empty ;
        return ;
    }
    if(a_clue->position_id == North_West){
        int k = pos_x -1;
        int p = pos_y -1;
        while(k >= 0 && p >= 0 && tab_general[k][p].obj_id == Empty){
            k-- ;
            p-- ;
        }
        if(k < 0 || p < 0)
            return ; // rien trouve, rien a effacer
        tab_general[k][p].obj_id = Empty ;
        return ;
    }
    if(a_clue->position_id == North){
        int k = pos_x -1;
        int p = pos_y ;
        while(k >= 0 && tab_general[k][p].obj_id == Empty){
            k-- ;
        }
        if(k < 0)
            return ;
        tab_general[k][p].obj_id = Empty ;
        return ;
    }
    if(a_clue->position_id == North_East){
        int k = pos_x +1 ;
        int p = pos_y -1 ;
        while(k < BOARD_SIZE && p >= 0 && tab_general[k][p].obj_id == Empty){
            k++ ;
            p-- ;
        }
        if(k == BOARD_SIZE || p < 0)
            return ;
        tab_general[k][p].obj_id = Empty ;
        return ;
    }
    if(a_clue->position_id == West){
        int k = pos_x ;
        int p = pos_y -1 ;
        while(p >= 0 && tab_general[k][p].obj_id == Empty){
            p-- ;
        }
        if(p < 0)
            return ;
        tab_general[k][p].obj_id = Empty ;
        return ;
    }
    if(a_clue->position_id == East){
        int k = pos_x +1 ;
        int p = pos_y ;
        while(k < BOARD_SIZE && tab_general[k][p].obj_id == Empty){
            k++ ;
        }
        if(k == BOARD_SIZE)
            return ;
        tab_general[k][p].obj_id = Empty ;
        return ;
    }
    if(a_clue->position_id == South_West){
        int k = pos_x -1 ;
        int p = pos_y +1 ;
        while(k >= 0 && p < BOARD_SIZE && tab_general[k][p].obj_id == Empty){
            k-- ;
            p++ ;
        }
        if(k < 0 || p == BOARD_SIZE)
            return ;
        tab_general[k][p].obj_id = Empty ;
        return ;
    }
    if(a_clue->position_id == South){
        int k = pos_x +1;
        int p = pos_y ;
        while(k < BOARD_SIZE && tab_general[k][p].obj_id == Empty){
            k++ ;
        }
        if(k == BOARD_SIZE)
            return ;
        tab_general[k][p].obj_id = Empty ;
        return ;
    }
    if(a_clue->position_id == South_East){
        int k = pos_x +1;
        int p = pos_y +1;
        while(k < BOARD_SIZE && p < BOARD_SIZE && tab_general[k][p].obj_id == Empty){
            k++ ;
            p++ ;
        }
        if(k == BOARD_SIZE || p == BOARD_SIZE)
            return ;
        tab_general[k][p].obj_id = Empty ;
        return ;
    }
}