#ifndef level_CPP
#define level_CPP
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

void level::load()
{
 FILE *in=fopen(filename.c_str(),"r");
 fscanf(in,"%d %d ",&L,&C);
 for(int i=1;i<=L;i++)
     {
      for(int j=1;j<=C;j++)
          {
           MAP[i][j].read(in);
           update_mini_texture((i-1)*40,(j-1)*40,MAP[i][j]);
          }
     }
 fclose(in);
 FILE *out=fopen("err.txt","w");
 for(int i=1;i<=L;i++)
     {
      for(int j=1;j<=C;j++)
          fprintf(out,"%d ",MAP[i][j].code);
      fprintf(out,"\n");
     }
}
void level::print()
{
 for(int i=position.x;i<=L;i++)
     {
      for(int j=position.y;j<=C;j++)
          {
           //if(i!=timy.level_position.x || j!=timy.level_position.y)
           MAP[i][j].print_image((j-1)*40,(i-1)*40+GRIDL);
          }
     }
}
void level::update_mini_texture(int l,int c,texture A)
{
 for(int i=l;i<=l+40;i++)
     for(int j=c;j<=c+40;j++)
         obstacles_on_screen[i][j].code=A.code;
 //print_mini_texture();
}

#endif
