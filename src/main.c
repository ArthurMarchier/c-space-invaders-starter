#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL_ttf.h>
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
        .vy=SPEEDY_ENNEMI_RAPIDE};
    
    Entity vie_tombante={
        .x=0,
        .y=0,
        .w=VIE_WIDTH,
        .h=VIE_WIDTH,
        .vx=BULLET_SPEED*0.4,
        .vy=BULLET_SPEED*0.4,
    };

    Entity bullet = {0};
    bool bullet_active = false;

    Entity bullet_enemy = {0};
    bool bullet_enemy_active = false;

    bool droite=true;

    Horde *horde=initial_horde();
    
    float time=0;
    
    int vies=3;
    int cote_fast=0;
    bool fast_actif=false;
    bool fast_droite=true;
    bool vie_tombe=false;
    int score=0;
    bool victoire=false;

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
        render(renderer, &player, &bullet, horde, &bullet_enemy, bullet_active, bullet_enemy_active, vies, &fast, fast_actif, vie_tombe, &vie_tombante);
        handle_input(&running, keys, &player, &bullet, &bullet_active);
        detect_collision_enemy(&bullet, horde, &bullet_active, &score);
        update(&player, &bullet, &bullet_enemy, &bullet_enemy_active, &bullet_active, dt);
        update_horde(horde, time, dt, &droite);
        attack_horde(horde, &player, &bullet_enemy, &bullet_enemy_active, &vies);
        update_fast_enemy(&fast, &fast_actif, &cote_fast, &fast_droite, &bullet_active, &bullet, dt, &vies);
        if(vies<5)
            vies_qui_tombent(&vies, &vie_tombante, &vie_tombe, &player, dt);
        end_game(horde, &player, &running, vies, score, &victoire);
    }

    free_horde(horde);
    float time_end=0;
    while(time_end<5){
        afficher_resultat(renderer, victoire);
        SDL_RenderPresent(renderer);
        Uint32 ticks = SDL_GetTicks();
        float dt = (ticks - last_ticks) / 1000.0f;
        last_ticks = ticks;
        time_end+=dt;
    }
    cleanup(window, renderer);
    return 0;
}
