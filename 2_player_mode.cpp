#include "headers/libHeaders.h"
#include "headers/2_player_mode.h"
#include "headers/piece.h"
#include "headers/gFunctions.h"
#include "headers/timer.h"
#include "headers/result.h"
using namespace std;

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern LTexture gTextTexture;
extern LTexture whiteTurn;
extern LTexture blackTurn;

extern string player1, player2, player3, player4;

extern LTexture background;
extern LTexture board;
extern LTexture strikerError;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern int striked;
extern enum state currState;

extern int totalWhitePieces;
extern int totalBlackPieces;
extern int totalPieces;
extern piece pieces[];
extern bool isGameRunning;

// adding fonts 
extern TTF_Font *gFont ;

// true if all required pieces have been potted for a particular player
bool allPotted;

// true if a player has potted one of his pieces in the last move
bool pottedLastMove;

// stores the ID of the piece that was potted in the last move
// -1 indicates that no piece was potted in the last move
int lastPottedPieceID;

// id of the current player
// in this mode, it can be 0 or 1
int totalPlayers;
int teamID;
int currPlayerID;
int winnerTeamID;

// id of the red ball
int redID = 4;
bool redPottedLastMove;
vector<int> pottedPieces[2];

// for both players, strikerLimit[] stores how much the striker can be moved
Pair strikerLimit[4];

// teamPieces[i] contains a pair [left, right], which are the pieces that player i has to pot
// for example, teamPieces[i] = {3, 8} means that the i'th player has to pot the pieces that have id from 3 to 8
Pair teamPieces[] = {Pair{1, 4}, Pair{4, 7}};

LTimer CLK;
int time_now;
int allottedTime = 15;

void initVariables(int TOTAL_PLAYERS)
{
    totalPlayers = TOTAL_PLAYERS;
    redPottedLastMove = false;
    teamID = 0;
    currPlayerID = 0;
    winnerTeamID = 0;
    lastPottedPieceID = -1;
    allPotted = false;
    pottedLastMove = 0;
    totalWhitePieces = 3;
    totalBlackPieces = 3;
    striked = 0;

    for(int i = 0; i < 2; i++) pottedPieces[i].clear();
    if(TOTAL_PLAYERS == 1) teamPieces[0] = Pair{1, 7};
    else{
        teamPieces[0] = Pair{1, 4};
        teamPieces[1] = Pair{4, 7};
    }

    if(TOTAL_PLAYERS <= 2){
        // {Pair{195, 785}, Pair{200, 780}}
        strikerLimit[0] = Pair{195, 785};
        strikerLimit[1] = Pair{200, 780};
    }
    else if(TOTAL_PLAYERS == 4){
        strikerLimit[0] = Pair{195, 785};
        strikerLimit[1] = Pair{195, 785};
        strikerLimit[2] = Pair{200, 785};
        strikerLimit[3] = Pair{195, 785};
    }

    CLK.start();
}

void initAllPieces()
{
    pieces[0].initialize(490 - 20, 800 - 20);

    if(currState == singlePlayerMode){
        pieces[1].initialize(150, 490);
        pieces[2].initialize(480, 150);
        pieces[3].initialize(790, 490);
        pieces[4].initialize(200 - 20, 200 - 20);
        pieces[5].initialize(790 - 20, 200 - 20);
        pieces[6].initialize(790 - 20, 790 - 20);
        pieces[7].initialize(200 - 20, 790 - 20);
        
        return;
    }
    
    double radius = 20;
    double boardCenter = 490;
    double shift = boardCenter - radius;
    
    pieces[4].initialize(shift, shift);
    pieces[2].initialize(shift + 2.0 * radius, shift);
    pieces[3].initialize(shift + radius, shift + sqrt(3.0) * radius);
    pieces[1].initialize(shift - radius, shift + sqrt(3.0) * radius);
    pieces[5].initialize(shift - 2.0 * radius, shift);
    pieces[6].initialize(shift - radius, shift - sqrt(3.0) * radius);
    pieces[7].initialize(shift + radius, shift - sqrt(3.0) * radius);
}

void handleEvent_2PlayerMode(SDL_Event e)
{
    // check if player wants to exit the game
    if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP){
        if(e.key.keysym.sym == SDLK_ESCAPE){
            // isGameRunning = false;
            CLK.pause();
            prevState = currState;
            currState = pauseState;
        }
        else{
            pieces[0].handleEvent(e, currPlayerID, totalPlayers);
        }
    }
}

void handle_2PlayerMode()
{
    // initialize the game variables if the game just started
    if(!isGameRunning){
        cout << "Initialized 2P\n";
        initVariables(2);
        initAllPieces();
        isGameRunning = true;
    }

    if(striked){
        // move all the pieces
        for(int i = 0; i < totalPieces; i++){
            if(!pieces[i].potted) pieces[i].move(i, teamID, currPlayerID);
        }

        // initialize position of the striker after all pieces stop moving
        if(motionOver()){
            gameWinLogic();

            if(!allPotted){
                handleNextMove();
            }
        }
    }
    else{
        time_now = CLK.getTicks() / 1000;
        if(time_now >= allottedTime){
            handleNextMove();
        }
        else pieces[0].move(0, teamID, currPlayerID);
    }

    if(allPotted){
        isGameRunning = false;
        winnerPresentation(2, winnerTeamID);
        currState = result;
        player1 = player2 = player3 = player4 = "";
        return;
    }

    renderAllPieces();
}

void renderAllPieces()
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // clearing the screen
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0xFF);
    SDL_RenderClear(gRenderer);
    
    // rendering the background and the board
    SDL_RenderCopy(gRenderer, background.mTexture, NULL, NULL);
    board.render(0, 0);

    if(!striked){
        // show the line parallel to which the striker will move if striked
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderDrawLine(gRenderer, (pieces[0].mPosX * 2 + pieces[0].texture.getWidth()) / 2, (pieces[0].mPosY * 2 + pieces[0].texture.getHeight()) / 2, mouseX, mouseY);
    }

    for(int i = 1; i < totalPieces; i++){
        if(!pieces[i].potted) pieces[i].render();
    }

    if(pieces[0].possibleToStrike) pieces[0].render();
    else strikerError.render(pieces[0].mPosX, pieces[0].mPosY);

    if(!striked){
        renderTurn();
        renderRemainingTime();
        renderSpeedBar();
    }

    SDL_RenderPresent(gRenderer);
}

void renderTurn()
{
    if(currState == singlePlayerMode) return;
    if(teamID == 0) whiteTurn.render(1000, 100);
    else blackTurn.render(1000, 100);
}

void renderRemainingTime()
{
    if(currState == singlePlayerMode){
        LTexture currTime;

        int ticks = (CLK.getTicks() / 1000.0);
        if(!currTime.loadFromRenderedText(to_string(ticks), SDL_Color{0, 0, 0} , gFont) ){
            printf("Error while loading from rendered text\n");
            return;
        }

        currTime.render(1000, 150);
        return;
    }

    SDL_SetRenderDrawColor(gRenderer, 0, 0xFF, 0, 0xFF);

    SDL_Rect timeBar = {1000, 150, 15.0 * ((double) allottedTime - (double) CLK.getTicks() / 1000), 20};
    SDL_RenderFillRect(gRenderer, &timeBar);
}

void renderSpeedBar()
{
    // write speed above speed bar
    SDL_Color textColor = { 0, 0, 0 };
    if(!gTextTexture.loadFromRenderedText("SPEED:", textColor , gFont)){
        printf( "Failed to render text texture!\n" );
    }
    else gTextTexture.render(1000, 850);

    // creating outline of the speedbar
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
    SDL_Rect speedBarOutline = {1000, 898, 182, 23};
    SDL_RenderDrawRect(gRenderer, &speedBarOutline);

    SDL_SetRenderDrawColor(gRenderer, 0, 0xFF, 0, 0xFF);

    // making a 180 by 20 rectangle to render the speed bar
    // chose 180 simply because it is a multiple of 60, which is the speed of the piece
    SDL_Rect speedBar = {1000, 900, 3 * pieces[0].speed, 20};
    SDL_RenderFillRect(gRenderer, &speedBar);
}

bool checkGameOver(int id1, int id2)
{
	bool over = 1;
	for(int i = id1; i <= id2; i++){
		if(!pieces[i].potted){
			over = 0;
			break;
		}
	}
	return over;
}

void handleFailedCover()
{
    redPottedLastMove = false;
    // pieces[redID].initialize(490 - pieces[redID].collider.r, 490 - pieces[redID].collider.r);
    handleInvalidPot(redID);
}

void gameWinLogic()
{
    if(checkGameOver(teamPieces[teamID].left, teamPieces[teamID].right)) {
        allPotted = true;
        winnerTeamID = teamID;
    }
    else if(checkGameOver(teamPieces[teamID ^ 1].left, teamPieces[teamID ^ 1].right)){
        allPotted = true;
        winnerTeamID = teamID ^ 1;
    }
}

void handleNextMove()
{
    // stop the timer
    if(CLK.isStarted()) CLK.stop();
    // prepare the striker for striking again
    striked = 0;

    // change player id if no piece was potted in last move
    if(!pottedLastMove){ 
        // handle the case where a player has failed to cover the red piece
        if(redPottedLastMove) handleFailedCover();
        if(!pieces[redID].potted && pottedPieces[teamID ^ 1].size() == totalWhitePieces){
            handleInvalidPot(pottedPieces[teamID ^ 1].back());
            pottedPieces[teamID ^ 1].pop_back();
        }

        // for(int i = 0; i < 2; i++){
        //     if(pottedPieces[i].size() == 3){
        //         handleInvalidPot(pottedPieces[i].back());
        //         pottedPieces[i].pop_back();
        //     }
        // }

        // change player id
        teamID ^= 1;
        currPlayerID = (currPlayerID + 1) % totalPlayers;
    }
    else{
        // checking if the red piece has been covered
        if(redPottedLastMove){
            // checking if it has been covered
            if(lastPottedPieceID != redID) redPottedLastMove = false;
            // else{
            //     handleFailedCover();
            //     if(pottedPieces[teamID].size() == totalWhitePieces){
            //         handleInvalidPot(pottedPieces[teamID].back());
            //         pottedPieces[teamID].pop_back();
            //         teamID ^= 1;
            //         currPlayerID = (currPlayerID + 1) % totalPlayers;
            //     }
            //     if(pottedPieces[teamID ^ 1].size() == totalWhitePieces){
            //         handleInvalidPot(pottedPieces[teamID ^ 1].back());
            //         pottedPieces[teamID ^ 1].pop_back();
            //     }
            // }
        }
        else{
            if(!pieces[redID].potted && pottedPieces[teamID].size() == totalWhitePieces){
                handleInvalidPot(pottedPieces[teamID].back());
                pottedPieces[teamID].pop_back();
                teamID ^= 1;
                currPlayerID = (currPlayerID + 1) % totalPlayers;
            }
            if(!pieces[redID].potted && pottedPieces[teamID ^ 1].size() == totalWhitePieces){
                handleInvalidPot(pottedPieces[teamID ^ 1].back());
                pottedPieces[teamID ^ 1].pop_back();
            }
        }
    }
    placeStriker();
    
    // before striking, no piece can be potted
    pottedLastMove = 0;

    // restart the timer for the next move
    if(!CLK.isStarted()) CLK.start();
}

void handleInvalidPot(int id)
{
    pieces[id].potted = false;
    double boardCenter = 490, shift = 0;
    double r = pieces[id].collider.r;
    bool put = false;
    int idx = 0;

    while(!put){
        circle C = circle{boardCenter + shift * (idx & 1 ? 1 : -1), boardCenter, r};
        bool colliding = false;
        for(int i = 1; i < totalPieces; i++){
            if(i == id || pieces[id].potted) continue;
            if(checkCollision(C, pieces[i].collider)){
                colliding = true;
                break;
            }
        }
        if(!colliding){
            pieces[id].initialize(boardCenter + shift - r, boardCenter - r);
            put = true;
        }

        if(idx & 1) shift += r * 2.0;
        idx++;
    }
}

void placeStriker()
{
    if(totalPlayers == 4){
        switch(currPlayerID){
            case 0: pieces[0].initialize(490 - 20, 800 - 20); break;
            case 1: pieces[0].initialize(800 - 20, 490 - 20); break;
            case 2: pieces[0].initialize(490 - 20, 180 - 20); break;
            case 3: pieces[0].initialize(180 - 20, 490 - 20); break;
        }
    }
    else if(totalPlayers <= 2){
        switch(currPlayerID){
            case 0: pieces[0].initialize(490 - 20, 800 - 20); break;
            case 1: pieces[0].initialize(490 - 20, 180 - 20); break;
        }
    }
}