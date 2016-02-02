#ifndef texture_H
#define texture_H
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"
#include<cstdio>
#include<string>
#include<cstring>
#include<stdlib.h>
#include<algorithm>
#include "library.h"

struct mini_texture
{
 int code;
 bool is_solid();
 void printf_err();
};

struct texture
{
 int code;
 bool solid;
 void read(FILE *in);
 void print_image(int x,int y);
};
#endif
