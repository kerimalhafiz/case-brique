using namespace std;
#include "world.hpp"
#include"game.hpp"



void init_game(Game* game, string filename) {
    game->world = new World;  
    init_world_from_file(game->world, filename);  
    

    if (game->world->height < 2) {
        cout << " La hauteur du monde est trop petite pour la raquette" << endl;
        return;
    }

    game->racket_y = game->world->height - 2;  
    game->racket_x = game->world->width / 2;   
    game->racket_half_width = 3;  
    game->ball_x = game->racket_x;
    game->ball_y = game->racket_y-1;
    game->ball_dx = 0;
    game->ball_dy = -1;
    game->ball_speed=50;
    game->statut=Begin;
    game->score=0;

}
void display_game(Window* window, Game* game){
    int col = game->world->width;
    int lig = game->world->height;
    int dimmondedivcol=800/col;
    int dimmondedivlig=600/lig;
    SDL_Color bl;
    for(int i = 0; i < lig ; i++){
        for(int k = 0; k < col; k++){
            switch(game->world->grid[i*col+k]){
                case Empty:
                    bl = game->empty_color;
                    break;
                case Border:
                    bl = game->border_color;
                    break;
                case Lose:
                    bl = game->lose_color;
                    break;
                case Type1:
                    bl = game->typ1_color;
                    break;
                case Type2:
                    bl = game->typ2_color;
                    break;
                default :
                    bl = game->empty_color;
                    cerr << "bloc inconnu";
            }
            set_color(&window->foreground, &bl);
            draw_fill_rectangle(window, k*dimmondedivcol, i*dimmondedivlig, dimmondedivcol, dimmondedivlig);
        }
    }
    set_color(&window->foreground, &game->racket_color);
    draw_fill_rectangle(window, game->racket_x*dimmondedivcol, game->racket_y*dimmondedivlig, 4*dimmondedivcol, dimmondedivlig);
    draw_fill_rectangle(window, (game->racket_x-3)*dimmondedivcol, game->racket_y*dimmondedivlig, 3*dimmondedivcol, dimmondedivlig);
    set_color(&window->foreground, &game->ball_color);
    draw_fill_rectangle(window, game->ball_x*dimmondedivcol, game->ball_y*dimmondedivlig, dimmondedivcol, dimmondedivlig);
    string score = "Score: " + to_string(game->score);
    switch (game->statut)
    {
    case GameOver:
          game->score_color={255, 0, 0, 255};
        break;
    case Win:
         game->score_color={0, 255, 0, 255};
         break;
    default:
            game->score_color = {255, 255, 255, 255};
        break;
    }
    set_color(&window->foreground, &game->score_color);
    draw_text(window, score, 10, 10);
}


bool keyboard_event(Game* game) {
  SDL_Event event;

  while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
          return true;  
      } else if (event.type == SDL_KEYDOWN) {
          SDL_KeyboardEvent key_event = event.key;

          switch (key_event.keysym.sym) {
              case SDLK_q:
                  cout << "Touche 'Q' pressée, le programme s'arrête." << endl;
                  return true;

              case SDLK_r:
                  cout << "Touche 'R' pressée, réinitialisation du jeu." << endl;
                  free_world(game->world);
                  delete game->world;
                  init_game(game, "world.dat");
                  break;

              case SDLK_LEFT:
                   cout<<"la touche du  gauche deplacement -1"<<endl;
                  move_racket(game, -1);
                  break;

              case SDLK_RIGHT:
              cout<<"la touche du droite deplacement +1"<<endl;
                  move_racket(game, 1);
                  break;

              case SDLK_SPACE:
                 cout<<"premier appuis lancement du jeu(la seconde vous permettre de mettre en pause)"<<endl;
                  change_statut(game);
                  break;

              default:
                  break;
          }
      }
  }
  return false;  
}

void move_racket(Game* game, int d){
    if (game->statut == Begin || game->statut == Play){ // la raquette ne se déplace que si 
    switch(d){                                          // la partie n'a pas encore commencé
        case 1:                                         // ou si elle est en cours
            if (game->racket_x<game->world->width-5){
                game->racket_x++;
            } // Si le milieu de la raquette se trouve à plus de 5 blocs de la bordure droite, on peut la déplacer
            break;
        case -1:
            if (game->racket_x>4){
                game->racket_x--;
            } // si le milieu de la raquette se trouve à plus de 4 blocs de la bordure gauche, on peut la déplacer
            break;
        default:
            break;
        }
    }
}

void move_ball(Game* game) {
    switch (game->statut){ 
        case Begin: // Si début de partie, la balle reste sur la raquette et suit son déplacement
            game->ball_x = game->racket_x;
            game->ball_y = game->racket_y-1;
            break;
        case Play: // Si partie en cours, la balle se déplace selon son vecteur de déplacement
            game->ball_x+= game->ball_dx;
            game->ball_y += game->ball_dy;
            break;
        default:
            break;
    }
    verif_bord(game);
}
void verif_bord(Game* game) {
    int x = game->ball_x;
    int y = game->ball_y;
    World* world = game->world;

    // Vérification des collisions avec les bords du monde (perdant, etc.)
    if (read(world, x, y) == Lose) {
        game->statut = GameOver;
        return;
    }

    bool casse = false;
    bool haut = (y > 0) && (read(world, x, y - 1) != Empty && read(world, x, y - 1) != Lose);
    bool bas = (y < world->height - 1) && (read(world, x, y + 1) != Empty && read(world, x, y + 1) != Lose);
    bool gauche = (x > 0) && (read(world, x - 1, y) != Empty && read(world, x - 1, y) != Lose);
    bool droite = (x < world->width - 1) && (read(world, x + 1, y) != Empty && read(world, x + 1, y) != Lose);

    // Collision avec la raquette
    if (y == game->racket_y - 1 && x >= game->racket_x - game->racket_half_width && x < game->racket_x + game->racket_half_width + 4) {
        // Rebond sur la raquette (modification de la direction verticale)
        game->ball_dy = -game->ball_dy;

        // Rebond réaliste : changer la direction horizontale de la balle en fonction de la position sur la raquette
        if (x < game->racket_x) {
            game->ball_dx = -1;  // Si la balle touche le côté gauche de la raquette
        } else if (x > game->racket_x) {
            game->ball_dx = 1;   // Si la balle touche le côté droit de la raquette
        } else {
            game->ball_dx = 0;   // Si la balle touche le centre de la raquette
        }
    }

    // Vérification des collisions avec les autres bords
    if (haut) {
        game->ball_dy = 1;
        if (read(world, x, y - 1) == Type1 || read(world, x, y - 1) == Type2) {
            if (read(world, x, y - 1) == Type2) {
                // Incrémenter le compteur de frappes pour Type2
                int index = (y - 1) * world->width + x;
                world->hit_count[index]++;

                // Si le compteur atteint 2, détruire le bloc
                if (world->hit_count[index] >= 2) {
                    write(world, x, y - 1, Empty);
                    casse = true;
                    game->score++;
                }
            } else {
                // Si c'est un Type1, juste le détruire
                write(world, x, y - 1, Empty);
                casse = true;
                game->score++;
            }
        }
    }

    if (bas) {
        game->ball_dy = -1;
        if (read(world, x, y + 1) == Type1 || read(world, x, y + 1) == Type2) {
            if (read(world, x, y + 1) == Type2) {
                // Incrémenter le compteur de frappes pour Type2
                int index = (y + 1) * world->width + x;
                world->hit_count[index]++;

                // Si le compteur atteint 2, détruire le bloc
                if (world->hit_count[index] >= 2) {
                    write(world, x, y + 1, Empty);
                    casse = true;
                    game->score++;
                }
            } else {
                // Si c'est un Type1, juste le détruire
                write(world, x, y + 1, Empty);
                casse = true;
                game->score++;
            }
        }
    }

    if (gauche) {
        game->ball_dx *= -1;
        if (read(world, x - 1, y) == Type1 || read(world, x - 1, y) == Type2) {
            if (read(world, x - 1, y) == Type2) {
                // Incrémenter le compteur de frappes pour Type2
                int index = y * world->width + (x - 1);
                world->hit_count[index]++;

                // Si le compteur atteint 2, détruire le bloc
                if (world->hit_count[index] >= 2) {
                    write(world, x - 1, y, Empty);
                    casse = true;
                    game->score++;
                }
            } else {
                // Si c'est un Type1, juste le détruire
                write(world, x - 1, y, Empty);
                casse = true;
                game->score++;
            }
        }
    }

    if (droite) {
        game->ball_dx *= -1;
        if (read(world, x + 1, y) == Type1 || read(world, x + 1, y) == Type2) {
            if (read(world, x + 1, y) == Type2) {
                // Incrémenter le compteur de frappes pour Type2
                int index = y * world->width + (x + 1);
                world->hit_count[index]++;

                // Si le compteur atteint 2, détruire le bloc
                if (world->hit_count[index] >= 2) {
                    write(world, x + 1, y, Empty);
                    casse = true;
                    game->score++;
                }
            } else {
                // Si c'est un Type1, juste le détruire
                write(world, x + 1, y, Empty);
                casse = true;
                game->score++;
            }
        }
    }

    // Gestion des collisions diagonales
    if (game->ball_dx != 0 && game->ball_dy != 0) {
        int next_x = x + game->ball_dx;
        int next_y = y + game->ball_dy;
        if ((read(world, next_x, next_y) == Type1 || read(world, next_x, next_y) == Type2) && !haut && !bas && !gauche && !droite) {
            if (read(world, next_x, next_y) == Type2) {
                // Incrémenter le compteur de frappes pour Type2
                int index = next_y * world->width + next_x;
                world->hit_count[index]++;

                // Si le compteur atteint 2, détruire le bloc
                if (world->hit_count[index] >= 2) {
                    write(world, next_x, next_y, Empty);
                    casse = true;
                    game->score++;
                    game->ball_dx *= -1;
                    game->ball_dy *= -1;
                }
            } else {
                // Si c'est un Type1, juste le détruire
                write(world, next_x, next_y, Empty);
                casse = true;
                game->score++;
                game->ball_dx *= -1;
                game->ball_dy *= -1;
            }
        }
    }

    // Vérifier si le jeu est terminé (tous les blocs détruits)
    if (casse && !casse_dublock(world)) {
        game->statut = Win;
    }
}

void  change_statut(Game *game){
      switch (game->statut)
      {
      case Begin:
            game->statut=Play;
            game->ball_dx = 0;
            game->ball_dy = -1;
            break;
      case Play:
      game->statut=Pause; 
      break;
      case Pause:
      game->statut=Play;
      break;
      case GameOver:
      game->statut=Begin;
      cout<<"game over"<<endl;
      break;
      case Win:
      game->statut=Begin;
      break;
      default:
        break;
      }
}

bool  casse_dublock(World* world) { 
    for (int i = 0; i < world->width * world->height; i++) {
        if (world->grid[i] == Type1 || world->grid[i] == Type2) {
            return true;
        }
    }
    return false;
}

