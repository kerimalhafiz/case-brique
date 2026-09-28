#ifndef WORLD_HPP
#define WORLD_HPP

#include "window.hpp"
#include <iostream>

using namespace std;
enum Block{
    Empty, Border, Type1, Type2, Lose 

};

struct World
{
    int width;//largeur
    int height;//hauteur
    Block*grid;
    int* hit_count; // Nouveau tableau pour suivre le nombre de coups pour chaque bloc
};
enum nom_enum{
val1,val2,val3
};


void init_world(World* world, int w, int h);
void free_world(World* world);
Block read(World* world, int x, int y);
void write(World* world, int x, int y, Block b);
void display(World *wordl);
void render_world(World* world, Window* Window);
void init_world_from_file(World* world, string filename);

#endif
