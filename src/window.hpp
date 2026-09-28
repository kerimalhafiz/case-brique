#include <SDL2/SDL_image.h>

#include <SDL2/SDL_ttf.h>

#ifndef   WINDOW_HPP
#define   WINDOW_HPP
#include <iostream>
#include <SDL2/SDL.h>
using namespace std;

struct Window {
    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;
    int width;
    int height;
    SDL_Color background;
    SDL_Color foreground;
    TTF_Font* sdl_font;

};
struct Ball{
    int x;
    int y;
    int dx,dy;
    int speed;
    SDL_Texture* texture;
};

SDL_Texture* load_image(Window* window, string image);
void draw_text(Window* window, string text, int x, int y);
void draw_texture(Window* window, SDL_Texture* texture, int x, int y, int w, int h);
void init_window(Window* window, int width, int height, string title);
void close_window(Window* window);
void set_color(SDL_Color* dst, int r, int g, int b, int a);
void set_color(SDL_Color* dst, SDL_Color* src);
void clear_window(Window* window);
void draw_fill_rectangle(Window* window, int x, int y, int w, int h);
void refresh_window(Window* window);
void init_ball(Ball *ball,Window *window,string image);
void update(Ball* ball, int width, int height, int speed);
void draw_ball(Window* window,Ball *ball);

#endif
