#include "headers/allModes.h"
#include "headers/texture.h"
#include "headers/gFunctions.h"
#include "headers/music.h"

extern SDL_Renderer *gRenderer;
extern LTexture baseBackground;
extern state currState;
extern LTexture allmodesScreen;
extern LTexture glowingTwoPlayer;
extern LTexture glowingFourPlayer;
extern LTexture glowingSinglePlayer;

extern string player1,player2,player3,player4 ;

void handleEvent_allModes(SDL_Event e)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
    {
        currState = mainMenu;
        playSelectSound();
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN && x > 435 && x < 836 && y > 303 && y < 437)
    {
        currState = singleModeScreen;
        player1="";
        player2="";
        player3="";
        player4="";
        playSelectSound();
        // playCollisionSound();
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN && x > 435 && x < 836 && y > 510 && y < 644)
    {
        currState = twoPlayerIntro;
        player1="";
        player2="";
        player3="";
        player4="";
        playSelectSound();
    }

    else if (e.type == SDL_MOUSEBUTTONDOWN && x > 435 && x < 836 && y > 709 && y < 845)
    {
        currState = fourPlayerIntro;
        player1="";
        player2="";
        player3="";
        player4="";
        playSelectSound();
    }
}

void handle_allModes()
{
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

    SDL_RenderCopy(gRenderer, allmodesScreen.mTexture, NULL, NULL);

    int x, y;
    SDL_GetMouseState(&x, &y);

    if (x > 435 && x < 836 && y > 303 && y < 437)
    {
        SDL_RenderCopy(gRenderer, glowingSinglePlayer.mTexture, NULL, NULL);
    }
    if (x > 435 && x < 836 && y > 510 && y < 644)
    {
        SDL_RenderCopy(gRenderer, glowingTwoPlayer.mTexture, NULL, NULL);
    }
    if (x > 435 && x < 836 && y > 709 && y < 845)
    {
        SDL_RenderCopy(gRenderer, glowingFourPlayer.mTexture, NULL, NULL);
    }

    SDL_RenderPresent(gRenderer);
}