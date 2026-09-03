#ifndef __BOARD_H__
#define __BOARD_H__


#define GRID_SIZE 3 // taille 3 par 3 
#define BOARD_SIZE (GRID_SIZE * GRID_SIZE)

//LES DIFFERENTES STRUCTURES
struct Room ; // a definir dans le .c
struct Object ; // les objet du jeu exemple une chaise (ca prend une case)

//LES PROTOTYPES DES FONCTIONS
struct Object * initialize_object(void); // initialisation d'un objet 
struct Room * initialize_room(void);
struct Room ** initialize_board(void);

#endif



