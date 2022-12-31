#include "headers/twoPlayerIntro.h"
#include "headers/libHeaders.h"
#include "headers/music.h"
#include "headers/gFunctions.h"
#include<bits/stdc++.h>

using namespace std; 



extern int SCREEN_WIDTH;

extern SDL_Renderer *gRenderer;
extern state currState;
extern LTexture playerOneName;
extern LTexture PlayerTwoName;
extern LTexture baseBackground;
extern LTexture inputTexture;
extern LTexture popUp;

extern LTexture loginMenu ;

extern string player1, player2, player3, player4; 
// string player1, inputText2;

extern SDL_Color textColor;
extern TTF_Font *gFont;

int flag = 0;
int beSure = 0;

int reset = 0;
int newP = 0;
int existing = 0;

fstream playerData;

// error dewar dorkar nai 
// just enter the name and play 


void handleEvent_twoPlayerIntro(SDL_Event e)
{
    // 2# take the name

    bool renderText = false;
    // int errorFlag = 0;
    if (flag == 2)
    {
        flag = 0;
        beSure = 0;
        reset = 0;
        newP = 0;
        existing = 0;
        player1="";
        player2 = "";
    }

    if (e.type == SDL_KEYDOWN)
    {
        // Handle backspace
        if (e.key.keysym.sym == SDLK_BACKSPACE && ((flag == 0 && player1.length() > 0) || (flag == 1 && player2.length() > 0)))
        {
            // lop off character
            if (flag == 0)
            {
                player1.pop_back();
            }
            else if (flag == 1)
            {
                player2.pop_back();
            }
            renderText = true;
        }
    }
    // Special text input event
    else if (e.type == SDL_TEXTINPUT && ((flag == 0 && player1.length() < 20) || (flag == 1 && player2.length() < 20)))
    {

        // Append character
        if (flag == 0)
        {
            player1 += e.text.text;
        }
        else if (flag == 1)
        {
            player2 += e.text.text;
        }
        renderText = true;
    }
    // cout<<inputText<<endl ;
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN && ((flag == 0 && player1 != "") || (flag == 1 && player2 != "")))
    {

        string currentUser;
        if (flag == 0)
        {
            // playerData.open("playerData.txt" );
            // while (getline(playerData , currentUser))
            // {
            //     if (currentUser == inputText)
            //     {
            //         cout << " hobe na re bhai \n";
            //         // ekhane same user ashle ki hobe entry korte hobe kintu
            //         beSure = 1;

            //         playErrorSound();
            //         errorFlag = 1;
            //     }
            // }
            // playerData.close();
            if (true)
            {
                playerData.open("playerData.txt", ios:: app);
                // cout << player1 << endl;
                playerData << player2;
                playerData << endl;
                playerData.close();
                playSelectSound();
            }
        }
        else if (flag == 1)
        {
            // playerData.open("playerData.txt");
            // while (getline(playerData, currentUser))
            // {
            //     if (currentUser == inputText2)
            //     {
            //         cout << " hobe na re bhai \n";
            //         playErrorSound();
            //         errorFlag = 1;
            //     }
            // }
            // playerData.close();
            if (true)
            {
                playerData.open("playerData.txt", ios:: app);
                // cout << player2 << endl;
                playerData << player2;
                playerData << endl;
                playerData.close();
            }
        }
        // if (!errorFlag)
            flag++;

        inputTexture.free();
        if (flag == 2)
        {
            currState = twoPlayerMode;
            playSelectSound();
        }
    }
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
    {
        currState = allModes;
    }

    // if (renderText)
    // {
    // Text is not empty
    if ((player1 != "" && flag == 0) || (player2 != "" && flag == 1))
    {
        // Render new text
        if (flag == 0)
        {
            inputTexture.loadFromRenderedText(player1.c_str(), textColor, gFont);
        }
        else if (flag == 1)
        {
            inputTexture.loadFromRenderedText(player2.c_str(), textColor, gFont);
        }
    }
    else
    {
        inputTexture.loadFromRenderedText("", textColor, gFont);
    }
    // }
}

void handle_twoPlayerIntro()
{
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

    SDL_RenderCopy(gRenderer, baseBackground.mTexture, NULL, NULL);

    loginMenu.render(400,100) ;
    if (flag == 0)
    {
        playerOneName.render(485, 400);
    }
    else if (flag == 1)
    {
        PlayerTwoName.render(485, 400);
    }
    inputTexture.render(520, 470);

    if (beSure)
    {
        //  popUp.render(400,400 );
    }
    // 1# load a window to ask their name
    // 3# check if its already there , if yes say welcome back

    SDL_RenderPresent(gRenderer);
}