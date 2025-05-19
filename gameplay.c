#include "header.h"

void tireTorpille(int x, int y) {
    for (int i = 0; i < MAX_TORPILLES; i++) {
        if (!torpilles[i].actif) {
            torpilles[i].x = x;
            torpilles[i].y = y;
            torpilles[i].actif = 1;
            break;
        }
    }
}

void tireBulle(int x, int y) {
    for (int i = 0; i < MAX_BULLES; i++) {
        if (!bulles[i].actif) {
            bulles[i].x = x;
            bulles[i].y = y;
            bulles[i].actif = 1;
            break;
        }
    }
}

void Banc_Poisson(int camera_x, BITMAP *collision) {
    int type = rand() % NB_TYPES_POISSONS;
    int taille_banc = 3 + rand() % 2;
    for (int i = 0; i < MAX_POISSONS && taille_banc > 0; i++) {
        if (!poissons[i].actif) {
            int x = camera_x + LARGEUR_ECRAN + rand() % 200;
            int y;
            int tentative = 0;
            do {
                y = rand() % (HAUTEUR_DECOR - poisson_imgs[type]->h);
                tentative++;
            } while (!positionPoissonValide(x, y, poisson_imgs[type], collision) && tentative < 10);

            if (tentative < 10) {
                poissons[i].type = type;
                poissons[i].x = x;
                poissons[i].y = y;
                poissons[i].actif = 1;
                poissons[i].clignote=0;
                poissons[i].temps_touche = 0;
                poissons[i].direction_y = (rand() % 3) - 1;
                poissons[i].delai_bulle = rand() % DELAI_BULLE;
                taille_banc--;
            }
        }
    }
}

void BonusLife(int camera_x, BITMAP *collision, BITMAP *coeur_img) {
    int tentative = 0;
    do {
        bonus.x = camera_x + LARGEUR_ECRAN + rand() % 200;
        bonus.y = rand() % (HAUTEUR_DECOR - coeur_img->h);
        tentative++;
    } while (!positionValide(bonus.x, bonus.y, collision, coeur_img) && tentative < 10);

    if (tentative < 10) {
        bonus.actif = 1;
        bonus.pris = 0;
    }
}

void ajouter_etoiles(int camera_x, BITMAP *collision) {
    for (int i = 0; i < MAX_ETOILES; i++) {
        if (!etoiles[i].actif) {
            int x = camera_x + LARGEUR_ECRAN + rand() % 200;
            int y = 0;

            for (int yy = 0; yy < HAUTEUR_DECOR - etoile_img->h; yy++) {
                int color = getpixel(collision, x, yy + etoile_img->h);
                if (getr(color) == 0 && getg(color) == 0 && getb(color) == 0) { // zone noire
                    y = yy;
                    break;
                }
            }

            etoiles[i].x = x;
            etoiles[i].y = y;
            etoiles[i].actif = 1;
            etoiles[i].delai_tir = rand() % DELAI_TIR_ETOILE;
            break;
        }
    }
}

void tirer_bulle_etoile(int x, int y) {
    for (int i = 0; i < MAX_BULLES_ETOILE; i++) {
        if (!bulles_etoile[i].actif) {
            bulles_etoile[i].x = x;
            bulles_etoile[i].y = y;
            bulles_etoile[i].actif = 1;
            break;
        }
    }
}




void jouer_niveau1(int progression_forcee) {
    srand(time(NULL));

    install_keyboard();
    set_color_depth(desktop_color_depth());
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, LARGEUR_ECRAN, HAUTEUR_ECRAN, 0, 0);

    BITMAP *buffer = create_bitmap(LARGEUR_ECRAN, HAUTEUR_ECRAN);
    BITMAP *decor = load_bitmap("biome1.bmp", NULL);
    BITMAP *collision = load_bitmap("collision1.bmp", NULL);
    BITMAP *vaisseau_img = load_bitmap("sousmarin2.bmp", NULL);
    BITMAP *torpille_img = load_bitmap("torpille.bmp", NULL);
    coeur_img = load_bitmap("coeur.bmp", NULL);
    bulle_img = load_bitmap("bulle.bmp", NULL);
    etoile_img = load_bitmap("etoile.bmp", NULL);

    for (int i = 0; i < MAX_ETOILES; i++) etoiles[i].actif = 0;
    for (int i = 0; i < MAX_BULLES_ETOILE; i++) bulles_etoile[i].actif = 0;
    int vie_temporaire = 100; // Jauge à 100%
    const int DEGAT_BULLE = 25; // Dégâts infligés par une bulle



    for (int i = 0; i < NB_TYPES_POISSONS; i++) {
        char nom[20];
        sprintf(nom, "poisson%d.bmp", i + 1);
        poisson_imgs[i] = load_bitmap(nom, NULL);

    }

    int vaisseau_x = 100, vaisseau_y = HAUTEUR_ECRAN / 2;
    int camera_x = 0, vies = NB_VIES, en_pause = 0;
    time_t debut_temps = time(NULL);
    int compteur_poisson = 0, clignote_vaisseau = 0;
    clock_t dernier_choc = 0;
    int prochain_palier = 30;

    bonus.actif = 0;
    bonus.pris = 0;

    for (int i = 0; i < MAX_TORPILLES; i++) torpilles[i].actif = 0;
    for (int i = 0; i < MAX_POISSONS; i++) poissons[i].actif = 0;
    for (int i = 0; i < MAX_BULLES; i++) bulles[i].actif = 0;

    while (!key[KEY_ESC] && vies > 0) {
        if (key[KEY_P]) { rest(200); en_pause = !en_pause; }

        if (!en_pause) {
            int new_x = vaisseau_x, new_y = vaisseau_y;
            if (key[KEY_RIGHT]) new_x += VITESSE_VAISSEAU;
            if (key[KEY_LEFT]) new_x -= VITESSE_VAISSEAU;
            if (key[KEY_UP]) new_y -= VITESSE_VAISSEAU;
            if (key[KEY_DOWN]) new_y += VITESSE_VAISSEAU;
            if (new_x < camera_x) new_x = camera_x;
            if (new_x > camera_x + LARGEUR_ECRAN - vaisseau_img->w)
                new_x = camera_x + LARGEUR_ECRAN - vaisseau_img->w;

            if (positionValide(new_x, new_y, collision, vaisseau_img)) {
                vaisseau_x = new_x;
                vaisseau_y = new_y;
            } else if (clock() - dernier_choc > 1000) {
                vies--;
                clignote_vaisseau = 1;
                dernier_choc = clock();
            }
            static int tir_delay = 0;
            if (key[KEY_SPACE]) {
                tireTorpille(vaisseau_x + vaisseau_img->w, vaisseau_y + vaisseau_img->h / 2);
                tir_delay = 10;  // délai entre tirs
            }
            if (tir_delay > 0) tir_delay--;

            for (int i = 0; i < MAX_TORPILLES; i++) {
                if (torpilles[i].actif) {
                    torpilles[i].x += 8;
                    if (torpilles[i].x > LARGEUR_DECOR) torpilles[i].actif = 0;
                }
            }

            for (int i = 0; i < MAX_BULLES; i++) {
                if (bulles[i].actif) {
                    bulles[i].x -= 5;
                    if (bulles[i].x < 0) bulles[i].actif = 0;

                    if (!clignote_vaisseau &&
                        bulles[i].x > vaisseau_x && bulles[i].x < vaisseau_x + vaisseau_img->w &&
                        bulles[i].y > vaisseau_y && bulles[i].y < vaisseau_y + vaisseau_img->h) {

                        vie_temporaire -= DEGAT_BULLE;
                        if (vie_temporaire <= 0) {
                            vies--;
                            vie_temporaire = 100;
                            clignote_vaisseau = 1;
                            dernier_choc = clock();
                        }
                        bulles[i].actif = 0;
                    }
                }
            }

            compteur_poisson++;
            if (compteur_poisson > 150) {
                Banc_Poisson(camera_x, collision);
                compteur_poisson = 0;
            }

            for (int i = 0; i < MAX_POISSONS; i++) {
                if (poissons[i].actif) {
                    poissons[i].x -= 2;
                    poissons[i].y += poissons[i].direction_y;
                    if (!positionPoissonValide(poissons[i].x, poissons[i].y, poisson_imgs[poissons[i].type], collision)) {
                        poissons[i].direction_y *= -1;
                        poissons[i].y += poissons[i].direction_y * 2;
                    }

                    poissons[i].delai_bulle--;
                    if (poissons[i].delai_bulle <= 0) {
                        tireBulle(poissons[i].x, poissons[i].y + poisson_imgs[poissons[i].type]->h / 2);
                        poissons[i].delai_bulle = DELAI_BULLE;
                    }

                    for (int j = 0; j < MAX_TORPILLES; j++) {
                        if (torpilles[j].actif) {
                            BITMAP *img = poisson_imgs[poissons[i].type];
                            if (torpilles[j].x > poissons[i].x && torpilles[j].x < poissons[i].x + img->w &&
                                torpilles[j].y > poissons[i].y && torpilles[j].y < poissons[i].y + img->h) {
                                poissons[i].clignote = 1;
                                poissons[i].temps_touche = clock();
                                torpilles[j].actif = 0;
                            }
                        }
                    }
                    if (poissons[i].clignote && clock() - poissons[i].temps_touche > TEMPS_CLIGNOTE * CLOCKS_PER_SEC / 1000)
                        poissons[i].actif = 0;
                    if (poissons[i].x < -100) poissons[i].actif = 0;

                    if (!clignote_vaisseau &&
                        poissons[i].x + poisson_imgs[poissons[i].type]->w > vaisseau_x &&
                        poissons[i].x < vaisseau_x + vaisseau_img->w &&
                        poissons[i].y + poisson_imgs[poissons[i].type]->h > vaisseau_y &&
                        poissons[i].y < vaisseau_y + vaisseau_img->h) {
                        vies--;
                        clignote_vaisseau = 1;
                        dernier_choc = clock();
                        }
                }
            }

            for (int i = 0; i < MAX_ETOILES; i++) {
                if (etoiles[i].actif) {
                    for (int j = 0; j < MAX_TORPILLES; j++) {
                        if (torpilles[j].actif) {
                            if (torpilles[j].x > etoiles[i].x && torpilles[j].x < etoiles[i].x + etoile_img->w &&
                                torpilles[j].y > etoiles[i].y && torpilles[j].y < etoiles[i].y + etoile_img->h) {
                                etoiles[i].actif = 0;         // L'étoile est détruite
                                torpilles[j].actif = 0;       // La torpille est détruite
                                }
                        }
                    }
                }
            }

            static int compteur_etoiles = 0;
            compteur_etoiles++;
            if (compteur_etoiles > 200) {
                ajouter_etoiles(camera_x, collision);
                compteur_etoiles = 0;
            }

            for (int i = 0; i < MAX_ETOILES; i++) {
                if (etoiles[i].actif) {

                    etoiles[i].delai_tir--;
                    if (etoiles[i].delai_tir <= 0) {
                        tirer_bulle_etoile(etoiles[i].x + etoile_img->w / 2, etoiles[i].y);
                        etoiles[i].delai_tir = DELAI_TIR_ETOILE;
                    }

                    if (etoiles[i].x < -etoile_img->w)
                        etoiles[i].actif = 0;
                }
            }

            for (int i = 0; i < MAX_BULLES_ETOILE; i++) {
                if (bulles_etoile[i].actif) {
                    bulles_etoile[i].y -= 5;
                    if (bulles_etoile[i].y < 0)
                        bulles_etoile[i].actif = 0;

                    if (!clignote_vaisseau &&
                        bulles_etoile[i].x > vaisseau_x && bulles_etoile[i].x < vaisseau_x + vaisseau_img->w &&
                        bulles_etoile[i].y > vaisseau_y && bulles_etoile[i].y < vaisseau_y + vaisseau_img->h) {

                        vie_temporaire -= DEGAT_BULLE;
                        if (vie_temporaire <= 0) {
                            vies--;
                            vie_temporaire = 100;
                            clignote_vaisseau = 1;
                            dernier_choc = clock();
                        }
                        bulles_etoile[i].actif = 0;
                        }

                }
            }

            int progression = vaisseau_x * 100 / LARGEUR_DECOR;
            if (progression >= prochain_palier && !bonus.actif) {
                BonusLife(camera_x, collision, coeur_img);
                prochain_palier += 30;
            }

            if (bonus.actif && !bonus.pris &&
                vaisseau_x + vaisseau_img->w > bonus.x &&
                vaisseau_x < bonus.x + coeur_img->w &&
                vaisseau_y + vaisseau_img->h > bonus.y &&
                vaisseau_y < bonus.y + coeur_img->h) {
                bonus.pris = 1;
                bonus.actif = 0;
                if (vies < NB_VIES) vies++;
            }

            camera_x += VITESSE_SCROLL;
            if (camera_x > LARGEUR_DECOR - LARGEUR_ECRAN)
                camera_x = LARGEUR_DECOR - LARGEUR_ECRAN;
        }
        int progression = vaisseau_x * 100 / LARGEUR_DECOR;
        if (progression >= 99) {
            gestionVictoire(1);  // Appelle la fonction avec le niveau en cours
            return; // Termine la fonction et revient au menu
        }



        blit(decor, buffer, camera_x, 0, 0, 0, LARGEUR_ECRAN, HAUTEUR_ECRAN);
        int vx_aff = vaisseau_x - camera_x;

        if (!clignote_vaisseau || ((clock() / 100) % 2 == 0))
            masked_blit(vaisseau_img, buffer, 0, 0, vx_aff, vaisseau_y, vaisseau_img->w, vaisseau_img->h);
        if (clock() - dernier_choc > 1000) clignote_vaisseau = 0;

        for (int i = 0; i < MAX_TORPILLES; i++) {
            if (torpilles[i].actif) {
                int tx = torpilles[i].x - camera_x;
                if (tx >= 0 && tx < LARGEUR_ECRAN)
                    masked_blit(torpille_img, buffer, 0, 0, tx, torpilles[i].y, torpille_img->w, torpille_img->h);
            }
        }

        for (int i = 0; i < MAX_BULLES; i++) {
            if (bulles[i].actif) {
                int bx = bulles[i].x - camera_x;
                if (bx >= 0 && bx < LARGEUR_ECRAN)
                    masked_blit(bulle_img, buffer, 0, 0, bx, bulles[i].y, bulle_img->w, bulle_img->h);
            }
        }

        for (int i = 0; i < MAX_POISSONS; i++) {
            if (poissons[i].actif) {
                int px = poissons[i].x - camera_x;
                if (!poissons[i].clignote || ((clock() / 100) % 2 == 0))
                    masked_blit(poisson_imgs[poissons[i].type], buffer, 0, 0, px, poissons[i].y,
                                poisson_imgs[poissons[i].type]->w, poisson_imgs[poissons[i].type]->h);
            }
        }

        if (bonus.actif && !bonus.pris) {
            int bx = bonus.x - camera_x;
            if (bx >= 0 && bx < LARGEUR_ECRAN)
                masked_blit(coeur_img, buffer, 0, 0, bx, bonus.y, coeur_img->w, coeur_img->h);
        }

        for (int i = 0; i < vies; i++)
            masked_blit(coeur_img, buffer, 0, 0, 10 + i * 35, 10, coeur_img->w, coeur_img->h);
        // Affichage de la jauge de vie
        int largeur_jauge_max = 100;
        int hauteur_jauge = 10;
        int x_jauge = 10;
        int y_jauge = 10 + coeur_img->h + 5;
        int largeur_jauge = largeur_jauge_max * vie_temporaire / 100;

        rectfill(buffer, x_jauge, y_jauge, x_jauge + largeur_jauge, y_jauge + hauteur_jauge, makecol(255, 0, 0));
        rect(buffer, x_jauge, y_jauge, x_jauge + largeur_jauge_max, y_jauge + hauteur_jauge, makecol(255, 255, 255));

        int c = vaisseau_x * 100 / LARGEUR_DECOR;
        int temps_ecoule = (int)(time(NULL) - debut_temps);
        textprintf_ex(buffer, font, 350, 50, makecol(255, 255, 255), -1, "Niveau 1 : %d %%", c);
        textprintf_ex(buffer, font, LARGEUR_ECRAN - 120, 10, makecol(255, 255, 255), -1, "Temps: %ds", temps_ecoule);

        if (en_pause)
            textprintf_centre_ex(buffer, font, LARGEUR_ECRAN / 2, HAUTEUR_ECRAN / 2, makecol(255, 255, 255), -1, "== PAUSE ==");

        for (int i = 0; i < MAX_ETOILES; i++) {
            if (etoiles[i].actif) {
                int ex = etoiles[i].x - camera_x;
                if (ex >= 0 && ex < LARGEUR_ECRAN)
                    masked_blit(etoile_img, buffer, 0, 0, ex, etoiles[i].y, etoile_img->w, etoile_img->h);
            }
        }

        for (int i = 0; i < MAX_BULLES_ETOILE; i++) {
            if (bulles_etoile[i].actif) {
                int bx = bulles_etoile[i].x - camera_x;
                if (bx >= 0 && bx < LARGEUR_ECRAN)
                    masked_blit(bulle_img, buffer, 0, 0, bx, bulles_etoile[i].y, bulle_img->w, bulle_img->h);
            }
        }


        blit(buffer, screen, 0, 0, 0, 0, LARGEUR_ECRAN, HAUTEUR_ECRAN);
        rest(20);
    }
    // Affiche "GAME OVER" si toutes les vies sont perdues
    if (vies <= 0) {
        // Réaffiche le dernier décor (fond)
        blit(decor, buffer, camera_x, 0, 0, 0, LARGEUR_ECRAN, HAUTEUR_ECRAN);

        // Réaffiche le vaisseau à sa dernière position
        int vx_aff = vaisseau_x - camera_x;
        masked_blit(vaisseau_img, buffer, 0, 0, vx_aff, vaisseau_y, vaisseau_img->w, vaisseau_img->h);

        // Réaffiche les cœurs (nombre de vies restantes = 0, donc pas nécessaire, mais pour style tu peux afficher vides)
        // masked_blit(coeur_img, buffer, 0, 0, 10, 10, coeur_img->w, coeur_img->h); // optionnel

        // Texte "GAME OVER" en rouge au milieu
        textprintf_centre_ex(buffer, font, LARGEUR_ECRAN / 2, HAUTEUR_ECRAN / 2,
                             makecol(255, 255, 255), -1, " G A M E   O V E R ");

        // Affiche le buffer final
        blit(buffer, screen, 0, 0, 0, 0, LARGEUR_ECRAN, HAUTEUR_ECRAN);
        rest(2000);
    }


    menu();
    destroy_bitmap(buffer);
    destroy_bitmap(decor);
    destroy_bitmap(collision);
    destroy_bitmap(vaisseau_img);
    destroy_bitmap(torpille_img);
    destroy_bitmap(coeur_img);
    destroy_bitmap(bulle_img);
    destroy_bitmap(etoile_img);

    for (int i = 0; i < NB_TYPES_POISSONS; i++) destroy_bitmap(poisson_imgs[i]);
}
