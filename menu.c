#include "header.h"

char pseudo[MAX_PSEUDO] = "";
int niveau_debloque = 1;
BITMAP* fond_menu = NULL;

void textegros(BITMAP* bmp, FONT* f, const char* txt, int x, int y, int color) {//fonction pour afficher en plus gros le texte
    for (int dx = 0; dx < 2; dx++) {
        for (int dy = 0; dy < 2; dy++) {
            textout_centre_ex(bmp, f, txt, x + dx, y + dy, color, -1);
        }
    }
}

void sauvegarde(const char* pseudo, int niveau) {//sauvegarde la progression du joueur
    FILE* f = fopen(FICHIER_SAUVEGARDE, "r");//ouvre le fichier de sauvegarde
    FILE* temp = fopen("temp.txt", "w");
    int trouve = 0;
    char p[MAX_PSEUDO];
    int n;
    if (f && temp) {
        while (fscanf(f, "%s %d", p, &n) == 2) {
            if (strcmp(pseudo, p) == 0) {//compare le pseudo saisie avec les pseudos sauvegarder pour trouver la partie
                fprintf(temp, "%s %d\n", pseudo, niveau);
                trouve = 1;
            } else {
                fprintf(temp, "%s %d\n", p, n);
            }
        }
        if (!trouve)//si non trouvé alors le fichier est fermé
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

int chargementPartie(const char* pseudo_cherche, int* niveau_retourne) {//pour charger un niveau deja joué
    FILE* f = fopen(FICHIER_SAUVEGARDE, "r");
    if (!f) return 0;

    char p[MAX_PSEUDO];
    int n;
    while (fscanf(f, "%s %d", p, &n) == 2) {
        if (strcmp(pseudo_cherche, p) == 0) {//compare le pseudo saisie avec les pseudos sauvegarder pour trouver la partie
            *niveau_retourne = n;
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void EcranMenu(const char* titre, const char* options[], int n, int selection) {
    if (fond_menu) blit(fond_menu, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    else clear_to_color(screen, makecol(0, 0, 0));

    textegros(screen, font, titre, SCREEN_W / 2, SCREEN_H / 2 - 100, makecol(255, 255, 255));
    for (int i = 0; i < n; i++) {
        int color = (i == selection) ? makecol(255, 255, 0) : makecol(255, 255, 255);
        textegros(screen, font, options[i], SCREEN_W / 2, SCREEN_H / 2 - 40 + i * 40, color);
    }
}

int GestionPseudo(char* destination) {
    char buffer[MAX_PSEUDO] = "";
    int index = 0;

    while (1) {
        if (fond_menu) blit(fond_menu, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        else clear_to_color(screen, makecol(0, 0, 0));

        textegros(screen, font, "Entrez votre pseudo :", SCREEN_W / 2, 200, makecol(255, 255, 255));
        textegros(screen, font, "(ECHAP pour annuler)", SCREEN_W / 2, 250, makecol(150, 150, 150));
        textegros(screen, font, buffer, SCREEN_W / 2, 300, makecol(255, 255, 0));

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

void gestionNiveaux() {
    clear_keybuf();

    const char* options[] = {
        "Jouer Niveau 1",
        "Jouer Niveau 2",
        "Jouer Niveau 3",
        "Retour"
    };
    int n_options = 4;
    int selection = 0;

    while (1) {
        EcranMenu("Choisissez un niveau :", options, n_options, selection);

        if (keypressed()) {
            int k = readkey();
            int scancode = k >> 8;

            if (scancode == KEY_DOWN) {
                selection = (selection + 1) % n_options;
            } else if (scancode == KEY_UP) {
                selection = (selection - 1 + n_options) % n_options;
            } else if (scancode == KEY_ENTER) {
                if (selection == 3) return;  // Retour

                if (selection + 1 <= niveau_debloque) {
                    allegro_message("Lancement du niveau %d...", selection + 1);

                    if (selection == 0) {
                        jouer_niveau1();
                        if (niveau_debloque < 2) {
                            niveau_debloque = 2;
                            sauvegarde(pseudo, niveau_debloque);
                        }
                    } else if (selection == 1) {
                        jouer_niveau2();
                        if (niveau_debloque < 3) {
                            niveau_debloque = 3;
                            sauvegarde(pseudo, niveau_debloque);
                        }
                    } else if (selection == 2) {
                        jouer_niveau3();
                    }
                } else {
                    allegro_message("Niveau verrouillé !");
                }
            } else if (scancode == KEY_ESC) {
                return;
            }
        }

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
        EcranMenu("MENU PRINCIPAL", options, 3, selection);

        if (key[KEY_DOWN]) { rest(150); selection = (selection + 1) % 3; }
        if (key[KEY_UP]) { rest(150); selection = (selection - 1 + 3) % 3; }

        if (key[KEY_ENTER]) {
            rest(200);
            if (selection == 0) {
                if (GestionPseudo(pseudo)) {
                    niveau_debloque = 1;
                    sauvegarde(pseudo, niveau_debloque);
                    gestionNiveaux();
                }
            } else if (selection == 1) {
                if (GestionPseudo(pseudo)) {
                    if (chargementPartie(pseudo, &niveau_debloque)) {
                        allegro_message("Bienvenue %s ! Dernier niveau atteint : %d", pseudo, niveau_debloque);
                        gestionNiveaux();
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
