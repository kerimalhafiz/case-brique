#include <iostream>
#include "world.hpp"  
#include "window.hpp"
#include"game.hpp"
using namespace std;

int main() {
    Window window;
    Game game;
    int cpt =0;
    bool quit = false;
    init_window(&window, 800, 600, "casse brique");
    init_game(&game, "world.dat");
    while (!quit){
        if(game.statut == Begin){
            move_ball(&game);
        }
        else if (game.statut == Play  && cpt == 20){
            move_ball(&game);
            cpt = 0;
            game.ball_speed++;
        }
        else if (game.statut == Win) {
            draw_text(&window, "VICTOIRE !", 300, 300);
            refresh_window(&window);
        }
        if (game.statut == GameOver) {
            draw_text(&window, " game over", 300, 300);
            refresh_window(&window);
        }
        quit = keyboard_event(&game);
        display_game(&window, &game);
        refresh_window(&window);
        if (game.statut == Play){
            cpt+=1;
        }
        SDL_Delay(10);
    }
    
    display_game(&window, &game);
    free_world(game.world);
    close_window(&window);
}