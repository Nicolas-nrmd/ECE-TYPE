#include "header.h"


int positionValide(int x, int y, BITMAP *collision, BITMAP *vaisseau) {
    for (int i = 0; i < vaisseau->w; i++) {//vérifie que le vaisseau reste dans les limites du décor
        for (int j = 0; j < vaisseau->h; j++) {
            int px = x + i;
            int py = y + j;
            if (px >= 0 && px < collision->w && py >= 0 && py < collision->h) {//le vaisseau ne touche pas le bitmap de collision
                int couleur = getpixel(collision, px, py);
                if (couleur == makecol(0, 0, 0)) return 0;//bitmap de collision en noir
            }
        }
    }
    return 1;
}

int positionPoissonValide(int x, int y, BITMAP *img, BITMAP *collision) {
    for (int i = 0; i < img->w; i++) {//vérifie que les poissons apparaissent dans les limites du décor
        for (int j = 0; j < img->h; j++) {
            int px = x + i;
            int py = y + j;
            if (px >= 0 && px < collision->w && py >= 0 && py < collision->h) {//les poissons n'apparaissent pas sur le bitmap de collision
                if (getpixel(collision, px, py) == makecol(0, 0, 0)) return 0;
            }
        }
    }
    return 1;
}

int positionRequinValide(int x, int y, BITMAP *img, BITMAP *collision) {
    for (int i = 0; i < img->w; i++) { // Vérifie que les requins apparaissent dans les limites du décor
        for (int j = 0; j < img->h; j++) {
            int px = x + i;
            int py = y + j;
            if (px >= 0 && px < collision->w && py >= 0 && py < collision->h) {
                // Les requins ne doivent pas apparaître sur les zones de collision (zones noires)
                if (getpixel(collision, px, py) == makecol(0, 0, 0)) return 0;
            }
        }
    }
    return 1; // La position est valide
}
