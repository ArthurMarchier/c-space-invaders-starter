#include <SDL2/SDL.h>
#include <stdbool.h>
//#include <SDL_ttf.h>
#include "entity.h"
#include "game.h"

int main(void)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (!init(&window, &renderer))
    {
        return 1;
    }

    bool running = true;
    Uint32 last_ticks = SDL_GetTicks();

    Entity player = {
        .x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2,
        .y = SCREEN_HEIGHT - 60,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
        .vx = 0,
        .vy = 0};

    Entity fast={
        .x=0,
        .y=0,
        .w=ENEMIES_WIDTH,
        .h=ENEMIES_HEIGHT,
        .vx=SPEEDX_ENNEMI_RAPIDE,
        .vy=SPEEDy_ENNEMI_RAPIDE};
    
    Entity bullet = {0};
    bool bullet_active = false;

    Entity bullet_enemy = {0};
    bool bullet_enemy_active = false;

    droite=true;

    Horde *horde=initial_horde();
    
    float time=0;
    
    int vies=0;
    int cote_fast=0;
    bool fast_actif=false;
    bool fast_droite=true;

    while (running)
    {
        Uint32 ticks = SDL_GetTicks();
        float dt = (ticks - last_ticks) / 1000.0f;
        if (dt > 0.05f)
            dt = 0.05f;
        last_ticks = ticks;
        time+=dt;

        SDL_PumpEvents();
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        handle_input(&running, keys, &player, &bullet, &bullet_active);
        detect_collision_enemy(&bullet, horde, &bullet_active);
        update(&player, &bullet, &bullet_enemy, &bullet_enemy_active, &bullet_active, dt);
        update_horde(horde, time, dt, &droite);
        attack_horde(horde, &player, &bullet_enemy, &bullet_enemy_active, &vies);
        defeat(horde, &player, &running, vies);
        render(renderer, &player, &bullet, horde, &bullet_enemy, bullet_active, bullet_enemy_active, vies);
    }

    free_horde(horde);
    cleanup(window, renderer);
    return 0;
}
