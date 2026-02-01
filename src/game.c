#include <SDL2/SDL.h>
#include "game.h"
#include <stdio.h>
#include <math.h>
#include <SDL_ttf.h>
#include <SDL_image.h>


bool init(SDL_Window **window, SDL_Renderer **renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Erreur SDL_Init: %s", SDL_GetError());
        return false;
    }

    if (TTF_Init() == -1)
        return false;

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

void detect_collision_enemy(Entity *bullet, Horde *horde, bool *bullet_active, int *score){
    int i=0;
    while(i<horde->Nbr_de_lignes*horde->Nbr_par_ligne && *bullet_active){
        if(horde->existence[i]!=0 && (horde->y[i]<=bullet->y) && bullet->y<=horde->y[i]+horde->h && horde->x[i]<=bullet->x && bullet->x<=horde->x[i]+horde->w){
            horde->existence[i]-=1;
            *bullet_active=false;
            if(horde->existence[i]==0)
                *score+=1;
        }
        i++;
    }
}

void render(SDL_Renderer *renderer, Entity *player, Entity *bullet, Horde *horde, Entity *bullet_enemy, bool bullet_active, bool bullet_enemy_active, float vies, Entity *fast, bool fast_actif, bool vie_tombe, Entity *vie_tombante)
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

    if(fast_actif)
    {
        SDL_Rect fast_rect = {
        (int)fast->x, (int)fast->y,
        fast->w, fast->h};
        SDL_SetRenderDrawColor(renderer, 0,255,255,0);
        SDL_RenderFillRect(renderer, &fast_rect);
    }

    if(vie_tombe)
    {
        SDL_Surface *surface = IMG_Load("images/Design_sans_titre-removebg-preview.png");
        if (!surface)
            printf("Erreur IMG_Load: %s\n", IMG_GetError());
        
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!texture)
            printf("Erreur texture: %s\n", SDL_GetError());

        SDL_Rect vie_tombante_rect = {
        (int)vie_tombante->x, (int)vie_tombante->y,
        vie_tombante->w, vie_tombante->h};
        SDL_RenderCopy(renderer, texture, NULL, &vie_tombante_rect);
    }

    for(int i=0;i<horde->Nbr_de_lignes*horde->Nbr_par_ligne;i++){
        if(horde->existence[i]==1){
            SDL_Rect enemy_rect = {
            (int)horde->x[i], (int)horde->y[i],
            horde->w, horde->h};
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderFillRect(renderer, &enemy_rect);
        }
        else if(horde->existence[i]==2){
            SDL_Rect enemy_rect = {
            (int)horde->x[i], (int)horde->y[i],
            horde->w, horde->h};
            SDL_SetRenderDrawColor(renderer, 0, 0, 150, 150);
            SDL_RenderFillRect(renderer, &enemy_rect);
        }
    }

    for(int i=0;i<vies;i++){
        SDL_Surface *surface = IMG_Load("images/Design_sans_titre-removebg-preview.png");
        if (!surface)
            printf("Erreur IMG_Load: %s\n", IMG_GetError());
        
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!texture)
            printf("Erreur texture: %s\n", SDL_GetError());

        SDL_Rect vies_rect = {
        5+18*(i%3), 5+18*(i/3),
        VIE_WIDTH, VIE_WIDTH};
        SDL_RenderCopy(renderer, texture, NULL, &vies_rect);
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
    if(newline && horde->Nbr_de_lignes<NBR_DE_LIGNES_MAX_HORDE){
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
        int k=1;
        horde->Nbr_de_lignes+=1;
        if(horde->Nbr_de_lignes%5==0){
            k=2;
        }
        for(int i=horde->Nbr_par_ligne*(horde->Nbr_de_lignes-1);i<horde->Nbr_par_ligne*horde->Nbr_de_lignes;i++){
            horde->x[i]=horde->x[i-horde->Nbr_par_ligne];
            horde->y[i]=0;
            horde->existence[i]=k;
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
        if(4*horde->Nbr_par_ligne<=i && i<5*horde->Nbr_par_ligne){
            horde->existence[i]=2;
        }
        else{
            horde->existence[i]=1;
        }
    }
    return horde;
}

void update_fast_enemy(Entity *fast, bool *fast_actif, int *cote_fast, bool *fast_droite, bool *bullet_active, Entity *bullet, float dt, int *vies){
    if(*fast_actif){
        fast->y+=dt*fast->vy;
        if(*fast_droite){
            fast->x+=dt*fast->vx;
        }
        else{
            fast->x-=dt*fast->vx;
        }
        *cote_fast+=1;
        if(*cote_fast==500){
            *cote_fast=0;
            int nbr1=random()%2;
            if(nbr1==0){
                *fast_droite=false;
            }
            else{
                *fast_droite=true;
            }
        }
        if(fast->x<=0 && *fast_droite==false){
            *fast_droite=true; //Pour ne pas qu'il sorte de l'écran
        }
        else if(fast->x+fast->w>=SCREEN_WIDTH && *fast_droite==true){
            *fast_droite=false;
        }
        if(*bullet_active && fast->y<=bullet->y && bullet->y<=fast->y+fast->h && fast->x<=bullet->x && bullet->x<=fast->x+fast->w){
            *fast_actif=false;
            *bullet_active=false;
        }
        if(fast->y>=SCREEN_WIDTH){
            *vies-=1;
            *fast_actif=false;
        }
    }
    else{
        int nbr2=rand()%3000;
        if(nbr2==0){
            fast->x=rand()%(SCREEN_WIDTH-3*ENEMIES_WIDTH)+ENEMIES_WIDTH;
            fast->y=-fast->h;
            *fast_actif=true;
        }
    }
}

void end_game(Horde *horde, Entity *player, bool *running, int vies, int score, bool *victoire){
    if(vies==0){
        *running=false;
    }
    else if(score==horde->Nbr_par_ligne*NBR_DE_LIGNES_MAX_HORDE){
        *running=false;
        *victoire=true;
    }
    int i=0;
    while(horde->y[i]+horde->h>=player->y && *running){
        if(horde->existence[i]==1){
            *running=false;
        }
        i+=1;
    }
}

void vies_qui_tombent(int *vies, Entity *vie_tombante, bool *vie_tombe, Entity *player, float dt){
    if(*vie_tombe){
        vie_tombante->y+=dt*vie_tombante->vy;
        if(((vie_tombante->y<=player->y && vie_tombante->y+vie_tombante->h>=player->y) || (vie_tombante->y>=player->y && vie_tombante->y<=player->y+player->h)) && ((vie_tombante->x>=player->x && vie_tombante->x<=player->x+player->w) || (vie_tombante->x<=player->x && vie_tombante->x+vie_tombante->w>=player->x))){
            *vie_tombe=false;
            *vies+=1;
        }
        if(vie_tombante->y>=SCREEN_WIDTH)
            *vie_tombe=false;
    }
    else{
        int nbr2=rand()%5000;
        if(nbr2==0){
            vie_tombante->x=rand()%(SCREEN_WIDTH-VIE_WIDTH)+VIE_WIDTH;
            vie_tombante->y=-vie_tombante->h;
            *vie_tombe=true;
        }
    }
}

void attack_horde(Horde *horde, Entity *player, Entity *bullet_enemy, bool *bullet_enemy_active, int *vies){
    if(*bullet_enemy_active && player->y>=bullet_enemy->y && bullet_enemy->y>=player->y-player->h && player->x<=bullet_enemy->x && bullet_enemy->x<=player->x+player->w){
        *vies=*vies-1;
        *bullet_enemy_active=false;
    }
    if(!*bullet_enemy_active){
        int nbr=rand()%50;
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


void afficher_resultat(SDL_Renderer *renderer, bool victoire)
{
    TTF_Font *font = TTF_OpenFont("font/BitcountSingle-VariableFont_CRSV,ELSH,ELXP,slnt,wght.ttf", 48);
    if (!font)
        return;

    SDL_Color rouge = {255, 0, 0, 255};
    SDL_Color vert = {0, 255, 0, 255};

    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, "YOU LOSE", rouge);
    if(victoire)
        surface = TTF_RenderUTF8_Blended(font, "YOU WIN", vert);

    if (!surface)
        return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dst;
    dst.w = surface->w;
    dst.h = surface->h;
    dst.x = (SCREEN_WIDTH - dst.w) / 2;
    dst.y = (SCREEN_HEIGHT - dst.h) / 2;

    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}
