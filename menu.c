#include "header.h"

char pseudo[MAX_PSEUDO] = "";
int niveau_debloque = 1;
BITMAP* fond_menu = NULL;

void textout_gros(BITMAP* bmp, FONT* f, const char* txt, int x, int y, int color) {
    for (int dx = 0; dx < 2; dx++) {
        for (int dy = 0; dy < 2; dy++) {
            textout_centre_ex(bmp, f, txt, x + dx, y + dy, color, -1);
        }
    }
}

void sauvegarder_progression(const char* pseudo, int niveau) {
    FILE* f = fopen(FICHIER_SAUVEGARDE, "r");
    FILE* temp = fopen("temp.txt", "w");
    int trouve = 0;
    char p[MAX_PSEUDO];
    int n;
    if (f && temp) {
        while (fscanf(f, "%s %d", p, &n) == 2) {
            if (strcmp(pseudo, p) == 0) {
                fprintf(temp, "%s %d\n", pseudo, niveau);
                trouve = 1;
            } else {
                fprintf(temp, "%s %d\n", p, n);
            }
        }
        if (!trouve)
            fprintf(temp, "%s %d\n", pseudo, niveau);
        fclose(f);
        fclose(temp);
        remove(FICHIER_SAUVEGARDE);
        rename("temp.txt", FICHIER_SAUVEGARDE);
    } else if (temp) {
        fprintf(temp, "%s %d\n", pseudo, niveau);
        fclose(temp);
    }
}

int charger_progression(const char* pseudo_cherche, int* niveau_retourne) {
    FILE* f = fopen(FICHIER_SAUVEGARDE, "r");
    if (!f) return 0;

    char p[MAX_PSEUDO];
    int n;
    while (fscanf(f, "%s %d", p, &n) == 2) {
        if (strcmp(pseudo_cherche, p) == 0) {
            *niveau_retourne = n;
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void dessiner_ecran_menu(const char* titre, const char* options[], int n, int selection) {
    if (fond_menu) blit(fond_menu, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    else clear_to_color(screen, makecol(0, 0, 0));

    textout_gros(screen, font, titre, SCREEN_W / 2, SCREEN_H / 2 - 100, makecol(255, 255, 255));
    for (int i = 0; i < n; i++) {
        int color = (i == selection) ? makecol(255, 255, 0) : makecol(255, 255, 255);
        textout_gros(screen, font, options[i], SCREEN_W / 2, SCREEN_H / 2 - 40 + i * 40, color);
    }
}

int saisir_pseudo_utilisateur(char* destination) {
    char buffer[MAX_PSEUDO] = "";
    int index = 0;

    while (1) {
        if (fond_menu) blit(fond_menu, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        else clear_to_color(screen, makecol(0, 0, 0));

        textout_gros(screen, font, "Entrez votre pseudo :", SCREEN_W / 2, 200, makecol(255, 255, 255));
        textout_gros(screen, font, "(ECHAP pour annuler)", SCREEN_W / 2, 250, makecol(150, 150, 150));
        textout_gros(screen, font, buffer, SCREEN_W / 2, 300, makecol(255, 255, 0));

        if (keypressed()) {
            int k = readkey();
            if ((k >> 8) == KEY_ENTER && index > 0) {
                strcpy(destination, buffer);
                return 1;
            } else if ((k >> 8) == KEY_ESC) {
                return 0;
            } else {
                k = k & 0xff;
                if (k >= 32 && k <= 126 && index < MAX_PSEUDO - 1) {
                    buffer[index++] = k;
                    buffer[index] = '\0';
                } else if (k == 8 && index > 0) {
                    buffer[--index] = '\0';
                }
            }
        }
        rest(30);
    }
}

void menu_niveaux() {
    const char* options[] = {
        "Jouer Niveau 1",
        "Jouer Niveau 2",
        "Jouer Niveau 3",
        "Retour"
    };
    int n_options = 4;
    int selection = 0;

    while (1) {
        dessiner_ecran_menu("Choisissez un niveau :", options, n_options, selection);

        if (key[KEY_DOWN]) { rest(150); selection = (selection + 1) % n_options; }
        if (key[KEY_UP]) { rest(150); selection = (selection - 1 + n_options) % n_options; }

        if (key[KEY_ENTER]) {
            rest(200);

            if (selection == 3) return;  // Retour

            if (selection + 1 <= niveau_debloque) {
                allegro_message("Lancement du niveau %d...", selection + 1);

                // Appelle le bon niveau selon la sélection
                if (selection == 0) {
                    jouer_niveau1();
                }


                // Met à jour la progression
                if (selection + 1 > niveau_debloque) {
                    niveau_debloque = selection + 1;
                    sauvegarder_progression(pseudo, niveau_debloque);
                }
            } else {
                allegro_message("Niveau verrouillé !");
            }
        }

        if (key[KEY_ESC]) return;

        rest(30);
    }
}

int menu() {

    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 546, 0, 0);

    fond_menu = load_bitmap("backmenu.bmp", NULL);
    if (!fond_menu) {
        allegro_message("Erreur de chargement de l'image de fond !");
        return 1;
    }

    const char* options[] = {
        "Nouvelle Partie",
        "Reprendre Partie",
        "Quitter"
    };
    int selection = 0;

    while (1) {
        dessiner_ecran_menu("MENU PRINCIPAL", options, 3, selection);

        if (key[KEY_DOWN]) { rest(150); selection = (selection + 1) % 3; }
        if (key[KEY_UP]) { rest(150); selection = (selection - 1 + 3) % 3; }

        if (key[KEY_ENTER]) {
            rest(200);
            if (selection == 0) {
                if (saisir_pseudo_utilisateur(pseudo)) {
                    niveau_debloque = 1;
                    sauvegarder_progression(pseudo, niveau_debloque);
                    menu_niveaux();
                }
            } else if (selection == 1) {
                if (saisir_pseudo_utilisateur(pseudo)) {
                    if (charger_progression(pseudo, &niveau_debloque)) {
                        allegro_message("Bienvenue %s ! Dernier niveau atteint : %d", pseudo, niveau_debloque);
                        menu_niveaux();
                    } else {
                        allegro_message("Pseudo inconnu !");
                    }
                }
            } else if (selection == 2) {
                break;
            }
        }

        if (key[KEY_ESC]) break;
        rest(30);
    }

    destroy_bitmap(fond_menu);
    allegro_exit();
    return 0;
}
END_OF_MENU();
