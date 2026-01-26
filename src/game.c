#include <SDL2/SDL.h>
#include "game.h"
#include <stdio.h>


bool init(SDL_Window **window, SDL_Renderer **renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Erreur SDL_Init: %s", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Space Invaders (SDL)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!*window)
    {
        SDL_Log("Erreur SDL_CreateWindow: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer)
    {
        SDL_Log("Erreur SDL_CreateRenderer: %s", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return false;
    }

    return true;
}

void handle_input(bool *running, const Uint8 *keys, Entity *player, Entity *bullet, bool *bullet_active)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            *running = false;
    }

    player->vx = 0.0f;
    if (keys[SDL_SCANCODE_LEFT])
        player->vx = -PLAYER_SPEED;
    if (keys[SDL_SCANCODE_RIGHT])
        player->vx = PLAYER_SPEED;

    if (keys[SDL_SCANCODE_SPACE] && !*bullet_active)
    {
        *bullet_active = true;
        bullet->x = player->x + player->w / 2 - BULLET_WIDTH / 2;
        bullet->y = player->y;
        bullet->w = BULLET_WIDTH;
        bullet->h = BULLET_HEIGHT;
        bullet->vy = -BULLET_SPEED;
    }
}

void update(Entity *player, Entity *bullet, bool *bullet_active, float dt)
{
    player->x += player->vx * dt;

    if (player->x < 0)
        player->x = 0;
    if (player->x + player->w > SCREEN_WIDTH)
        player->x = SCREEN_WIDTH - player->w;

    if (*bullet_active)
    {
        bullet->y += bullet->vy * dt;
        if (bullet->y + bullet->h < 0)
            *bullet_active = false;
    }
}

void detect_collision_enemy(Entity *bullet, Horde *horde, bool *bullet_active){
    int i=0;
    while(i<horde->Nbr_de_lignes*horde->Nbr_par_ligne && *bullet_active){
        if(horde->existence[i]==1 && (horde->y[i]<=bullet->y) && bullet->y<=horde->y[i]+horde->h && horde->x[i]<=bullet->x && bullet->x<=horde->x[i]+horde->w){
            horde->existence[i]=0;
            *bullet_active=false;
        }
        i++;
    }
}

void render(SDL_Renderer *renderer, Entity *player, Entity *bullet, Horde *horde, bool bullet_active)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect player_rect = {
        (int)player->x, (int)player->y,
        player->w, player->h};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &player_rect);

    if (bullet_active)
    {
        SDL_Rect bullet_rect = {
            (int)bullet->x, (int)bullet->y,
            bullet->w, bullet->h};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &bullet_rect);
    }

    for(int i=0;i<horde->Nbr_de_lignes*horde->Nbr_par_ligne;i++){
        if(horde->existence[i]==1){
            SDL_Rect enemy_rect = {
            (int)horde->x[i], (int)horde->y[i],
            horde->w, horde->h};
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderFillRect(renderer, &enemy_rect);
        }
    }

    SDL_RenderPresent(renderer);

}

void cleanup(SDL_Window *window, SDL_Renderer *renderer)
{
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}


void update_horde(Horde *horde, float dt){
    if(horde->y[0]+horde->v*dt>DISTANCE_ENTRE_ENNEMIS_Y+horde->h){
        float *New_x=malloc((horde->Nbr_par_ligne*(horde->Nbr_de_lignes+1))*sizeof(float));
        // if(New_x==NULL){
        //     printf("Probleme dans la redifinition de la horde");
        //     return NULL;
        // }
        float *New_y=malloc((horde->Nbr_par_ligne*(horde->Nbr_de_lignes+1))*sizeof(float));
        // if(New_y==NULL){
        //     printf("Probleme dans la redifinition de la horde");
        //     return NULL;
        // }
        int *New_existence=malloc((horde->Nbr_par_ligne*(horde->Nbr_de_lignes+1))*sizeof(int));
        // if(New_existence==nullptr){
        //     printf("Probleme dans la redifinition de la horde");
        //     return NULL;
        // }
        horde->Nbr_de_lignes+=1;
        for(int i=0;i<horde->Nbr_par_ligne;i++){
            New_x[i]=horde->x[i];
            New_y[i]=0;
            New_existence[i]=1;
        }
        for(int i=horde->Nbr_par_ligne;i<horde->Nbr_par_ligne*horde->Nbr_de_lignes;i++){
            New_x[i]=horde->x[i-horde->Nbr_par_ligne];
            New_y[i]=horde->y[i-horde->Nbr_par_ligne]+dt*horde->v;
            New_existence[i]=horde->existence[i-horde->Nbr_par_ligne];
        }
        free(horde->x);
        free(horde->y);
        free(horde->existence);
        horde->x=New_x;
        horde->y=New_y;
        horde->existence=New_existence;
    }
    else{
        for(int i=0;i<horde->Nbr_de_lignes*horde->Nbr_par_ligne;i++){
            horde->y[i] += horde->v * dt;
        }
    }
}

Horde* initial_horde(){
    Horde *horde=malloc(sizeof(Horde));
    // if(horde==NULL) return NULL;
    horde->x=malloc(sizeof(float)*ENEMIES_ON_LINE);
    // if(horde->x==NULL){
    //     free(horde);
    //     return NULL;
    // }
    horde->y=malloc(sizeof(float)*ENEMIES_ON_LINE);
    // if(horde->y==NULL){
    //     free(horde);
    //     return NULL;
    // }
    horde->existence=malloc(sizeof(int)*ENEMIES_ON_LINE);
    // if(horde->existence==NULL){
    //     free(horde);
    //     return NULL;
    // }
    horde->Nbr_de_lignes=1;
    horde->Nbr_par_ligne=ENEMIES_ON_LINE;
    horde->v=ENEMIES_SPEED;
    horde->w=ENEMIES_WIDTH;
    horde->h=ENEMIES_HEIGHT;
    for(int i=0;i<horde->Nbr_par_ligne;i++){
        horde->x[i]=150+i*(DISTANCE_ENTRE_ENNEMIS_X+horde->w);
        horde->y[i]=0;
        horde->existence[i]=1;
    }
    return horde;
}

void defeat(Horde *horde, Entity *player, bool *running){
    int i=horde->Nbr_par_ligne*horde->Nbr_de_lignes-1;
    while(horde->y[i]-horde->h>=player->y){
        if(horde->existence[i]==1){
            *running=false;
            printf("You lose");
        }
        i=i-1;
    }
}

void free_horde(Horde* horde){
    free(horde->x);
    free(horde->y);
    free(horde->existence);
    free(horde);
}