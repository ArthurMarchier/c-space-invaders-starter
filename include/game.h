#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "entity.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 20
#define PLAYER_SPEED 400.0f

#define BULLET_WIDTH 10
#define BULLET_HEIGHT 20
#define BULLET_SPEED 600.0f

#define ENEMIES_ON_LINE 5
#define ENEMIES_SPEED_MAX 130.0f
#define DISTANCE_ENTRE_ENNEMIS_X 50
#define DISTANCE_ENTRE_ENNEMIS_Y 30
#define ENEMIES_WIDTH 50
#define ENEMIES_HEIGHT 30


bool init(SDL_Window **window, SDL_Renderer **renderer);
void handle_input(bool *running, const Uint8 *keys, Entity *player, Entity *bullet, bool *bullet_active);
void update(Entity *player, Entity *bullet, bool *bullet_active, float dt);
void detect_collision_enemy(Entity *bullet, Horde *horde, bool *bullet_active);
void render(SDL_Renderer *renderer, Entity *player, Entity *bullet, Horde *horde, bool bullet_active);
void cleanup(SDL_Window *window, SDL_Renderer *renderer);
void update_horde(Horde *horde, float time, float dt);
Horde* initial_horde();
void defeat(Horde *horde, Entity *player, bool *running);
void free_horde(Horde* horde);

#endif
