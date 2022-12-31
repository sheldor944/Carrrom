#ifndef TWO_PLAYER_MODE
#define TWO_PLAYER_MODE

#include "libHeaders.h"

void initVariables(int TOTAL_PLAYERS);
void initAllPieces();
void handle_2PlayerMode();

// handles event 
void handleEvent_2PlayerMode(SDL_Event e);

// handles all types of rendering
void renderAllPieces();
void renderTurn();
void renderRemainingTime();
void renderSpeedBar();

// checks if the pieces having id from id1 to id2 are potted 
bool checkGameOver(int id1, int id2);

// place the red ball on the center if it is not covered
void handleFailedCover();

// checks who won the game
void gameWinLogic();

// prepres the game for the next move
void handleNextMove();

// puts back the piece id at the center
void handleInvalidPot(int id);

// places the striker for the next move
void placeStriker();

#endif