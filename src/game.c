#include <SDL2/SDL.h>
#include "game.h"
#include <stdio.h>
#include <math.h>
#include <SDL_ttf.h>


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

void update(Entity *player, Entity *bullet, Entity *bullet_enemy, bool *bullet_enemy_active, bool *bullet_active, float dt)
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

    if(*bullet_enemy_active){
        bullet_enemy->y += bullet_enemy->vy * dt;
        if (bullet_enemy->y > SCREEN_HEIGHT)
            *bullet_enemy_active = false;
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

void render(SDL_Renderer *renderer, Entity *player, Entity *bullet, Horde *horde, Entity *bullet_enemy, bool bullet_active, bool bullet_enemy_active, float vies)
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

    if (bullet_enemy_active)
    {
        SDL_Rect bullet_enemy_rect = {
            (int)bullet_enemy->x, (int)bullet_enemy->y,
            bullet_enemy->w, bullet_enemy->h};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &bullet_enemy_rect);
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

    for(int i=0;i<vies;i++){
        SDL_Rect vies_rect = {
        5+12*i, 5,
        10, 10};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &vies_rect);
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


void update_horde(Horde *horde, float time, float dt, bool *droite){
    bool newline=false;
    if(*droite){
        for(int i=0;i<horde->Nbr_de_lignes*horde->Nbr_par_ligne;i++){
            horde->x[i]+=dt*(0.2+0.8*time/(time+20))*horde->v;
        }
        if(horde->x[horde->Nbr_par_ligne-1]+horde->w>=SCREEN_WIDTH-50){
            *droite=false;
            for(int i=0;i<horde->Nbr_de_lignes*horde->Nbr_par_ligne;i++){
                horde->y[i] += DISTANCE_ENTRE_ENNEMIS_Y+horde->h;
                newline=true;
            }
        }
    }
    if(!*droite){
        for(int i=0;i<horde->Nbr_de_lignes*horde->Nbr_par_ligne;i++){
            horde->x[i]-=dt*(0.2+0.8*time/(time+20))*horde->v;
        }
        if(horde->x[0]<=50){
            *droite=true;
            for(int i=0;i<horde->Nbr_de_lignes*horde->Nbr_par_ligne;i++){
                horde->y[i] += DISTANCE_ENTRE_ENNEMIS_Y+horde->h;
                newline=true;
            }
        }
    }
    // for(int i=0;i<horde->Nbr_de_lignes*horde->Nbr_par_ligne;i++){
    //     horde->y[i]+=dt*(0.2+0.8*time/(time+20))*horde->v;
    //}
    if(newline && horde->Nbr_de_lignes<=20){
        horde->x=realloc(horde->x,(horde->Nbr_par_ligne*(horde->Nbr_de_lignes+1))*sizeof(float));
        // if(horde->x==NULL){
        //     printf("Probleme dans la redifinition de la horde");
        //     return NULL;
        // }
        horde->y=realloc(horde->y,(horde->Nbr_par_ligne*(horde->Nbr_de_lignes+1))*sizeof(float));
        //  if(horde->y==NULL){
        //     printf("Probleme dans la redifinition de la horde");
        //     return NULL;
        // }
        horde->existence=realloc(horde->existence,(horde->Nbr_par_ligne*(horde->Nbr_de_lignes+1))*sizeof(int));
        // if(horde->existence==NULL){
        //     printf("Probleme dans la redifinition de la horde");
        //     return NULL;
        // }
        horde->Nbr_de_lignes+=1;
        for(int i=horde->Nbr_par_ligne*(horde->Nbr_de_lignes-1);i<horde->Nbr_par_ligne*horde->Nbr_de_lignes;i++){
            horde->x[i]=horde->x[i-horde->Nbr_par_ligne];
            horde->y[i]=0;
            horde->existence[i]=1;
        }
    
    }
}

Horde* initial_horde(){
    Horde *horde=malloc(sizeof(Horde));
    horde->Nbr_de_lignes=8;
    // if(horde==NULL) return NULL;
    horde->x=malloc(sizeof(float)*ENEMIES_ON_LINE*horde->Nbr_de_lignes);
    // if(horde->x==NULL){
    //     free(horde);
    //     return NULL;
    // }
    horde->y=malloc(sizeof(float)*ENEMIES_ON_LINE*horde->Nbr_de_lignes);
    // if(horde->y==NULL){
    //     free(horde);
    //     return NULL;
    // }
    horde->existence=malloc(sizeof(int)*ENEMIES_ON_LINE*horde->Nbr_de_lignes);
    // if(horde->existence==NULL){
    //     free(horde);
    //     return NULL;
    // }
    horde->Nbr_par_ligne=ENEMIES_ON_LINE;
    horde->v=ENEMIES_SPEED_MAX;
    horde->w=ENEMIES_WIDTH;
    horde->h=ENEMIES_HEIGHT;
    for(int i=0;i<horde->Nbr_par_ligne*horde->Nbr_de_lignes;i++){
        horde->x[i]=50+(i%horde->Nbr_par_ligne)*(DISTANCE_ENTRE_ENNEMIS_X+horde->w);
        horde->y[i]=(DISTANCE_ENTRE_ENNEMIS_Y+horde->h)*(horde->Nbr_de_lignes-1)-(i/horde->Nbr_par_ligne)*(DISTANCE_ENTRE_ENNEMIS_Y+horde->h);
        horde->existence[i]=1;
    }
    return horde;
}

void update_fast_enemy(Entity *fast, bool *fast_actif, int *cote_fast, bool *fast_droite, bool *bullet_active, Entity *bullet, float dt){
    if(*fast_actif){
        fast->y+=dt*fast->vy;
        if(*fast_droite){
            fast->x+=dt*fast->vx;
        }
        else{
            fast->x+=dt*fast->vx;
        }
        *cote_fast+=1;
        if(*cote_fast==20){
            *cote_fast=0;
            if(*fast_droite){
                *fast_droite=false;
            }
            else{
                *fast_droite=true;
            }
        }
        if(*bullet_active && fast->y<=bullet->y && bullet->y<=fast->y+fast->h && fast->x<=bullet->x && bullet->x<=fast->x+fast->w){
            *fast_actif=false;
            *bullet_active=false;
        }
    }
    else{
        int nbr=rand()%15;
        if(nbr==0){
            fast->x=rand()%(SCREEN_WIDTH-3*ENEMIES_WIDTH)+ENEMIES_WIDTH;
            *fast_actif=true;
        }
    }
}

void defeat(Horde *horde, Entity *player, bool *running, int vies){
    if(vies==0){
        *running=false;
        printf("You lose");
    }
    int i=0;
    while(horde->y[i]+horde->h>=player->y && *running){
        if(horde->existence[i]==1){
            *running=false;
            printf("You lose");
        }
        i+=1;
    }
}

void attack_horde(Horde *horde, Entity *player, Entity *bullet_enemy, bool *bullet_enemy_active, int *vies){
    if(*bullet_enemy_active && player->y>=bullet_enemy->y && bullet_enemy->y>=player->y-player->h && player->x<=bullet_enemy->x && bullet_enemy->x<=player->x+player->w){
        *vies=*vies-1;
        *bullet_enemy_active=false;
    }
    if(!*bullet_enemy_active){
        int nbr=rand()%30;
        if(nbr==0){
            int indice_tireur=rand()%(horde->Nbr_de_lignes*horde->Nbr_par_ligne);
            if(horde->existence[indice_tireur]!=0 && horde->y[indice_tireur]<=player->y-150){
                *bullet_enemy_active = true;
                bullet_enemy->x = horde->x[indice_tireur] + horde->w / 2 - BULLET_WIDTH / 2;
                bullet_enemy->y = horde->y[indice_tireur]+horde->h;
                bullet_enemy->w = BULLET_WIDTH;
                bullet_enemy->h = BULLET_HEIGHT;
                bullet_enemy->vy = BULLET_SPEED/2;
            }
        }
    }
}

void free_horde(Horde* horde){
    free(horde->x);
    free(horde->y);
    free(horde->existence);
    free(horde);
}