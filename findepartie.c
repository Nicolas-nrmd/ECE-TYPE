#include "header.h"

void gestionVictoire(int niveau_actuel) {
    clear_keybuf();  //vide le clavier

    BITMAP *decor = load_bitmap("biome1.bmp", NULL);
    clear(decor);

    textprintf_centre_ex(decor, font, LARGEUR_ECRAN / 2, HAUTEUR_ECRAN / 2 - 20,//affiche un message de félicitations en noir
                         makecol(255, 255, 255), -1,
                         "Bravo ! Vous avez terminé le niveau %d.", niveau_actuel);

    textprintf_centre_ex(decor, font, LARGEUR_ECRAN / 2, HAUTEUR_ECRAN / 2 + 10,//affiche un message de félicitations en noir
                         makecol(255, 255, 255), -1,
                         "Le niveau %d est désormais débloqué.", niveau_actuel + 1);

    textprintf_centre_ex(decor, font, LARGEUR_ECRAN / 2, HAUTEUR_ECRAN / 2 + 40,//affiche un message de félicitations en noir
                         makecol(255, 255, 255), -1,
                         "Appuyez sur une touche pour revenir au menu...");

    blit(decor, screen, 0, 0, 0, 0, LARGEUR_ECRAN, HAUTEUR_ECRAN);//nettoie l'ecran
    readkey(); // Attend que le joueur appuie sur une touche
}
