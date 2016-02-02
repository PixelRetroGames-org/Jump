#ifndef player_CPP
#define player_CPP
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"
#include<cstdio>
#include<string>
#include<cstring>
#include<stdlib.h>
#include<algorithm>
#include "player.h"

void player::generate_screen_position()
{
 screen_position.x=(level_position.x-1)*40;
 screen_position.y=(level_position.y-1)*40;
}
void player::load_initial_image()
{
 image=make_it_transparent(image_name);
}
bool player::load_image()
{
 std::string filename=image_name;
 if(horizontal.val==0)
    return false;
 filename=(horizontal.val>0?"R":"L")+filename;
 image=make_it_transparent(filename);
 if(image!=NULL)
    return true;
 return false;
}
void player::print_image()
{
 apply_surface(screen_position.y,screen_position.x+GRIDL,image,SCREEN.screen);
}
void player::erase_image()
{
 SDL_Surface *clear=SDL_LoadBMP("clear.bmp");
 apply_surface(screen_position.y,screen_position.x+GRIDL,clear,SCREEN.screen);
}
void player::jump()
{
 vertical.val=-2;
}

#endif
