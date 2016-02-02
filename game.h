#ifndef game_H
#define game_H
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"
#define MAX_LIN 720
#define MAX_COL 1280
#define LEVELL 100
#define LEVELC 100
#define GRIDL 40
#define GRIDC 1280

struct BigScreen
{
 SDL_Surface *screen;
 BigScreen()
 {
  screen=NULL;
 }
 void initialize();
};

extern BigScreen SCREEN;

#endif
