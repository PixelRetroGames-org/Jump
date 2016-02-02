#ifndef texture_CPP
#define texture_CPP
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"
#include<cstdio>
#include<string>
#include<cstring>
#include<stdlib.h>
#include<algorithm>
#include "texture.h"

bool mini_texture::is_solid()
{
 if(code==1)
    return true;
 return false;
}

void mini_texture::printf_err()
{
 FILE *f=fopen("error.txt","w");
 fprintf(f,"%d ",code);
}

void texture::read(FILE *in)
{
 fscanf(in,"%d ",&code);
 if(code==1)
    solid=true;
 else
    solid=false;
}
void texture::print_image(int x,int y)
{
 SDL_Surface *image;
 switch(code)
        {
         case 0:
                {
                 image=make_it_transparent("black.bmp");
                 break;
                }
         case 1:
                {
                 image=make_it_transparent("floor.bmp");
                 break;
                }
        }
 apply_surface(x,y,image,SCREEN.screen);
}
#endif
