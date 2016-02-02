#ifndef library_H
#define library_H
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"
#include<cstdio>
#include<string>
#include<cstring>
#include<stdlib.h>
#include<algorithm>
//#include "game.h"
#include "globals.h"

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );

SDL_Surface *make_it_transparent( std::string filename );

void int_to_string(int nr,char *v);

struct coordinates
{
 int x,y;
};

struct direction
{
 int val;
 //pixel per time unit
};

struct Timer
{
 //The clock time when the timer started
 int startTicks;
 //The ticks stored when the timer was paused
 int pausedTicks;
 //The timer status
 bool paused;
 bool started;
 Timer();
 void start();
 void stop();
 void pause();
 void unpause();
 int get_ticks();
 bool is_started();
 bool is_paused();
};
#endif
