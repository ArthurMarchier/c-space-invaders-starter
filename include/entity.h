#ifndef ENTITY_H
#define ENTITY_H

typedef struct
{
    float x, y;
    float vx, vy;
    int w, h;
} Entity;

typedef struct
{   
    int Nbr_par_ligne;
    int Nbr_de_lignes;
    float *x; //Liste des positions en x
    float *y; //Tableau des positions en y
    float v;
    int w, h;
    bool *existence; //Détecte si l'ennemi est vivant ou non
} Horde;

#endif
