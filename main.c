#include "header.h"


int main() {
    allegro_init();
    install_keyboard();
    set_color_depth(desktop_color_depth());
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, LARGEUR_ECRAN, HAUTEUR_ECRAN, 0, 0);

    menu();

    allegro_exit();
    return 0;
}
END_OF_MAIN();
