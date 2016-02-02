#ifndef library_CPP
#define library_CPP
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

Timer::Timer()
{
 //Initialize the variables
 startTicks = 0;
 pausedTicks = 0;
 paused = false;
 started = false;
}
void Timer::start()
{
 //Start the timer
 started = true;
 //Unpause the timer
 paused = false;
 //Get the current clock time
 startTicks = SDL_GetTicks();
}
void Timer::stop()
{
 //Stop the timer
 started = false;
 //Unpause the timer
 paused = false;
}
void Timer::pause()
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
void Timer::unpause()
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
int Timer::get_ticks()
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
bool Timer::is_started()
{
 return started;
}
bool Timer::is_paused()
{
 return paused;
}

#endif
