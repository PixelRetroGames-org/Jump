#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"
#include<cstdio>
#include<string>
#include<cstring>
#include<algorithm>
#define MAX_LIN 600
#define MAX_COL 800
#define LEVELL 10
#define LEVELC 20

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
 direction orizontal,vertical;
 int lives,points;
 coordinates level_position,screen_position;
 std::string image_name;
 SDL_Surface *image;
 bool load_image()
 {
  std::string filename=image_name;
  filename=(orizontal.val>=0?"R":"L")+filename;
  image=make_it_transparent(filename);
  if(image!=NULL)
     return true;
  return false;
 }
 void print_image()
 {
  apply_surface(screen_position.x,screen_position.y,image,screen);
 }
};

player timy;

struct texture
{
 int code;
 void read(FILE *in)
 {
  fscanf(in,"%d ",&code);
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
               MAP[i][j].print_image((j-1)*40,(i-1)*40);
           }
      }
 }
};

int main( int argc, char* args[] )
{
 SDL_Init(SDL_INIT_EVERYTHING);
 screen=SDL_SetVideoMode(MAX_COL,MAX_LIN,32,SDL_HWSURFACE);
 TTF_Init();
 timy.image_name="warrior.bmp";
 timy.screen_position.x=0;
 timy.screen_position.y=120;
 timy.level_position.x=4;
 timy.level_position.y=1;
 timy.load_image();
 timy.print_image();
 level TEST;
 TEST.filename="Test_Level.lvl";
 TEST.position.x=1;
 TEST.position.y=1;
 TEST.load();
 TEST.print();
 SDL_Flip(screen);
 SDL_Delay(5000);
return 0;
}
