#ifndef level_H
#define level_H
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

struct level
{
 std::string filename;
 texture MAP[LEVELL][LEVELC];
 mini_texture obstacles_on_screen[MAX_LIN][MAX_COL];
 int L,C;
 coordinates position;
 void load();
 void print();
 void update_mini_texture(int l,int c,texture A);
};

void print_mini_texture(level A);

#endif
