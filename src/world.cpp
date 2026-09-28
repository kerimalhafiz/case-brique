#include "world.hpp"
using namespace std;
#include <iostream>
#include <new>
#include <fstream>



void init_world(World* world,int w,int h){

world->height = h;
world->width = w;
world->grid = new Block[h*w];
world->hit_count = new int[h * w];

for (int i=0;i<h*w;i++){
    world->grid[i] = Empty;
    world->hit_count[i]=0;
}
}

void free_world(World* world){
        delete []world->grid;
    
}


Block read(World* world, int x, int y){
      if (x<0|| x>=world->width|| y<0 || y>=world->height)
      {
           cout<<"erreur"<<x<<","<<y;
           return Empty;
      }
         int i=y*world->width+x;
         return world->grid[i];
      
}
void write(World* world, int x, int y, Block b){
    if (x<0|| x>=world->width|| y<0 || y>=world->height)
      {
           cout<<"erreur"<<x<<","<<y;
      }
      int i=y*world->width+x;
      world->grid[i]=b;
}
void display(World* world){
    char c;// caractere par defaut je ss pas il m'affiche que je dois l'initialiser avant de l'utiliser
    for(int i=0; i<world->height; i++){
        for(int j=0; j<world->width; j++){
            c=';';
            Block b = read(world,j,i);  
    switch(b){
        case Empty:
            c='.';
            break;
        case Border:
            c ='#';
            break;
        case Lose:
            c='$';
            break;
        case Type1:
            c='1';
            break;
        case Type2:
            c='2';
            break;
        default:
            break;
    }
    cout<< c;

        }
        cout<<endl;
    }

}
void init_world_from_file(World* world, string filename) {
    char c;
    ifstream fic(filename);
    
    if (!fic) {
        cout << "Erreur: Impossible d'ouvrir le fichier " << filename << endl;
        return;
    }

    int h, w;
    fic >> w >> h;
    init_world(world, w, h);

    for (int i = 0; i < world->height; i++) {
        for (int j = 0; j < world->width; j++) {
            fic >> c;  
            Block b=Border;
            switch (c) {
                case '.':
                b = Empty; 
                break;
                case '#': 
                b = Border;
                 break;
                case '$':
                 b = Lose; 
                 break;
                case '1': 
                b = Type1; 
                break;
                case '2':
                 b = Type2;
                  break;
                default:
                break;
            }

            write(world, j, i, b); 
        }
    }
    fic.close();
}
void render_world(World* world, Window* window) {
    int blockwidth = window->width / world->width;
    int blockheight = window->height / world->height;
    
    SDL_Rect rec;
    rec.h = blockheight;
    rec.w = blockwidth;



    for (int i = 0; i < world->height; i++) {
        for (int j = 0; j < world->width; j++) {
            rec.x = j * blockwidth;
            rec.y = i * blockheight;

            Block b = world->grid[i * world->width + j];

            switch (b) {
                case Empty:  
                    set_color(&window->foreground, 0, 0, 0, 255);
                    break;   
                case Border:
                    set_color(&window->foreground, 128, 128, 128, 255);
                    break;
                case Type1:  
                set_color(&window->foreground, 255, 0, 0, 255);
                    break;     
                case Type2: 
               set_color(&window->foreground, 0, 255, 0, 255);
                    break;    
                case Lose:  
                set_color(&window->foreground, 0, 0, 0, 255);
                    break;       
                default:
                    break;
            }

            draw_fill_rectangle(window,rec.x,rec.y,rec.w,rec.h);
        }
    }

    
}
