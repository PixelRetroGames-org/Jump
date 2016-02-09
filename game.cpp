#ifndef game_CPP
#define game_CPP
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
#include "game.h"

void BigScreen::initialize()
{
 SDL_Init(SDL_INIT_EVERYTHING);
 screen=SDL_SetVideoMode(MAX_COL,MAX_LIN,32,SDL_SWSURFACE);
 SDL_WM_SetCaption("Jump!","PixelRetroGames");
 TTF_Init();
}

#endif
