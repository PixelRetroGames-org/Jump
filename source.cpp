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

BigScreen SCREEN;

player timy;
player monster;

void move_monster_greedy(player &M,level A)
{
 M.horizontal.val=(timy.level_position.y<M.level_position.y?-1:1);
 M.horizontal.val=timy.level_position.y==M.level_position.y?0:M.horizontal.val;
 M.vertical.val=(timy.level_position.x<M.level_position.x && A.MAP[M.level_position.x][M.level_position.y+M.horizontal.val].solid)?-1:0;
}

void move_monster_OY(player &M,level A,int y1,int y2)
{
 if(y1>y2)
    std::swap(y1,y2);
 if(M.last_horizontal.val==0)
    M.last_horizontal.val=-1;
 if((M.last_horizontal.val==1 && M.level_position.y==y2) || (M.last_horizontal.val==-1 && M.level_position.y==y1))
    M.last_horizontal.val*=(-1);
 M.horizontal.val=M.last_horizontal.val;
}

//Returns if the player is/isn't on solid texture
bool update_position(level A,player &P)
{
 bool pp=false;
 for(int i=P.screen_position.x+40+1;i<=std::min(P.screen_position.x+40*2,MAX_LIN) && !pp;i++)
     for(int j=P.screen_position.y;j<=std::min(P.screen_position.y+40,MAX_COL) && !pp;j++)
         if(A.obstacles_on_screen[i][j].is_solid())
            pp=true;
 if(!pp)
    {
     P.vertical.val=1;
    }
 P.screen_position.x+=P.vertical.val;
 P.screen_position.y+=P.horizontal.val*P.step_lenght;
 P.vertical.val+=P.vertical.val!=0?(P.vertical.val<0?1:-1):0;
 P.horizontal.val+=P.horizontal.val!=0?(P.horizontal.val<0?1:-1):0;
 //P.vertical.val=P.horizontal.val=0;
 P.in_air=false;
 return false;
}

void print_lives()
{
 TTF_Font *font=TTF_OpenFont("font.ttf",30);
 SDL_Color color={255,46,28};
 SDL_Surface *message=TTF_RenderText_Solid(font,"LIVES:",color);
 apply_surface(MAX_COL-300,4,message,SCREEN.screen);
 int POZ=MAX_COL-200;
 SDL_Surface *heart=make_it_transparent("heart.bmp");
 for(int i=1;i<=timy.lives;i++)
     {
      apply_surface(POZ+(i-1)*40,0,heart,SCREEN.screen);
     }
}

void print_points()
{
 int POZ=120;
 TTF_Font *font=TTF_OpenFont("font.ttf",30);
 char nr[90];
 int_to_string(timy.points,nr);
 SDL_Color color={255,216,0};
 SDL_Surface *message=TTF_RenderText_Solid(font,nr,color);
 apply_surface(POZ,4,message,SCREEN.screen);
 color={255,216,0};
 message=TTF_RenderText_Solid(font,"POINTS:",color);
 apply_surface(POZ-120,4,message,SCREEN.screen);
 message=make_it_transparent("coin.bmp");
 apply_surface(POZ+(strlen(nr)*20)-20,0,message,SCREEN.screen);
}

void print_grid()
{
 print_lives();
 print_points();
 SDL_Flip(SCREEN.screen);
}

int main( int argc, char* args[] )
{
 SCREEN.initialize();
 timy.image_name="warrior.bmp";
 timy.level_position.x=1;
 timy.level_position.y=4;
 timy.generate_screen_position();
 timy.horizontal.val=1;
 timy.load_image();
 timy.print_image();
 monster.image_name="zombie.bmp";
 monster.level_position.x=4;
 monster.level_position.y=5;
 monster.generate_screen_position();
 monster.horizontal.val=1;
 monster.load_image();
 monster.horizontal.val=0;
 monster.print_image();
 level TEST;
 TEST.filename="Test_Level.lvl";
 TEST.position.x=1;
 TEST.position.y=1;
 TEST.load();
 TEST.print();
 //print_mini_texture(TEST);
 SDL_Flip(SCREEN.screen);
 timy.points=123456789;
 timy.lives=5;
 print_grid();

 const int FRAMES_PER_SECOND=10;
 Timer fps;
 fps.start();
 int frame=0;
 SDL_Event event;
 Uint8 *keystates=SDL_GetKeyState(NULL);
 bool quit=false,blocked=false;
 while(!quit)
       {
        fps.start();
        SDL_PollEvent( &event );
        if(!blocked)
           {
            if(keystates[SDLK_RIGHT])
               {
                timy.horizontal.val=1;
               }
            if(keystates[SDLK_LEFT])
               {
                timy.horizontal.val=-1;
               }
            if(keystates[SDLK_SPACE] || keystates[SDLK_UP])
               {
                if(TEST.MAP[timy.level_position.x+1][timy.level_position.y].solid)
                   timy.jump();
               }
            if(keystates[SDLK_ESCAPE])
               {
                quit=true;
               }
           }
        blocked=false;
        if(event.type==SDL_QUIT)
           quit=true;
        timy.load_image();
        timy.erase_image();
        update_position(TEST,timy);
        //timy.generate_screen_position();
        timy.print_image();
        //move_monster_greedy(monster,TEST);
        move_monster_OY(monster,TEST,1,10);
        monster.erase_image();
        update_position(TEST,monster);
        monster.load_image();
        //monster.generate_screen_position();
        monster.print_image();
        SDL_Flip(SCREEN.screen);
        //SDL_Delay(500);
        frame++;
        if(fps.get_ticks()<1000/FRAMES_PER_SECOND)
           {
            if(timy.in_air || monster.in_air || true)
               {
                int X=((1000/FRAMES_PER_SECOND)-fps.get_ticks())/2;
                SDL_Delay(X);
                if(timy.in_air)
                   {
                    timy.load_image();
                    timy.erase_image();
                    update_position(TEST,timy);
                    //timy.generate_screen_position();
                    timy.print_image();
                    SDL_Flip(SCREEN.screen);
                   }
                if(monster.in_air)
                   {
                    monster.load_image();
                    monster.erase_image();
                    update_position(TEST,monster);
                    //monster.generate_screen_position();
                    monster.print_image();
                    SDL_Flip(SCREEN.screen);
                   }
                SDL_Delay(X);
               }
           }
       }
 timy.erase_image();
 timy.generate_screen_position();
 timy.print_image();
 SDL_Flip(SCREEN.screen);
return 0;
}
