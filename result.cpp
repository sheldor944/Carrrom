#include "headers/result.h"
#include "headers/libHeaders.h"
#include "headers/gFunctions.h"
#include "headers/music.h"


extern SDL_Renderer *gRenderer;
extern LTexture baseBackground;
extern state currState;
extern LTexture inputTexture ; 
extern LTexture resultScreen ;

extern SDL_Color textColor;
extern TTF_Font *gFont;

extern string  player1, player2 , player3 , player4 ;

void handleEvent_result(SDL_Event e)
{
    if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE){
        currState = mainMenu;
        playSelectSound();
    }
}   

void winnerPresentation ( int id, int winner  )
{
    if(id == 1)
    {
        inputTexture.loadFromRenderedText(player1, textColor, gFont);
    }
    if(id==2)
    {
        string s ; 
        if ( winner == 0 )
        {
            s= player1 ;
        }
        else{
            s= player2;
        }
        inputTexture.loadFromRenderedText(s.c_str(), textColor, gFont);
        
    }
    if(id==4)
    {
        string s ;
        if(winner== 0 )
        {
            s= player1 + ", " + player2;
        }
        else{
            s= player3 + ", " + player4;
        }
        inputTexture.loadFromRenderedText(s.c_str(), textColor, gFont);

    }
}

void handle_result()
{
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

    SDL_RenderCopy(gRenderer, baseBackground.mTexture, NULL, NULL);
    resultScreen.render(400,100);
    inputTexture.render(650 - (inputTexture.mWidth / 2),813);

    SDL_RenderPresent(gRenderer);

}