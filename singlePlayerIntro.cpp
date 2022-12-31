#include "headers/singlePlayerIntro.h"
#include "headers/gFunctions.h"
#include "headers/libHeaders.h"
#include "headers/music.h"
#include <bits/stdc++.h>

using namespace std;

extern int SCREEN_WIDTH;

extern SDL_Renderer *gRenderer;
extern LTexture singlePlayerName;

extern LTexture baseBackground;
extern LTexture inputTexture;
extern LTexture popUp;

extern LTexture loginMenu;

extern state currState ;

extern string player1;

extern SDL_Color textColor;
extern TTF_Font *gFont;

extern fstream playerData;

int singlePlayerFlag = 0;

void handleEvent_singlePlayerIntro(SDL_Event e)
{
    bool renderText = false;

    if (singlePlayerFlag == 1)
    {
        singlePlayerFlag = 0;
        player1 = "";
    }
    if (e.type == SDL_KEYDOWN)
    {
        // Handle backspace
        if (e.key.keysym.sym == SDLK_BACKSPACE && ((singlePlayerFlag == 0 && player1.length() > 0)))
        {
            // lop off character
            player1.pop_back();
            renderText = true;
        }
    }
    // Special text input event
    else if (e.type == SDL_TEXTINPUT && ((singlePlayerFlag == 0 && player1.length() < 20)))
    {

        // Append character

        player1 += e.text.text;

        renderText = true;
    }
    // cout<<inputText<<endl ;
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN && ((singlePlayerFlag == 0 && player1 != "")))
    {

        string currentUser;

        playerData.open("playerData.txt", ios::app);
        playerData << player1;
        playerData << endl;
        playerData.close();
        playSelectSound();

        singlePlayerFlag++;

        inputTexture.free();
        if (singlePlayerFlag == 1)
        {
            currState = singlePlayerMode;
            playSelectSound();
        }
    }
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
    {
        currState = allModes;
    }

    if ((player1 != ""))
    {
        // Render new text

        inputTexture.loadFromRenderedText(player1.c_str(), textColor, gFont);
    }
    else
    {
        inputTexture.loadFromRenderedText("", textColor, gFont);
    }
}

void handle_singlePlayerIntro()
{
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

    SDL_RenderCopy(gRenderer, baseBackground.mTexture, NULL, NULL);

    loginMenu.render(400,100);

    singlePlayerName.render(435, 400);

    inputTexture.render(480, 470);

    SDL_RenderPresent(gRenderer);
}