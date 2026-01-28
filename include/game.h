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

#define ENEMIES_ON_LINE 10
#define ENEMIES_SPEED_MAX 50.0f
#define DISTANCE_ENTRE_ENNEMIS_X 10
#define DISTANCE_ENTRE_ENNEMIS_Y 20
#define ENEMIES_WIDTH 50
#define ENEMIES_HEIGHT 30

#define SPEEDX_ENNEMI_RAPIDE 60
#define SPEEDY_ENNEMI_RAPIDE 70


bool init(SDL_Window **window, SDL_Renderer **renderer);
void handle_input(bool *running, const Uint8 *keys, Entity *player, Entity *bullet, bool *bullet_active);
void update(Entity *player, Entity *bullet, Entity *bullet_enemy, bool *bullet_enemy_active, bool *bullet_active, float dt);
void detect_collision_enemy(Entity *bullet, Horde *horde, bool *bullet_active);
void render(SDL_Renderer *renderer, Entity *player, Entity *bullet, Horde *horde, Entity *bullet_enemy, bool bullet_active, bool bullet_enemy_active, float vies);
void cleanup(SDL_Window *window, SDL_Renderer *renderer);
void update_horde(Horde *horde, float time, float dt, bool *droite);
Horde* initial_horde();
void defeat(Horde *horde, Entity *player, bool *running, int vies);
void attack_horde(Horde *horde, Entity *player, Entity *bullet_enemy, bool *bullet_enemy_active, int *vies);
void free_horde(Horde* horde);

#endif
