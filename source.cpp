#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"
#include<cstdio>
#include<string>
#include<cstring>
#include<stdlib.h>
#include<algorithm>
#define MAX_LIN 600
#define MAX_COL 800
#define LEVELL 100
#define LEVELC 100
#define GRIDL 40
#define GRIDC 800

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
 SDL_Rect offset;
 offset.x=x;
 offset.y=y;
 SDL_BlitSurface(source,NULL,destination,&offset);
}

SDL_Surface *make_it_transparent( std::string filename )
{
 //The image that's loaded
 SDL_Surface *loadedImage=NULL;
 //The optimized image that will be used
 SDL_Surface *optimizedImage=NULL;
 //Load the image
 loadedImage=IMG_Load(filename.c_str());
 //If the image loaded
 if(loadedImage!=NULL)
    {
	//Create an optimized image
	optimizedImage=SDL_DisplayFormat(loadedImage);
	//Free the old image
	SDL_FreeSurface(loadedImage);
	//If the image was optimized just fine
	if(optimizedImage!=NULL)
        {
	    //Map the color key
	    Uint32 colorkey=SDL_MapRGB(optimizedImage->format,0xFF,0x0,0xE1);
	    //Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
	    SDL_SetColorKey(optimizedImage,SDL_SRCCOLORKEY,colorkey);
        }
    }
 //Return the optimized image
 return optimizedImage;
}

void int_to_string(int nr,char *v)
{
 int cnr=nr,nr_digits=0;
 while(cnr!=0)
       {
        cnr/=10;
        nr_digits++;
       }
 v[nr_digits]=0;
 while(nr!=0)
       {
        *(v+(--nr_digits))=nr%10+'0';
        nr/=10;
       }
}

SDL_Surface *screen;

struct coordinates
{
 int x,y;
};

struct direction
{
 int val;
 //pixel per time unit
};

struct player
{
 bool in_air;
 direction horizontal,vertical;
 int lives,points;
 coordinates level_position,screen_position;
 std::string image_name;
 SDL_Surface *image;
 player()
 {
  in_air=false;
  horizontal.val=vertical.val=0;
  lives=points=0;
  level_position.x=level_position.y=screen_position.x=screen_position.y=0;
  image_name="";
  image=NULL;
 }
 void generate_screen_position()
 {
  screen_position.x=(level_position.x-1)*40;
  screen_position.y=(level_position.y-1)*40;
 }
 void load_initial_image()
 {
  image=make_it_transparent(image_name);
 }
 bool load_image()
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
 void print_image()
 {
  apply_surface(screen_position.y,screen_position.x+GRIDL,image,screen);
 }
 void erase_image()
 {
  SDL_Surface *clear=SDL_LoadBMP("clear.bmp");
  apply_surface(screen_position.y,screen_position.x+GRIDL,clear,screen);
 }
 void jump()
 {
  vertical.val=-2;
 }
};

player timy;
player monster;

struct texture
{
 int code;
 bool solid;
 void read(FILE *in)
 {
  fscanf(in,"%d ",&code);
  if(code==1)
     solid=true;
  else
     solid=false;
 }
 void print_image(int x,int y)
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
  apply_surface(x,y,image,screen);
 }
};

struct level
{
 std::string filename;
 texture MAP[LEVELL][LEVELC];
 int L,C;
 coordinates position;
 void load()
 {
  FILE *in=fopen(filename.c_str(),"r");
  fscanf(in,"%d %d ",&L,&C);
  for(int i=1;i<=L;i++)
      {
       for(int j=1;j<=C;j++)
           MAP[i][j].read(in);
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
 void print()
 {
  for(int i=position.x;i<=L;i++)
      {
       for(int j=position.y;j<=C;j++)
           {
            if(i!=timy.level_position.x || j!=timy.level_position.y)
               MAP[i][j].print_image((j-1)*40,(i-1)*40+GRIDL);
           }
      }
 }
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
 Timer()
 {
  //Initialize the variables
  startTicks = 0;
  pausedTicks = 0;
  paused = false;
  started = false;
 }
 void start()
 {
  //Start the timer
  started = true;
  //Unpause the timer
  paused = false;
  //Get the current clock time
  startTicks = SDL_GetTicks();
 }
 void stop()
 {
  //Stop the timer
  started = false;
  //Unpause the timer
  paused = false;
 }
 void pause()
 {
  //If the timer is running and isn't already paused
  if((started==true) && (paused==false))
     {
      //Pause the timer
      paused = true;
      //Calculate the paused ticks
      pausedTicks = SDL_GetTicks() - startTicks;
     }
 }
 void unpause()
 {
  //If the timer is paused
  if(paused==true)
     {
      //Unpause the timer
      paused=false;
      //Reset the starting ticks
      startTicks = SDL_GetTicks() - pausedTicks;
      //Reset the paused ticks
      pausedTicks = 0;
     }
 }
 int get_ticks()
 {
  //If the timer is running
  if(started==true)
     {
      //If the timer is paused
      if(paused==true)
         {
          //Return the number of ticks when the timer was paused
          return pausedTicks;
         }
      else
         {
          //Return the current time minus the start time
          return SDL_GetTicks() - startTicks;
         }
     }
  //If the timer isn't running
  return 0;
 }
 bool is_started()
 {
  return started;
 }
 bool is_paused()
 {
  return paused;
 }
};

void move_monster(player &M,level A)
{
 M.horizontal.val=(timy.level_position.y<M.level_position.y?-1:1);
 M.horizontal.val=timy.level_position.y==M.level_position.y?0:M.horizontal.val;
 M.vertical.val=(timy.level_position.x<M.level_position.x && A.MAP[M.level_position.x][M.level_position.y+M.horizontal.val].solid)?-1:0;
}

//Returns if the player is/isn't on solid texture
bool update_position(level A,player &P)
{
 bool pp=false;
 for(int i=P.level_position.x-1;i>=P.level_position.x+P.vertical.val && !pp;i--)
     if(A.MAP[i][P.level_position.y].solid)
        {
         P.level_position.x=i+1;
         pp=true;
        }
 if(!pp)
    P.level_position.x+=P.vertical.val;
 P.level_position.y+=P.horizontal.val;
 P.level_position.x=std::min(A.L,P.level_position.x);
 P.level_position.y=std::min(A.C,P.level_position.y);
 P.level_position.x=std::max(1,P.level_position.x);
 P.level_position.y=std::max(1,P.level_position.y);
 if(A.MAP[P.level_position.x][P.level_position.y].solid)
    {
     P.level_position.x-=P.vertical.val;
     P.level_position.y-=P.horizontal.val;
    }
 if(P.vertical.val==0)
    {
     if(!A.MAP[P.level_position.x+1][P.level_position.y].solid)
        {
         P.level_position.x++;
         P.vertical.val=P.horizontal.val=0;
         P.in_air=true;
         return true;
        }
    }
 P.vertical.val=P.horizontal.val=0;
 P.in_air=false;
 return false;
}

void print_lives()
{
 TTF_Font *font=TTF_OpenFont("font.ttf",30);
 SDL_Color color={255,46,28};
 SDL_Surface *message=TTF_RenderText_Solid(font,"LIVES:",color);
 apply_surface(400,4,message,screen);
 int POZ=500;
 SDL_Surface *heart=make_it_transparent("heart.bmp");
 for(int i=1;i<=timy.lives;i++)
     {
      apply_surface(POZ+(i-1)*40,0,heart,screen);
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
 apply_surface(POZ,4,message,screen);
 color={255,216,0};
 message=TTF_RenderText_Solid(font,"POINTS:",color);
 apply_surface(POZ-120,4,message,screen);
 message=make_it_transparent("coin.bmp");
 apply_surface(POZ+(strlen(nr)*20)-20,0,message,screen);
}

void print_grid()
{
 print_lives();
 print_points();
 SDL_Flip(screen);
}

int main( int argc, char* args[] )
{
 SDL_Init(SDL_INIT_EVERYTHING);
 screen=SDL_SetVideoMode(MAX_COL,MAX_LIN,32,SDL_HWSURFACE);
 SDL_WM_SetCaption("Jump!","Pixel RetroGames");
 TTF_Init();
 timy.image_name="warrior.bmp";
 timy.level_position.x=1;
 timy.level_position.y=1;
 timy.generate_screen_position();
 timy.horizontal.val=1;
 timy.load_image();
 timy.print_image();
 monster.image_name="zombie.bmp";
 monster.level_position.x=1;
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
 SDL_Flip(screen);
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
        update_position(TEST,timy);
        timy.erase_image();
        timy.generate_screen_position();
        timy.print_image();
        move_monster(monster,TEST);
        update_position(TEST,monster);
        monster.load_image();
        monster.erase_image();
        monster.generate_screen_position();
        monster.print_image();
        SDL_Flip(screen);
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
                    update_position(TEST,timy);
                    timy.erase_image();
                    timy.generate_screen_position();
                    timy.print_image();
                    SDL_Flip(screen);
                   }
                if(monster.in_air)
                   {
                    monster.load_image();
                    update_position(TEST,monster);
                    monster.erase_image();
                    monster.generate_screen_position();
                    monster.print_image();
                    SDL_Flip(screen);
                   }
                SDL_Delay(X);
               }
           }
       }
 timy.erase_image();
 timy.generate_screen_position();
 timy.print_image();
 SDL_Flip(screen);
return 0;
}
