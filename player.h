#ifndef player_H
#define player_H
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"
#include<cstdio>
#include<string>
#include<cstring>
#include<stdlib.h>
#include<algorithm>
#include "level.h"

struct player
{
 bool in_air;
 direction horizontal,vertical,last_horizontal;
 int lives,points;
 int step_lenght;
 coordinates level_position,screen_position;
 std::string image_name;
 SDL_Surface *image;
 player()
 {
  in_air=false;
  horizontal.val=vertical.val=last_horizontal.val=0;
  lives=points=0;
  step_lenght=10;
  level_position.x=level_position.y=screen_position.x=screen_position.y=0;
  image_name="";
  image=NULL;
 }
 void generate_screen_position();
 void load_initial_image();
 bool load_image();
 void print_image();
 void erase_image();
 void jump();
};
#endif // player_CPP
