#include "headers/rules.h"
#include "headers/texture.h"
#include "headers/gFunctions.h"
#include "headers/music.h"

extern SDL_Renderer* gRenderer  ; 
extern LTexture allRules;
extern LTexture background ;
extern state currState ;
extern LTexture rulesBackground; 


void handleEvent_rules(SDL_Event e)
{
    if(e.type== SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
    {
        
        currState = mainMenu ;
        playSelectSound();
    }
}

void handle_rules()
{
    SDL_RenderClear( gRenderer );
    SDL_SetRenderDrawColor ( gRenderer , 255 , 255 , 255, 255) ;
    SDL_RenderCopy ( gRenderer , rulesBackground.mTexture, NULL , NULL );
    SDL_RenderPresent( gRenderer );
}