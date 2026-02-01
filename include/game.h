#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
#include "entity.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 20
#define PLAYER_SPEED 400.0f

#define BULLET_WIDTH 10
#define BULLET_HEIGHT 20
#define BULLET_SPEED 600.0f

#define ENEMIES_ON_LINE 8
#define ENEMIES_SPEED_MAX 80.0f
#define DISTANCE_ENTRE_ENNEMIS_X 10
#define DISTANCE_ENTRE_ENNEMIS_Y 20
#define ENEMIES_WIDTH 50
#define ENEMIES_HEIGHT 30
#define NBR_DE_LIGNES_MAX_HORDE 20

#define SPEEDX_ENNEMI_RAPIDE 100
#define SPEEDY_ENNEMI_RAPIDE 200

#define VIE_WIDTH 15


bool init(SDL_Window **window, SDL_Renderer **renderer);
void handle_input(bool *running, const Uint8 *keys, Entity *player, Entity *bullet, bool *bullet_active);
void update(Entity *player, Entity *bullet, Entity *bullet_enemy, bool *bullet_enemy_active, bool *bullet_active, float dt);
void detect_collision_enemy(Entity *bullet, Horde *horde, bool *bullet_active, int *score);
void render(SDL_Renderer *renderer, Entity *player, Entity *bullet, Horde *horde, Entity *bullet_enemy, bool bullet_active, bool bullet_enemy_active, float vies, Entity *fast, bool fast_actif, bool vie_tombe, Entity *vie_tombante);
void cleanup(SDL_Window *window, SDL_Renderer *renderer);
void update_horde(Horde *horde, float time, float dt, bool *droite);
Horde* initial_horde();
void update_fast_enemy(Entity *fast, bool *fast_actif, int *cote_fast, bool *fast_droite, bool *bullet_active, Entity *bullet, float dt, int *vies);
void end_game(Horde *horde, Entity *player, bool *running, int vies, int score, bool *victoire);
void vies_qui_tombent(int *vies, Entity *vie_tombante, bool *vie_tombe, Entity *player, float dt);
void attack_horde(Horde *horde, Entity *player, Entity *bullet_enemy, bool *bullet_enemy_active, int *vies);
void free_horde(Horde* horde);
void afficher_resultat(SDL_Renderer *renderer, bool victoire);

#endif
