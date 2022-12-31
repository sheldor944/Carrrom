#include "headers/fourPlayerIntro.h"
#include "headers/libHeaders.h"
#include "headers/gFunctions.h"
#include "headers/music.h"
#include <bits/stdc++.h>

using namespace std;

extern int SCREEN_WIDTH;

extern SDL_Renderer *gRenderer;
extern state currState;
extern LTexture playerOneName;
extern LTexture PlayerTwoName;
extern LTexture PlayerThreeName ;
extern LTexture PlayerFourName ; 

extern LTexture baseBackground;
extern LTexture inputTexture;
extern LTexture popUp;

extern LTexture loginMenu ;

// string player1, player2, player3, player4;
extern string player1, player2, player3, player4;

extern SDL_Color textColor;
extern TTF_Font *gFont;

extern fstream playerData;

int fourPlayerFlag = 0;

void handleEvent_fourPlayerIntro(SDL_Event e)
{
    bool renderText = false;

    if (fourPlayerFlag == 4)
    {
        fourPlayerFlag = 0;
        player1="";
        player2="";
        player3="";
        player4="";
    }
    if (e.type == SDL_KEYDOWN)
    {
        // Handle backspace
        if (e.key.keysym.sym == SDLK_BACKSPACE && ((fourPlayerFlag == 0 && player1.length() > 0) || (fourPlayerFlag == 1 && player2.length() > 0) || (fourPlayerFlag == 2 && player3.length() > 0) || (fourPlayerFlag == 3 && player4.length() > 0)))
        {
            // lop off character
            if (fourPlayerFlag == 0)
            {
                player1.pop_back();
            }
            else if (fourPlayerFlag == 1)
            {
                player2.pop_back();
            }
            else if (fourPlayerFlag == 2)
            {
                player3.pop_back();
            }
            else if (fourPlayerFlag == 3)
            {
                player4.pop_back();
            }
            renderText = true;
        }
    }
    // Special text input event
    else if (e.type == SDL_TEXTINPUT && ((fourPlayerFlag == 0 && player1.length() < 20) || (fourPlayerFlag == 1 && player2.length() < 20) || (fourPlayerFlag == 2 && player3.length() < 20) || (fourPlayerFlag == 3 && player4.length() < 20)))
    {

        // Append character
        if (fourPlayerFlag == 0)
        {
            player1 += e.text.text;
        }
        else if (fourPlayerFlag == 1)
        {
            player2 += e.text.text;
        }
        else if (fourPlayerFlag == 2)
        {
            player3 += e.text.text;
        }
        else if (fourPlayerFlag == 3)
        {
            player4 += e.text.text;
        }
        renderText = true;
    }
    // cout<<inputText<<endl ;
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN && ((fourPlayerFlag == 0 && player1 != "") || (fourPlayerFlag == 1 && player2 != "") || (fourPlayerFlag == 2 && player3 != "") || (fourPlayerFlag == 3 && player4 != "")))
    {

        string currentUser;
        if (fourPlayerFlag == 0)
        {

            playerData.open("playerData.txt", ios::app);
            playerData << player1;
            playerData << endl;
            playerData.close();
            playSelectSound();
        }
        else if (fourPlayerFlag == 1)
        {

            playerData.open("playerData.txt", ios::app);
            playerData << player2;
            playerData << endl;
            playerData.close();
            playSelectSound();
        }
        else if (fourPlayerFlag == 2)
        {
            playerData.open("playerData.txt", ios::app);
            playerData << player3;
            playerData << endl;
            playerData.close();
            playSelectSound();
        }
        else if (fourPlayerFlag == 3)
        {
            playerData.open("playerData.txt", ios::app);
            playerData << player4;
            playerData << endl;
            playerData.close();
            playSelectSound();
        }

        fourPlayerFlag++;

        inputTexture.free();
        if (fourPlayerFlag == 4)
        {
            currState = fourPlayerMode;
            playSelectSound();
        }
    }
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
    {
        currState = allModes;
    }

    if ((player1 != "" && fourPlayerFlag == 0) || (player2 != "" && fourPlayerFlag == 1) || (player3 !="" && fourPlayerFlag == 2) || (player4!="" && fourPlayerFlag == 3))
    {
        // Render new text
        if (fourPlayerFlag == 0)
        {
            inputTexture.loadFromRenderedText(player1.c_str(), textColor, gFont);
        }
        else if (fourPlayerFlag == 1)
        {
            inputTexture.loadFromRenderedText(player2.c_str(), textColor, gFont);
        }
        else if (fourPlayerFlag == 2)
        {
            inputTexture.loadFromRenderedText(player3.c_str(), textColor, gFont);

        }
        else if(fourPlayerFlag ==3)
        {
            inputTexture.loadFromRenderedText(player4.c_str(), textColor, gFont);

        }
    }
    else
    {
        inputTexture.loadFromRenderedText("", textColor, gFont);
    }
}

void handle_fourPlayerIntro()
{
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

    SDL_RenderCopy(gRenderer, baseBackground.mTexture, NULL, NULL);

    loginMenu.render(400,100);

    if (fourPlayerFlag == 0)
    {
        playerOneName.render(485, 400);
    }
    else if (fourPlayerFlag == 1)
    {
        PlayerTwoName.render(485, 400);
    }
    else if(fourPlayerFlag  == 2)
    {
        PlayerThreeName.render(485,400);
    }
    else if( fourPlayerFlag == 3)
    {
        PlayerFourName.render(485 , 400 );
    }
    
    inputTexture.render(520, 470);

    SDL_RenderPresent(gRenderer);
}