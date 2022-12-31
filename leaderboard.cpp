#include "headers/leaderboard.h"
#include "headers/libHeaders.h"
#include "headers/gFunctions.h"
#include "headers/music.h"

extern fstream leaderboardData ;
extern vector<pair<double ,pair<string ,string >>> topPlayerRecords ;

extern LTexture topPlayer ; 
extern LTexture secondPlayer ; 
extern LTexture thirdPlayer ; 
extern LTexture topPlayerScore ; 
extern LTexture secondPlayerScore ; 
extern LTexture thirdPlayerScore ; 


extern SDL_Color textColor;
extern TTF_Font *gFont;

extern SDL_Renderer* gRenderer  ; 
extern LTexture background ;
extern state currState ;
extern LTexture baseBackground ;

extern LTexture leaderboardBackground ;
extern LTexture justName ; 
extern LTexture justTime ;


void handleEvent_leaderboard(SDL_Event e)
{
    searchForLeaders();
    if(e.type== SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
    {
        
        currState = mainMenu ;
        playSelectSound();
    }

    topPlayer.loadFromRenderedText(topPlayerRecords[0].second.first.c_str() ,textColor , gFont);
    topPlayerScore.loadFromRenderedText(topPlayerRecords[0].second.second.c_str(),textColor,gFont);


    secondPlayer.loadFromRenderedText(topPlayerRecords[1].second.first.c_str() ,textColor , gFont);
    secondPlayerScore.loadFromRenderedText(topPlayerRecords[1].second.second.c_str(),textColor,gFont);


    thirdPlayer.loadFromRenderedText(topPlayerRecords[2].second.first.c_str() ,textColor , gFont);
    thirdPlayerScore.loadFromRenderedText(topPlayerRecords[2].second.second.c_str(),textColor,gFont);
}

void addToTheLeaderBoard(double timeTaken , string playerName)
{
    // cout << playerName << " " << timeTaken << endl;
    leaderboardData.open("leaderboardData.txt" , ios::app);
    leaderboardData <<playerName <<" "<<timeTaken<<endl;
    leaderboardData.close();
    
}
void searchForLeaders()
{
    leaderboardData.open("leaderboardData.txt");
    string player , line , timeInString; 
    topPlayerRecords.clear();
    double time ; 
    int i = 1 ; 
    while(leaderboardData>>line )
    {
        if(i&1)
        {
            player = line ;
        }
        else{
            time = stod(line);
            timeInString = line ;

            topPlayerRecords.push_back({time,{player,timeInString}});

        }

        i++;
    }
    sort(topPlayerRecords.begin(),topPlayerRecords.end());
    leaderboardData.close();
}

void handle_leaderboard()
{   
    SDL_RenderClear( gRenderer );
    SDL_SetRenderDrawColor ( gRenderer , 255 , 255 , 255, 255) ;
    SDL_RenderCopy ( gRenderer , baseBackground.mTexture, NULL , NULL );

    leaderboardBackground.render(400,110);

    justName.render(450,250 );
    justTime.render(700 , 250);

    topPlayer.render(450,320);
    topPlayerScore.render(700 ,320);

    secondPlayer.render(450 , 420 );
    secondPlayerScore.render(700 , 420 );

    thirdPlayer.render(450,520);
    thirdPlayerScore.render(700 , 520);

    SDL_RenderPresent( gRenderer );
    
}