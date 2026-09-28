#ifndef GAME_HPP
#define GAME_HPP

#include "world.hpp"
#include "window.hpp"



enum Statut{
    Begin,Play,Pause,Win,GameOver
};
struct Game{
    World* world;
    SDL_Color empty_color={0,0,0,255};
    SDL_Color border_color={128, 128, 128, 255};
    SDL_Color lose_color={0,0,0,255};
    SDL_Color typ1_color={255, 0, 0, 255};
    SDL_Color typ2_color={ 0, 255, 0, 255};
    int racket_x;
    int racket_y;
    int racket_half_width;
    SDL_Color racket_color= {135, 206, 235};
    int ball_x,ball_y,ball_dx,ball_dy;
    SDL_Color  ball_color={200,200,200,195};
    int ball_speed;
    Statut statut;
    int score;
    SDL_Color score_color;
    
};

void init_game(Game* game, string filename);
void display_game(Window* window, Game* game);
bool keyboard_event(Game* game);
void move_racket(Game* game, int d);
void move_ball(Game* game);
void  change_statut(Game *game);
void verif_bord(Game* game);
bool casse_dublock(World* world) ;
#endif
