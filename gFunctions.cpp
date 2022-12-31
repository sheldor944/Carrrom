#include "headers/libHeaders.h"
#include "headers/circle.h"
#include "headers/gFunctions.h"
#include "headers/maths.h"
#include "headers/texture.h"
#include "headers/piece.h"
#include <fstream>

// Screen dimension constants
int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 980;

int totalWhitePieces;
int totalBlackPieces;
int totalPieces = 8;
piece pieces[8];

bool isGameRunning;

LTexture background;
LTexture board;
LTexture strikerError;
LTexture mainMenuScreen;
LTexture allRules;
LTexture baseBackground;
LTexture playButton;
LTexture allmodesScreen;
LTexture singlePlayerScreen;
LTexture glowingPlay;
LTexture glowingRules;
LTexture glowingSinglePlayer;
LTexture glowingRapid;
LTexture glowingVsComputer;
LTexture glowingTwoPlayer;
LTexture glowingFourPlayer;
LTexture rulesBackground;

LTexture loginMenu;
LTexture leaderboardBackground;
LTexture pauseMenu;
LTexture glowingResumeButton;
LTexture glowingExitButton;
LTexture glowingLeaderboardButton;
LTexture singleMode;
LTexture resultScreen;

// Music
Mix_Music *gMusic = NULL;
Mix_Music *clockTicSound = NULL;
Mix_Music *collisionSound = NULL;

Mix_Chunk *hardCollisionSound = NULL;
Mix_Chunk *selectionSound = NULL;

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The window renderer
SDL_Renderer *gRenderer = NULL;
LTexture gTextTexture;
LTexture whiteTurn, blackTurn;

TTF_Font *gFont = NULL;

// new
SDL_Color textColor = {0, 0, 0, 0xFF};

// Player intro
LTexture playerOneName;
LTexture PlayerTwoName;
LTexture PlayerThreeName;
LTexture PlayerFourName;
LTexture playerIntroInput;
LTexture inputTexture;

LTexture winner1, winner2;

string player1, player2, player3, player4;

LTexture topPlayer;
LTexture secondPlayer;
LTexture thirdPlayer;
LTexture topPlayerScore;
LTexture secondPlayerScore;
LTexture thirdPlayerScore;

LTexture justName;
LTexture justTime;

LTexture singlePlayerName;

// leader board file

fstream leaderboardData;

// vector to store all the top players
vector<pair<double, pair<string, string>>> topPlayerRecords;

int striked = 0;
int paused = 0;

state currState;
state prevState;

bool init()
{
	// Initialization flag
	bool success = true;

	// initially, the game is not running
	isGameRunning = false;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		// Create window
		gWindow = SDL_CreateWindow("CAROM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			// Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				// Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				// Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				// Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}

				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	bool success = true;

	// open font
	gFont = TTF_OpenFont("fonts/Roboto-Black.ttf", 40);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		// Render text
		SDL_Color textColor = {0, 0, 0};
		if (!gTextTexture.loadFromRenderedText("TEST", textColor, gFont))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
	}
	// convert font to images
	SDL_Color textColor = {0, 0, 0};
	if (!whiteTurn.loadFromRenderedText("WHITE'S TURN", textColor, gFont))
	{
		printf("Failed to render text texture!\n");
		success = false;
	}

	if (!blackTurn.loadFromRenderedText("BLACK'S TURN", textColor, gFont))
	{
		printf("Failed to render text texture!\n");
		success = false;
	}

	// load required images
	if (!background.loadFromFile("images/background.png"))
	{
		printf("Background could not be loaded\n");
		success = false;
	}

	if (!board.loadFromFile("images/board.png"))
	{
		printf("Board could not be loaded\n");
		success = false;
	}

	if (!strikerError.loadFromFile("images/error.png"))
	{
		printf("Striker Error could not be loaded\n");
		success = false;
	}

	if (!pieces[0].texture.loadFromFile("images/striker.png"))
	{
		printf("Image could not be loaded\n");
		success = false;
	}

	for (int i = 1; i < 4; i++)
	{
		if (!pieces[i].texture.loadFromFile("images/white2.png"))
		{
			printf("Image could not be loaded\n");
			success = false;
		}
	}

	if (!pieces[4].texture.loadFromFile("images/red.png"))
	{
		printf("Image could not be loaded\n");
		success = false;
	}

	for (int i = 5; i < totalPieces; i++)
	{
		if (!pieces[i].texture.loadFromFile("images/black2.png"))
		{
			printf("Image could not be loaded\n");
			success = false;
		}
	}

	// mainMenu and UI
	if (!mainMenuScreen.loadFromFile("images/mainMenu.jpeg"))
	{
		printf("Image could not be loaded for mainMeny\n");
		success = false;
	}
	if (!allRules.loadFromFile("images/rules.jpeg"))
	{
		printf("Rules cannot be loaded \n");
		success = false;
	}
	if (!baseBackground.loadFromFile("images/baseBackground.jpeg"))
	{
		printf("Base Background could not be loaded\n");
		success = false;
	}
	if (!playButton.loadFromFile("images/black.png"))
	{
		printf("Could not load play Button \n");
		success = false;
	}
	if (!allmodesScreen.loadFromFile("images/allmode.jpeg"))
	{
		printf("Couild not load all mode menu \n");
		success = false;
	}
	if (!singlePlayerScreen.loadFromFile("images/singlePlayer.jpeg"))
	{
		printf("Couild not load all single player menu  \n");
		success = false;
	}
	if (!glowingPlay.loadFromFile("images/glowingPlay.png"))
	{
		printf("Couild not load all glowing play   \n");
		success = false;
	}
	// edit neederd
	if (!glowingPlay.loadFromFile("images/glowingPlay.png"))
	{
		printf("Couild not load all glowing play   \n");
		success = false;
	}
	if (!glowingRules.loadFromFile("images/glowingRules.png"))
	{
		printf("Couild not load all glowingRules   \n");
		success = false;
	}

	if (!glowingSinglePlayer.loadFromFile("images/glowingSinglePlayer.png"))
	{
		printf("Couild not load all glowingSinglePlayer   \n");
		success = false;
	}
	if (!glowingRapid.loadFromFile("images/glowingRapid.png"))
	{
		printf("Couild not load all glowingRapid   \n");
		success = false;
	}
	if (!glowingVsComputer.loadFromFile("images/glowingVsComputer.png"))
	{
		printf("Couild not load all glowingVsComputer   \n");
		success = false;
	}
	if (!glowingTwoPlayer.loadFromFile("images/glowingTwoPlayer.png"))
	{
		printf("Couild not load all glowingTwoPlayer   \n");
		success = false;
	}
	if (!glowingFourPlayer.loadFromFile("images/glowingFourPlayer.png"))
	{
		printf("Couild not load all glowingFourPlayer   \n");
		success = false;
	}

	if (!loginMenu.loadFromFile("images/loginMenu.png"))
	{
		printf("Couild not load all loginMenu   \n");
		success = false;
	}
	if (!leaderboardBackground.loadFromFile("images/leaderboardImage.png"))
	{
		printf("Couild not load leaderboardImage   \n");
		success = false;
	}
	if (!pauseMenu.loadFromFile("images/pauseMenu.png"))
	{
		printf("Couild not load pauseMann   \n");
		success = false;
	}
	if (!glowingResumeButton.loadFromFile("images/glowingResumeButton.png"))
	{
		printf("Couild not glowingResumeButton   \n");
		success = false;
	}
	if (!glowingExitButton.loadFromFile("images/glowingExitButton.png"))
	{
		printf("Couild not glowingExitButton   \n");
		success = false;
	}
	if (!singleMode.loadFromFile("images/singleMode.jpeg"))
	{
		printf("Couild not singleMode   \n");
		success = false;
	}
	if (!glowingLeaderboardButton.loadFromFile("images/glowingLeaderboardButton.png"))
	{
		printf("Couild not glowingLeaderboardButton   \n");
		success = false;
	}
	if (!resultScreen.loadFromFile("images/resultScreen.png"))
	{
		printf("Couild not resultScreen   \n");
		success = false;
	}
	if (!rulesBackground.loadFromFile("images/rulesBackground.jpeg"))
	{
		printf("Couild not rrulesScreen   \n");
		// baki
		success = false;
	}

	// Loadin music
	// Mix_OpenAudio(48000 , AUDIO_S16SYS , 6.1 , 4096);
	gMusic = Mix_LoadMUS("music/test.mp3");
	if (gMusic == NULL)
	{
		printf("Could not load music \n");
		success = false;
	}
	clockTicSound = Mix_LoadMUS("music/clockTicSound.mp3");

	if (clockTicSound == NULL)
	{
		printf("Could not load musclockTicSoundic \n");
		success = false;
	}
	collisionSound = Mix_LoadMUS("music/sound_ballhitsballhard.wav");
	if (collisionSound == NULL)
	{
		printf("Could not load collisionSound \n");
		success = false;
	}

	hardCollisionSound = Mix_LoadWAV("music/hardCollision.wav");

	if (hardCollisionSound == NULL)
	{
		printf("could not load HardCollision \n");
		success = false;
	}

	selectionSound = Mix_LoadWAV("music/selectionSound.wav");

	if (selectionSound == NULL)
	{
		printf(" Could not load selsction sound \n");
		success = false;
	}

	// new
	if (!playerOneName.loadFromRenderedText("Player  1 Name  :", textColor, gFont))
	{
		printf("Failed to render prompt text!\n");
		success = false;
	}
	if (!PlayerTwoName.loadFromRenderedText("Player 2 Name  :", textColor, gFont))
	{
		printf("Failed to render prompt text!\n");
		success = false;
	}
	if (!PlayerThreeName.loadFromRenderedText("Player 3 Name  :", textColor, gFont))
	{
		printf("Failed to render prompt text!\n");
		success = false;
	}
	if (!PlayerFourName.loadFromRenderedText("Player 4 Name  :", textColor, gFont))
	{
		printf("Failed to render prompt text!\n");
		success = false;
	}
	if (!singlePlayerName.loadFromRenderedText("Enter the Players Name:", textColor, gFont))
	{
		printf("Failed to render prompt text!\n");
		success = false;
	}
	if (!justName.loadFromRenderedText("Player Name", textColor, gFont))
	{
		printf("Failed to render prompt text!\n");
		success = false;
	}
	if (!justTime.loadFromRenderedText("time", textColor, gFont))
	{
		printf("Failed to render prompt text!\n");
		success = false;
	}

	return success;
}

void close()
{
	mainMenuScreen.free();
	allRules.free();
	baseBackground.free();
	playButton.free();
	allmodesScreen.free();
	singlePlayerScreen.free();
	glowingPlay.free();
	glowingRules.free();
	glowingSinglePlayer.free();
	glowingRapid.free();
	glowingVsComputer.free();
	glowingTwoPlayer.free();
	glowingFourPlayer.free();
	gTextTexture.free();

	// Free loaded images
	for (int i = 0; i < totalPieces; i++)
	{
		pieces[i].freeTexture();
	}

	background.free();
	board.free();

	// Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	TTF_CloseFont(gFont);
	gWindow = NULL;
	gRenderer = NULL;
	gFont = NULL;

	// Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool checkCollision(circle A, circle B)
{
	bool collided = 1;

	double sumOfRadii = A.r + B.r;
	double sqOfSum = sumOfRadii * sumOfRadii;
	if (distanceSq(A.x, A.y, B.x, B.y) >= sqOfSum)
		collided = 0;

	return collided;
}

bool motionOver()
{
	bool over = 1;
	for (int i = 0; i < totalPieces; i++)
	{
		if (pieces[i].potted)
			continue;
		if (abs(pieces[i].mVelX) >= 0.005 || abs(pieces[i].mVelY) >= 0.005)
		{
			over = 0;
			break;
		}
		pieces[i].mVelX = pieces[i].mVelY = 0;
	}

	return over;
}