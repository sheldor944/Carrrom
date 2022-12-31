#include "headers/music.h"
#include "headers/gFunctions.h"
#include "headers/libHeaders.h"

extern state currState ;

extern Mix_Music *gMusic ; 
extern Mix_Chunk *hardCollisionSound ;
extern Mix_Chunk *selectionSound ;
extern Mix_Music *collisionSound ; 

void playBackgroundMusic()
{
    
    if(Mix_PlayingMusic() == 0 )
    {
        Mix_HaltMusic();
        Mix_PlayMusic(gMusic, -1);
    }
}

// if in Game we have to use mix_haltMusic() to stop all the background musics 

void playHardCollision()
{
    Mix_HaltMusic();
    Mix_PlayChannel( -1, hardCollisionSound , 0 );
}

void playSelectSound()
{
    if(currState == twoPlayerMode ||  currState == fourPlayerMode || currState == singlePlayerMode )
    {
        Mix_HaltMusic();
    }
    Mix_PlayChannel (-1 , selectionSound , 0);
}

void playCollisionSound()
{
    Mix_PlayMusic(collisionSound , 0 );
}




