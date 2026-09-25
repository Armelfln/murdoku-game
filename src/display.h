#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "board.h"
#include "raylib.h"

/*Affiche la solution complete du puzzle dans une fenetre raylib*/
void show_solution(struct Box ** board,struct Box ** copy_board, struct Perso * tab_perso, struct Perso * tab_perso_jeu,struct Clue ** tab_clue);

void game_start(struct Box ** board, struct Box ** copy_board, struct Perso * tab_perso, struct Perso * tab_perso_jeu, struct Clue ** tab_clue, Texture2D spritesheet);

int player_win(struct Box ** board, struct Box ** copy_board, struct Perso * tab_perso, struct Perso * tab_perso_jeu);

#endif