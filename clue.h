// clue.h
#ifndef __CLUE_H__
#define __CLUE_H__

#include "board.h"

struct Clue{
    int id_perso ;
    Position_id position_id ;
    Object_id obj_id ;
};

// LES PROTOTYPES DES FONCTIONS

/*Cree un indice pour un personnage en placant un objet oriente autour de lui, l'ajoute dans tab_clue*/
int create_a_clue(struct Box ** tab_general, struct Perso * perso, struct Clue ** tab_clue);

/*Initialise un tableau d'indices vide (2 emplacements par personnage)*/
struct Clue ** initialize_tab_clue(void);

/*Libere le tableau d'indices*/
void clue_free(struct Clue ** tab);

/*Verifie que tous les indices sur le plateau sont valide et les regenere sinon*/
int all_clue_good(struct Box ** board, struct Perso * tab, struct Clue ** tab_clue);



#endif