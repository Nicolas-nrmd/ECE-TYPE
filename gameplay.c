#include "header.h"

void tireTorpille(int x, int y) {
    for (int i = 0; i < MAX_TORPILLES; i++) { //Pour chaque torpille jusqu'au max de torpille
        if (!torpilles[i].actif) { //vérifie ue la torpille n'est pas active
            torpilles[i].x = x; //initialise les coordonnées de depart de la torpille
            torpilles[i].y = y;
            torpilles[i].actif = 1; //lance la torpille
            break;
        }
    }
}

void tireBulle(int x, int y) {
    for (int i = 0; i < MAX_BULLES; i++) {//Pour chaque bulle jusqu'au max de bulle
        if (!bulles[i].actif) { //vérifie que la bulle n'est pas active
            bulles[i].x = x; //initialise les coordonnées de depart de la bulle
            bulles[i].y = y;
            bulles[i].actif = 1;//lance la bulle
            break;
        }
    }
}

void Banc_Poisson(int camera_x, BITMAP *collision) {
    int type = rand() % NB_TYPES_POISSONS; //poissons aléatoires
    int taille_banc = 3 + rand() % 2; // taille du banc aléatoire entre 3 et 4
    for (int i = 0; i < MAX_POISSONS && taille_banc > 0; i++) { //pour chaque poissons jusqu'au max
        if (!poissons[i].actif) {  //vérifie que le poisson n'est pas actif
            int x = camera_x + LARGEUR_ECRAN + rand() % 200; //initialise les coordonnées aléatoire de depart du poisson
            int y;
            int tentative = 0;
            do {
                y = rand() % (HAUTEUR_DECOR - poisson_imgs[type]->h); //génère des poissons
                tentative++;
            } while (!positionPoissonValide(x, y, poisson_imgs[type], collision) && tentative < 10); //tant qu'ils ont une position valide

            if (tentative < 10) { //le test de tentative de placement ne dépasse pas 10
                poissons[i].type = type; //type de poisson
                poissons[i].x = x; //coordonnées de placement
                poissons[i].y = y;
                poissons[i].actif = 1; //lance le poisson
                poissons[i].clignote=0; //ne clignote pas
                poissons[i].temps_touche = 0; // pas de delai de touche
                poissons[i].direction_y = (rand() % 3) - 1; //mouvement aléatoire
                poissons[i].delai_bulle = rand() % DELAI_BULLE; //tir aléatoire
                taille_banc--;
            }
        }
    }
}

void BonusLife(int camera_x, BITMAP *collision, BITMAP *coeur_img) {
    int tentative = 0;
    do {
        bonus.x = camera_x + LARGEUR_ECRAN + rand() % 200; //génère la position de la bonus life aléatoirement
        bonus.y = rand() % (HAUTEUR_DECOR - coeur_img->h);
        tentative++;
    } while (!positionValide(bonus.x, bonus.y, collision, coeur_img) && tentative < 10); //vérifie que le bonus est sur le blanc du bitmap de colision

    if (tentative < 10) { //le nombre de tentative ne dépasse pas 10
        bonus.actif = 1;
        bonus.pris = 0;
    }
}

void ajouter_etoiles(int camera_x, BITMAP *collision) {
    for (int i = 0; i < MAX_ETOILES; i++) { //pour chaque étoile jusqu'au max
        if (!etoiles[i].actif) { //vérifie que l'étoile n'est pas active
            int x = camera_x + LARGEUR_ECRAN + rand() % 200; //génère sa position aléatoirement
            int y = 0;

            for (int yy = 0; yy < HAUTEUR_DECOR - etoile_img->h; yy++) {
                int color = getpixel(collision, x, yy + etoile_img->h); //place l'étoile sur le bitmap de collision
                if (getr(color) == 0 && getg(color) == 0 && getb(color) == 0) { // zone noire
                    y = yy;
                    break;
                }
            }

            etoiles[i].x = x;
            etoiles[i].y = y;
            etoiles[i].actif = 1;
            etoiles[i].delai_tir = rand() % DELAI_TIR_ETOILE; //le tir de l'étoile est aléatoire
            break;
        }
    }
}

void tirer_bulle_etoile(int x, int y) {
    for (int i = 0; i < MAX_BULLES_ETOILE; i++) {//pour chaque bulle de l'étoile jusqu'au max
        if (!bulles_etoile[i].actif) { //vérifie que pas encore active
            bulles_etoile[i].x = x; //initialise ses positions de départ
            bulles_etoile[i].y = y;
            bulles_etoile[i].actif = 1; //lance la bulle
            break;
        }
    }
}
void tireTorpilleSpeciale(int x, int y) {
    for (int i = 0; i < MAX_TORPILLES; i++) {
        if (!torpilles[i].actif) {
            torpilles[i].x = x;
            torpilles[i].y = y;
            torpilles[i].actif = 2; // différent de 1 pour repérer les spéciales
            break;
        }
    }
}

void ajouter_requin(int camera_x, int y_min, int y_max, BITMAP *collision) {

    for (int i = 0; i < MAX_REQUINS; i++) { //pour chaque requin jusqu'au max
        if (!requins[i].actif) { //vérifie que le requin n'est pas encore actif
            int tentative = 0;
            const int MAX_TENTATIVES = 10;

            while (tentative < MAX_TENTATIVES) { //nombre de tentative limité
                int x = camera_x + LARGEUR_ECRAN + rand() % 200; //place le requin aléatoirement
                int y = y_min + rand() % (y_max - y_min + 1);

                if (positionRequinValide(x, y, requin_img, collision)) { //vérifie la position valide avant placement
                    requins[i].x = x; //initialisation des coordonnées
                    requins[i].y = y;
                    requins[i].actif = 1; //lance le requin
                    requins[i].delai_tir = DELAI_TIR_REQUIN;
                    requins[i].clignote = 0;
                    break;
                }
                tentative++; //incrémente le nombre de tentative
            }

            break;
        }
    }
}

void ajouterPoisson(int camera_x, BITMAP *collision) {
    int type = 0;
    int taille_banc = 3 + rand() % 2; //banc aléatoire entre 3 et 4
    for (int i = 0; i < MAX_POISSONS && taille_banc > 0; i++) { //pour chaque poisson jusqu'au max
        if (!poissons[i].actif) { //poisson pas encore actif
            int x = camera_x + LARGEUR_ECRAN + rand() % 200; //poisson générer aléatoirement
            int y;
            int tentative = 0;
            do {
                y = rand() % (HAUTEUR_DECOR - poisson_imgs[type]->h); //position aléatoire
                tentative++;
            } while (!positionPoissonValide(x, y, poisson_imgs[type], collision) && tentative < 10);//vérifie la validité du positionnement

            if (tentative < 10) { //nombre max de tentative
                poissons[i].type = type; //type de poisson
                poissons[i].x = x; //coordonées
                poissons[i].y = y;
                poissons[i].actif = 1; //lance le poisson
                poissons[i].clignote=0;
                poissons[i].temps_touche = 0;
                poissons[i].direction_y = (rand() % 3) - 1; //direction aléatoire
                poissons[i].delai_bulle = rand() % DELAI_BULLE; //tire aléatoirement
                taille_banc--;
            }
        }
    }
}


void tirer_epine_etoile(int x, int y) {
    for (int i = 0; i < MAX_EPINES_ETOILE; i++) { //pour chaque epine jusqu'au max
        if (!epines_etoile[i].actif) { //epine pas encore active
            epines_etoile[i].actif = 1;
            epines_etoile[i].x = x; //coordonnées initialisées
            epines_etoile[i].y = y;

            // Angle aléatoire entre 135° et 45° pour viser vers le haut
            float angle = ((rand() % 91) + 45) * (M_PI / 180.0); // entre π/4 et 3/4

            float vitesse = 4.0f; // vitesse de tir
            epines_etoile[i].dx = cos(angle) * vitesse;
            epines_etoile[i].dy = -fabs(sin(angle)) * vitesse; // toujours vers le haut

            break;
        }
    }
}

void tireDent(int x, int y) {
    for (int i = 0; i < MAX_DENTS; i++) { //pour chaque dents jusqu'au max
        if (!dents[i].actif) {
            dents[i].actif = 1;
            dents[i].x = x; //coordonnées de lancements
            dents[i].y = y;
            dents[i].dx = -5;  // vers la gauche
            dents[i].dy = 0;
            break;
        }
    }
}

void tir(int x, int y) {
    for (int i = 0; i < MAX_TIR; i++) {
        if (!tirs[i].actif) {
            tirs[i].actif = 1;
            tirs[i].x = x;
            tirs[i].y = y;
            tirs[i].dx = -5;  // vers la gauche
            tirs[i].dy = 0;
            break;
        }
    }
}

void tireFeu(int x, int y) {
    for (int i = 0; i < MAX_FEU; i++) {
        if (!feus[i].actif) {
            feus[i].actif = 1;
            feus[i].x = x;
            feus[i].y = y;
            feus[i].dx = -5;  // vers la gauche
            feus[i].dy = 0;
            break;
        }
    }
}
