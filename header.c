#ifndef HEADER_H
#define HEADER_H


#include <allegro.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

#define LARGEUR_ECRAN 800
#define HAUTEUR_ECRAN 546
#define LARGEUR_DECOR 8316
#define HAUTEUR_DECOR 546
#define VITESSE_SCROLL 2
#define VITESSE_VAISSEAU 3
#define NB_VIES 3
#define MAX_TORPILLES 100
#define MAX_POISSONS 50
#define NB_TYPES_POISSONS 4
#define MAX_BULLES 100
#define TEMPS_CLIGNOTE 100
#define DELAI_BULLE 100

#define MAX_PSEUDO 20
#define FICHIER_SAUVEGARDE "sauvegardes.txt"
#define MAX_ETOILES 10
#define MAX_BULLES_ETOILE 20
#define DELAI_TIR_ETOILE 100

#define MAX_REQUINS 10
#define DELAI_TIR_REQUIN 100
#define DEGAT_REQUIN 50

#define DELAI_BULLE_BOSS 60
#define BOSS_FRAME_COUNT 5
#define BOSS_FRAME_WIDTH 187
#define BOSS_FRAME_HEIGHT 159

typedef struct {
    int x, y;
    int actif;
    int delai_tir;
} Etoile;

typedef struct {
    int x, y;
    int actif;
} BulleEtoile;

typedef struct {
    int x, y;
    int actif;
} Torpille;

typedef struct {
    int x, y;
    int type;
    int actif;
    int clignote;
    int temps_touche;
    int direction_y;
    int delai_bulle;
} Poisson;

typedef struct {
    int x, y;
    int actif;
} Bulle;

typedef struct {
    int x, y;
    int actif;
    int pris;
} Bonus;

typedef struct {
    int x, y;
    int actif;
    int pris;
} BouclierBonus;

typedef struct {
    int actif;
    int x, y;
    int y_init;
    int hp;
    BITMAP *spritesheet;
    int frame;
    int frame_timer;
    int largeur_frame;
    int hauteur_frame;
    int nb_frames;
} Boss;

typedef struct {
    int x, y;
    int dx, dy;
    int actif;
} epine_etoile_t;

typedef struct {
    int x, y;
    int actif;
    int delai_tir;
    clock_t temps_touche;
    int clignote;
} Requin;

#define MAX_DENTS 50

typedef struct {
    int actif;
    float x, y;
    float dx, dy;
} Dent;

Dent dents[MAX_DENTS];


Requin requins[MAX_REQUINS];
BITMAP *requin_img;

extern BouclierBonus boucliers[3];
extern BITMAP *bouclier_img;
extern int bouclier_actif;



Torpille torpilles[MAX_TORPILLES];
Poisson poissons[MAX_POISSONS];
Bulle bulles[MAX_BULLES];
Bonus bonus;

Etoile etoiles[MAX_ETOILES];
BulleEtoile bulles_etoile[MAX_BULLES_ETOILE];
BITMAP *etoile_img;

BITMAP *poisson_imgs[NB_TYPES_POISSONS];
BITMAP *coeur_img, *bulle_img;

int positionValide(int x, int y, BITMAP *collision, BITMAP *vaisseau);
int positionPoissonValide(int x, int y, BITMAP *img, BITMAP *collision);

void tireTorpille(int x, int y);
void tireBulle(int x, int y);
void Banc_Poisson(int camera_x, BITMAP *collision);
void BonusLife(int camera_x, BITMAP *collision, BITMAP *coeur_img);

void textegros(BITMAP* bmp, FONT* f, const char* txt, int x, int y, int color);
void sauvegarde(const char* pseudo, int niveau);
int chargementPartie(const char* pseudo_cherche, int* niveau_retourne);
void EcranMenu(const char* titre, const char* options[], int n, int selection);
int GestionPseudo(char* destination);
void gestionNiveaux();
int menu();
void ajouter_etoiles(int camera_x, BITMAP *collision);
void tirer_bulle_etoile(int x, int y);
void jouer_niveau1(void);
void gestionVictoire(int niveau_actuel);

void ajouter_requin(int camera_x, int y_min, int y_max);
void jouer_niveau2(void);
void ajouterPoisson(int camera_x, BITMAP *collision);


void jouer_niveau3(void);

#define MAX_EPINES_ETOILE 50
extern epine_etoile_t epines_etoile[MAX_EPINES_ETOILE];

void tirer_epine_etoile(int x, int y);
epine_etoile_t epines_etoile[MAX_EPINES_ETOILE];
#endif //HEADER_H
