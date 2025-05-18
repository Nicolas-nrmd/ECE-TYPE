#ifndef HEADER_H
#define HEADER_H


#include <allegro.h>
#include <time.h>
#include <stdio.h>

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
#define TEMPS_CLIGNOTE 300
#define DELAI_BULLE 100

#define MAX_PSEUDO 20
#define FICHIER_SAUVEGARDE "sauvegardes.txt"

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

Torpille torpilles[MAX_TORPILLES];
Poisson poissons[MAX_POISSONS];
Bulle bulles[MAX_BULLES];
Bonus bonus;

BITMAP *poisson_imgs[NB_TYPES_POISSONS];
BITMAP *coeur_img, *bulle_img;

int est_position_valide(int x, int y, BITMAP *collision, BITMAP *vaisseau);
int position_poisson_valide(int x, int y, BITMAP *img, BITMAP *collision);

void tirer_torpille(int x, int y);
void tirer_bulle(int x, int y);
void ajouter_banc_poissons(int camera_x, BITMAP *collision);
void generer_bonus(int camera_x, BITMAP *collision, BITMAP *coeur_img);

void textout_gros(BITMAP* bmp, FONT* f, const char* txt, int x, int y, int color);
void sauvegarder_progression(const char* pseudo, int niveau);
int charger_progression(const char* pseudo_cherche, int* niveau_retourne);
void dessiner_ecran_menu(const char* titre, const char* options[], int n, int selection);
int saisir_pseudo_utilisateur(char* destination);
void menu_niveaux();
int menu();

void jouer_niveau1(void);

#endif //HEADER_H
