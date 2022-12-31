#include "headers/libHeaders.h"
#include "headers/fourPlayerMode.h"
#include "headers/piece.h"
#include "headers/gFunctions.h"
#include "headers/timer.h"
#include "headers/2_player_mode.h"
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

// true if all required pieces have been potted for a particular player
extern bool allPotted;

// true if a player has potted one of his pieces in the last move
extern bool pottedLastMove;

// stores the ID of the piece that was potted in the last move
// -1 indicates that no piece was potted in the last move
extern int lastPottedPieceID;

// id of the current player
// in this mode, it can be 0 or 1
extern int totalPlayers;
extern int teamID;
extern int currPlayerID;
extern int winnerTeamID;

// id of the red ball
extern int redID;
extern bool redPottedLastMove;
extern vector<int> pottedPieces[2];

// for both players, strikerLimit[] stores how much the striker can be moved
extern Pair strikerLimit[];

// teamPieces[i] contains a pair [left, right], which are the pieces that player i has to pot
// for example, teamPieces[i] = {3, 8} means that the i'th player has to pot the pieces that have id from 3 to 8
extern Pair teamPieces[];

extern LTimer CLK;
extern int time_now;
extern int allottedTime;

void handleEvent_4PlayerMode(SDL_Event e)
{
    // check if player wants to exit the game
    // if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP){
    //     if(e.key.keysym.sym == SDLK_ESCAPE){
    //         isGameRunning = false;
    //         currState = mainMenu;
    //     }
    //     else{
    //         pieces[0].handleEvent(e, currPlayerID, totalPlayers);
    //     }
    // }
    // handleEvent_2PlayerMode(e, currPlayerID);

    handleEvent_2PlayerMode(e);
}

void handle_4PlayerMode()
{
    // initialize the game variables if the game just started
    if(!isGameRunning){
        cout << "Calling initVariables\n";
        initVariables(4);
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
        winnerPresentation(4, winnerTeamID);
        currState = result;
        player1 = player2 = player3 = player4 = "";
        return;
    }

    renderAllPieces();
}