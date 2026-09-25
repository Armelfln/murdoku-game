#include "display.h"
#include "raylib.h"
#include "clue.h"
#include "board.h"
#include <stdio.h>

#define CELL_SIZE 70
#define BOARD_ORIGIN_X 50
#define BOARD_ORIGIN_Y 50

static Color room_colors[9] = {
    (Color){255, 200, 200, 255}, // Kitchen
    (Color){200, 255, 200, 255}, // Bedroom
    (Color){200, 200, 255, 255}, // Living_room
    (Color){255, 255, 200, 255}, // Garage
    (Color){255, 200, 255, 255}, // Garden
    (Color){200, 255, 255, 255}, // Lobby
    (Color){230, 220, 200, 255}, // Lounge
    (Color){220, 200, 230, 255}, // Cellar
    (Color){200, 230, 220, 255}, // Terrace
};

static const char *object_labels[6] = {
    "Plant", "Chair", "Library", "Table", "Computer", ""};

static Rectangle object_sprites[6] = {
    {6 * 16, 1 * 16, 16, 16},  // Plant
    {11 * 16, 9 * 16, 16, 16}, // Chair
    {45 * 16, 5 * 16, 16, 16}, // Library
    {15 * 16, 9 * 16, 16, 16}, // Table
    {16 * 16, 7 * 16, 16, 16}, // Computer
    {0, 0, 0, 0},              // Empty
};
static Rectangle ID_perso[NUMBER_OF_CHARACTERS] = {
    {35 * 16, 17 * 16, 16, 16},
    {36 * 16, 17 * 16, 16, 16},
    {37 * 16, 17 * 16, 16, 16},
    {38 * 16, 17 * 16, 16, 16},
    {39 * 16, 17 * 16, 16, 16},
    {40 * 16, 17 * 16, 16, 16},
    {41 * 16, 17 * 16, 16, 16},
    {42 * 16, 17 * 16, 16, 16},
    {43 * 16, 17 * 16, 16, 16},
};
static Rectangle draw_perso[NUMBER_OF_CHARACTERS + 1] = {
    // {24 * 16, 0 * 16, 16, 16}, // perso 0
    // {24 * 16, 0 * 16, 16, 16}, // perso 0
    // {24 * 16, 0 * 16, 16, 16}, // perso 0
    // {24 * 16, 0 * 16, 16, 16}, // perso 0
    // {24 * 16, 0 * 16, 16, 16}, // perso 0
    // {24 * 16, 0 * 16, 16, 16}, // perso 0
    // {24 * 16, 0 * 16, 16, 16}, // perso 0
    // {24 * 16, 0 * 16, 16, 16}, // perso 0
    // {24 * 16, 0 * 16, 16, 16}, // perso 0
    {35 * 16, 17 * 16, 16, 16},
    {36 * 16, 17 * 16, 16, 16},
    {37 * 16, 17 * 16, 16, 16},
    {38 * 16, 17 * 16, 16, 16},
    {39 * 16, 17 * 16, 16, 16},
    {40 * 16, 17 * 16, 16, 16},
    {41 * 16, 17 * 16, 16, 16},
    {42 * 16, 17 * 16, 16, 16},
    {43 * 16, 17 * 16, 16, 16},
    {24 * 16, 7 * 16, 16, 16}, // victim
};

static const char *position_labels[9] = {
    "Nord-Ouest", "Nord", "Nord-Est",
    "Ouest", "Centre", "Est",
    "Sud-Ouest", "Sud", "Sud-Est"};

void show_solution(struct Box **board, struct Box **copy_board, struct Perso *tab_perso, struct Perso *tab_perso_jeu, struct Clue **tab_clue)
{
    static int b = 1;
    InitWindow(1375, 850, "Murdoku");
    Texture2D spritesheet = LoadTexture("assets/Tilesheet/colored-transparent_packed.png");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        if (b != 0)
        {
            game_start(board, copy_board, tab_perso, tab_perso_jeu, tab_clue, spritesheet);
        }
        else
        {
            for (int i = 0; i < BOARD_SIZE; i++)
            {
                for (int j = 0; j < BOARD_SIZE; j++)
                {
                    Room_id r = find_which_room(board, i, j);
                    Object_id o = find_what_object(board, i, j);

                    int x = BOARD_ORIGIN_X + j * CELL_SIZE;
                    int y = BOARD_ORIGIN_Y + i * CELL_SIZE;
                    DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, room_colors[r]);
                    DrawRectangleLines(x, y, CELL_SIZE, CELL_SIZE, GRAY);

                    if (o != Empty)
                    {
                        Rectangle source_obj = object_sprites[o];
                        Rectangle dest_obj = {x, y, CELL_SIZE, CELL_SIZE};
                        DrawTexturePro(spritesheet, source_obj, dest_obj, (Vector2){0, 0}, 0.0f, (Color){100, 180, 180, 255});
                    }
                    if (who_perso(board, tab_perso, i, j) != -1)
                    {
                        int perso = who_perso(board, tab_perso, i, j);
                        Rectangle source_perso = draw_perso[perso];
                        if (tab_perso[perso].suspect == 0)
                            source_perso = draw_perso[NUMBER_OF_CHARACTERS];
                        Rectangle dest_perso = {x, y, CELL_SIZE, CELL_SIZE};
                        DrawTexturePro(spritesheet, source_perso, dest_perso, (Vector2){0, 0}, 0.0f, (Color){100, 180, 180, 255});
                    }
                }
            }
        }
        for (int k = 0; k <= GRID_SIZE; k++)
        {
            int x = BOARD_ORIGIN_X + k * GRID_SIZE * CELL_SIZE;
            int y = BOARD_ORIGIN_Y + k * GRID_SIZE * CELL_SIZE;

            // ligne verticale
            DrawLineEx((Vector2){x, BOARD_ORIGIN_Y}, (Vector2){x, BOARD_ORIGIN_Y + BOARD_SIZE * CELL_SIZE}, 3, BLACK);
            // ligne horizontale
            DrawLineEx((Vector2){BOARD_ORIGIN_X, y}, (Vector2){BOARD_ORIGIN_X + BOARD_SIZE * CELL_SIZE, y}, 3, BLACK);
        }
        if (b != 0)
        {
            // mode jeu : on propose de voir la solution
            Rectangle solution_button = {BOARD_ORIGIN_X, 700, 200, 50};
            DrawRectangleRec(solution_button, LIGHTGRAY);
            DrawText("Voir la solution", solution_button.x + 10, solution_button.y + 15, 20, BLACK);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), solution_button))
            {
                b = 0;
            }
        }
        else
        {
            // mode solution : on propose de revenir au jeu
            Rectangle back_to_game_button = {BOARD_ORIGIN_X, 700, 200, 50};
            DrawRectangleRec(back_to_game_button, LIGHTGRAY);
            DrawText("Back to game", back_to_game_button.x + 10, back_to_game_button.y + 15, 20, BLACK);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), back_to_game_button))
            {
                b = 1;
            }
        }
        EndDrawing();
    }
    UnloadTexture(spritesheet);
    CloseWindow();
}

void game_start(struct Box **board, struct Box **copy_board, struct Perso *tab_perso, struct Perso *tab_perso_jeu, struct Clue **tab_clue, Texture2D spritesheet)
{
    static int perso_selectionne = -1; // -1 = aucun perso choisi pour l'instant, persiste entre les appels

    if (player_win(board, copy_board, tab_perso, tab_perso_jeu) == 0)
    {
        int x_win = BOARD_SIZE * CELL_SIZE + 2 * BOARD_ORIGIN_X;
        int y_win = BOARD_ORIGIN_Y + 8 * CELL_SIZE;
        int who = find_murderer(tab_perso);
        char texte[64];
        snprintf(texte, sizeof(texte), "YOU WIN. PERSO %d WAS THE MURDERER", who);

        DrawText(texte, x_win - 1, y_win, 25, BLACK);
        DrawText(texte, x_win + 1, y_win, 25, BLACK);
        DrawText(texte, x_win, y_win - 1, 25, BLACK);
        DrawText(texte, x_win, y_win + 1, 25, BLACK);
        DrawText(texte, x_win, y_win, 25, RED);
    }
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            Room_id r = find_which_room(copy_board, i, j);
            Object_id o = find_what_object(copy_board, i, j);

            int x = BOARD_ORIGIN_X + j * CELL_SIZE;
            int y = BOARD_ORIGIN_Y + i * CELL_SIZE;
            DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, room_colors[r]);
            DrawRectangleLines(x, y, CELL_SIZE, CELL_SIZE, GRAY);

            if (o != Empty)
            {
                Rectangle source_obj = object_sprites[o];
                Rectangle dest_obj = {x, y, CELL_SIZE, CELL_SIZE};
                DrawTexturePro(spritesheet, source_obj, dest_obj, (Vector2){0, 0}, 0.0f, (Color){100, 180, 180, 255});
            }
            if (who_perso(copy_board, tab_perso_jeu, i, j) != -1)
            {
                int perso = who_perso(copy_board, tab_perso_jeu, i, j);
                Rectangle source_perso = draw_perso[perso];
                if (tab_perso[perso].suspect == 0)
                    source_perso = draw_perso[NUMBER_OF_CHARACTERS];
                Rectangle dest_perso = {x, y, CELL_SIZE, CELL_SIZE};
                DrawTexturePro(spritesheet, source_perso, dest_perso, (Vector2){0, 0}, 0.0f, (Color){100, 180, 180, 255});
            }
        }
    }

    int x_tab_clue = BOARD_SIZE * CELL_SIZE + 2 * BOARD_ORIGIN_X;
    int y_tab_clue = BOARD_ORIGIN_Y;

    DrawRectangle(x_tab_clue, y_tab_clue, NUMBER_OF_CHARACTERS * CELL_SIZE, CELL_SIZE, (Color){230, 220, 200, 255});
    DrawRectangle(x_tab_clue, y_tab_clue + CELL_SIZE, NUMBER_OF_CHARACTERS * CELL_SIZE, CELL_SIZE, (Color){230, 220, 200, 255});

    for (int p = 0; p < NUMBER_OF_CHARACTERS; p++)
    {
        Rectangle source_perso = draw_perso[p];
        Rectangle source_id_perso = ID_perso[p];
        Rectangle dest_perso = {x_tab_clue + p * CELL_SIZE, y_tab_clue, CELL_SIZE, CELL_SIZE};
        Rectangle dest_id_perso = {x_tab_clue + p * CELL_SIZE, y_tab_clue + CELL_SIZE, CELL_SIZE, CELL_SIZE};
        DrawTexturePro(spritesheet, source_perso, dest_perso, (Vector2){0, 0}, 0.0f, (Color){100, 180, 180, 255});
        DrawTexturePro(spritesheet, source_id_perso, dest_id_perso, (Vector2){0, 0}, 0.0f, (Color){100, 180, 180, 255});
        if (p == perso_selectionne)
        {
            DrawRectangleLinesEx(dest_perso, 4, RED);
        }
    }
    int y_clue = y_tab_clue + 2 * CELL_SIZE + 20;
    for (int c = 0; c < 2 * NUMBER_OF_CHARACTERS; c++)
    {
        if (tab_clue[c] != NULL)
        {
            char texte[64];
            snprintf(texte, sizeof(texte), "Perso %d : %s au %s",
                     tab_clue[c]->id_perso,
                     object_labels[tab_clue[c]->obj_id],
                     position_labels[tab_clue[c]->position_id]);

            DrawText(texte, x_tab_clue - 1, y_clue, 16, BLACK);
            DrawText(texte, x_tab_clue + 1, y_clue, 16, BLACK);
            DrawText(texte, x_tab_clue, y_clue - 1, 16, BLACK);
            DrawText(texte, x_tab_clue, y_clue + 1, 16, BLACK);
            DrawText(texte, x_tab_clue, y_clue, 16, WHITE); // le texte final, par-dessus les 4 copies decalees
            y_clue += 20;
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        int mx = GetMouseX();
        int my = GetMouseY();

        int clue_x = (mx - x_tab_clue) / CELL_SIZE;
        int clue_y = (my - y_tab_clue) / CELL_SIZE;

        if (clue_x >= 0 && clue_x < NUMBER_OF_CHARACTERS && clue_y >= 0 && clue_y < 1)
        {
            perso_selectionne = clue_x;
        }
        else if (perso_selectionne != -1)
        {
            int j = (mx - BOARD_ORIGIN_X) / CELL_SIZE;
            int i = (my - BOARD_ORIGIN_Y) / CELL_SIZE;
            if (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE)
            {
                unplace_a_character(tab_perso_jeu, perso_selectionne, copy_board); // retire l'ancienne position avant de replacer
                place_a_character(i, j, tab_perso_jeu, perso_selectionne, copy_board);
            }
        }
    }
}

int player_win(struct Box **board, struct Box **copy_board, struct Perso *tab_perso, struct Perso *tab_perso_jeu)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            int id_solution = who_perso(board, tab_perso, i, j);
            int id_jeu = who_perso(copy_board, tab_perso_jeu, i, j);
            if (id_solution != id_jeu)
                return -1;
        }
    }
    return 0;
}