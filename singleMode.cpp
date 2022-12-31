#include "headers/singleMode.h"
#include "headers/gFunctions.h"
#include "headers/libHeaders.h"
#include "headers/music.h"


extern SDL_Renderer *gRenderer;
extern state currState ;
extern LTexture singleMode ; 
extern LTexture glowingLeaderboardButton ; 
extern LTexture glowingRapid ;

void handleEvent_singleMode(SDL_Event e)
{

    int x, y;
    SDL_GetMouseState(&x, &y);

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
    {
        currState = allModes;
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN && x > 435 && x < 836 && y > 309 && y < 433)
    {

        currState = singlePlayerIntro;
        playSelectSound();
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN && x > 435 && x < 836 && y > 569 && y < 700)
    {
        currState = leaderboard;
        playSelectSound();
    }
}

void handle_singleMode()
{
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

    SDL_RenderCopy(gRenderer, singleMode.mTexture, NULL, NULL);
    int x, y;
    SDL_GetMouseState(&x, &y);

    if (x > 435 && x < 836 && y > 309 && y < 433)
    {
        SDL_RenderCopy(gRenderer, glowingRapid.mTexture, NULL, NULL);
    }
    if (x > 435 && x < 836 && y > 569 && y < 700)
    {
        SDL_RenderCopy(gRenderer, glowingLeaderboardButton.mTexture, NULL, NULL);
    }
    SDL_RenderPresent(gRenderer);

}