#ifndef gFUNCTIONS
#define gFUNCTIONS

#include "circle.h"
#include "piece.h"

// the state of the program
enum state
{
	mainMenu,
	allModes,
	playerIntro,
	rules,
	leaderboard,
	singlePlayerMode,
	twoPlayerMode,
	fourPlayerMode,
	result,
	twoPlayerIntro,
	fourPlayerIntro,
	singlePlayerIntro,
	pauseState ,
	resultState ,
	singleModeScreen
};

extern enum state prevState;

struct Pair
{
	int left, right;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

// checks if the circles A and B are colling or not
bool checkCollision(circle A, circle B);

// checks if all pieces have stopped moving
bool motionOver();

#endif 


