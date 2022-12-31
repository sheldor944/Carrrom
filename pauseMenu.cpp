#include "headers/pauseManu.h"
#include "headers/libHeaders.h"
#include "headers/gFunctions.h"
#include "headers/texture.h"
#include "headers/music.h"
#include "headers/2_player_mode.h"
#include "headers/timer.h"
#include "headers/music.h"


extern SDL_Renderer *gRenderer;
extern LTexture baseBackground;
extern state currState;
extern LTexture pauseMenu;
extern LTexture glowingResumeButton;
extern LTexture glowingExitButton;

extern LTimer CLK;
extern bool isGameRunning;

void handleEvent_pauseManu(SDL_Event e)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    if (x > 500 && x < 803 && y > 440 && y < 512 && e.type == SDL_MOUSEBUTTONDOWN)
    {
        // resume button so resume oibo 
        currState = prevState;
        playSelectSound();
        CLK.unpause();
    }
    if (x > 501 && x < 803 && y > 564 && y < 630 && e.type == SDL_MOUSEBUTTONDOWN)
    {
        // quit current mode of the game
        isGameRunning = false;
        currState = allModes;
        playSelectSound();
    }
    if (x > 848 && x < 897 && y > 249 && y < 294 && e.type == SDL_MOUSEBUTTONDOWN)
    {
        // resume oibo ikano 
        // cross button 
        currState = prevState;
        playSelectSound();
        CLK.unpause();
    }
}

void handle_pauseMenu()
{
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

    SDL_RenderCopy(gRenderer, baseBackground.mTexture, NULL, NULL);

    pauseMenu.render(400, 150);

    int x, y;
    SDL_GetMouseState(&x, &y);
    if (x > 500 && x < 803 && y > 440 && y < 512)
    {
        glowingResumeButton.render(400, 150);
    }
    if (x > 501 && x < 803 && y > 564 && y < 630)
    {

        glowingExitButton.render(400, 150);
    }

    SDL_RenderPresent(gRenderer);
}