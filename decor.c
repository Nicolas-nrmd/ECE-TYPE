#include "header.h"


int est_position_valide(int x, int y, BITMAP *collision, BITMAP *vaisseau) {
    for (int i = 0; i < vaisseau->w; i++) {
        for (int j = 0; j < vaisseau->h; j++) {
            int px = x + i;
            int py = y + j;
            if (px >= 0 && px < collision->w && py >= 0 && py < collision->h) {
                int couleur = getpixel(collision, px, py);
                if (couleur == makecol(0, 0, 0)) return 0;
            }
        }
    }
    return 1;
}

int position_poisson_valide(int x, int y, BITMAP *img, BITMAP *collision) {
    for (int i = 0; i < img->w; i++) {
        for (int j = 0; j < img->h; j++) {
            int px = x + i;
            int py = y + j;
            if (px >= 0 && px < collision->w && py >= 0 && py < collision->h) {
                if (getpixel(collision, px, py) == makecol(0, 0, 0)) return 0;
            }
        }
    }
    return 1;
}
