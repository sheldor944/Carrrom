#ifndef LEADERBOARD
#define LEADERBOARD 

#include "libHeaders.h"

#include <bits/stdc++.h>
using namespace std; 

// handle events and rendering
void handleEvent_leaderboard(SDL_Event e);
void handle_leaderboard();

// sort the leaderboard
void searchForLeaders();

// add new data
void addToTheLeaderBoard(double timeTaken , string playerName);

#endif 
