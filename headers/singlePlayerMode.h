#ifndef SINGLE_PLAYER_MODE
#define SINGLE_PLAYER_MODE

#include "libHeaders.h"

void handleEvent_singlePlayerMode(SDL_Event e);
void handle_singlePlayerMode();

bool gameEndLogic();
void handleNextMove_SinglePlayer();

#endif 