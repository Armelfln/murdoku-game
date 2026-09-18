#ifndef __BOARD_H__
#define __BOARD_H__

#define GRID_SIZE 3 // taille 3 par 3 
#define BOARD_SIZE (GRID_SIZE * GRID_SIZE)
#define NUMBER_OF_CHARACTERS 9

//LES ENUM

typedef enum {
    North_West,
    North,
    North_East,
    West,
    Center,
    East,
    South_West,
    South,
    South_East
} Position_id ;

typedef enum {
    Kitchen,
    Bedroom,
    Living_room,
    Garage,
    Garden,
    Lobby,
    Lounge,
    Cellar,
    Terrace
} Room_id ;

typedef enum {
    Plant,
    Chair,
    Library,
    Table,
    Computer,
    Empty
} Object_id ;

//LES DIFFERENTES STRUCTURES

struct Box;

struct Pos{
    int pos_x ;
    int pos_y ;
};

struct Perso{
    int id_perso ;
    int pos_x ; // c'est le num de la case sur le plateau de 9x9
    int pos_y ;
    int suspect ; // 1 si c est un suspect et 0 si c est la victime 
};

struct Clue; // declaration opaque, definition complete dans clue.h/clue.c

//LES PROTOTYPES DES FONCTIONS

/*Permet de choisir des objets au hasard avec ponderation*/
Object_id random_object(void);

/*Permet d'initialiser un tableau de personnages pas present sur le tableau*/
struct Perso * initialize_tab_perso(void);

/*Permet de liberer le tableau de personnages*/
void tab_perso_free(struct Perso * tab);

/*Permet de placer un personnage sur le plateau*/
int place_a_character(int pos_x, int pos_y, struct Perso * tab, int id, struct Box ** tab_general);

/*Permet de retirer un personnage du plateau (annule un placement)*/
int unplace_a_character(struct Perso * tab, int id, struct Box ** tab_general);

/*Permet de remplir un tableau avec des room dans un ordre random et sans repetition*/
void shuffle_rooms(Room_id *tab, int size);

/*Cree le tableau qui va etre rempli par des room aleatoires sans repetition*/
Room_id *initialize_shuffled_rooms(void);

/*Cree l'echequier general du jeu avec des room et des objet au hasard*/
struct Box ** general_tab(void);

/*Libere l'echequier general du jeu*/
void general_tab_free(struct Box ** board);

/*Choisit un personnage au hasard comme victime*/
struct Perso choose_a_victim(struct Perso * tab_perso);

/*Choisi un personnage au hasard qui n'est pas la victime comme meurtrier*/
struct Perso choose_a_murderer(struct Perso * tab_perso);

/*Retourne le rang de la victime dans le tableau de personnages*/
int find_victim(struct Perso * tab_perso);

/*Retourne le rang du meurtrier dans le tableau de personnages*/
int find_murderer(struct Perso * tab_perso);

/*Retourne la position x d'un personnage*/
int pos_x_perso(struct Perso * tab_perso, int id_perso);

/*Retourne la position y d'un personnage*/
int pos_y_perso(struct Perso * tab_perso, int id_perso);

/*Indique si un personnage est un suspect (1) ou la victime (0)*/
int is_perso_suspect(struct Perso * tab_perso, int id_perso);

/*Retourne l'id d'un personnage*/
int get_id_perso(struct Perso perso);

/*Retourne la room dans laquelle se trouve une case*/
Room_id find_which_room(struct Box ** tab_general, int pos_x, int pos_y);

/*Retourne la position (nord/sud/est/ouest/centre...) d'une case dans sa room*/
Position_id find_which_position(struct Box ** tab_general, int pos_x, int pos_y);

/*Retourne l'objet present sur une case*/
Object_id find_what_object(struct Box ** tab_general, int pos_x, int pos_y);

/*Retourne le personnage present sur une case, NULL si aucun*/
struct Perso * who_perso(struct Box ** tab_general, int pos_x, int pos_y);

/*Indique si une room est vide de tout personnage*/
int is_a_room_empty(struct Box ** tab_general, int pos_x, int pos_y);

/*Place un objet en balayant depuis une position dans une direction donnee, jusqu'a la premiere case libre*/
int place_an_object_oriented(struct Box ** tab_general, Object_id obj_id, int pos_x, int pos_y, Position_id pos_id);

/*Verifie si un indice est respecte par les positions actuelles des personnages*/
int is_the_clue_respected(struct Box ** tab_general, struct Perso * tab, struct Clue * a_clue);

/*retourne un tableau de coordonné de toutes les case de la meme room que la victime*/
struct Pos * the_room_of_the_victim(struct Box ** tab_general, struct Perso * tab_perso);

void erase_the_clue_object(struct Box ** tab_general, struct Perso * tab, struct Clue * a_clue);

#endif