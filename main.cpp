#include "headers/libHeaders.h"
#include "headers/gFunctions.h"
#include "headers/mainMenu.h"
#include "headers/allModes.h"
#include "headers/playerIntro.h"
#include "headers/rules.h"
#include "headers/leaderboard.h"
#include "headers/singlePlayerMode.h"
#include "headers/2_player_mode.h"
#include "headers/fourPlayerMode.h"
#include "headers/twoPlayerIntro.h"
#include "headers/result.h"
#include "headers/fourPlayerIntro.h"
#include "headers/singlePlayerIntro.h"
#include "headers/pauseManu.h"
#include "headers/singleMode.h"


extern enum state currState;
extern bool isGameRunning;

int main()
{
	//Start up SDL and create window
	if( !init() ) printf( "Failed to initialize!\n" );
	else{
		//Load media
		if( !loadMedia() ) printf( "Failed to load media!\n" );
		else{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;
			isGameRunning = false;
			// initAllPieces();

			currState = mainMenu;

			//While application is running
			while( !quit ){
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT ) quit = true;

					switch(currState){
						case mainMenu: handleEvent_mainMenu(e); break;
						case allModes: handleEvent_allModes(e); break;
						case playerIntro: handleEvent_playerIntro(e); break;
						case rules: handleEvent_rules(e); break;
						case leaderboard: handleEvent_leaderboard(e); break;
						case singlePlayerMode: handleEvent_singlePlayerMode(e); break;
						case twoPlayerMode: handleEvent_2PlayerMode(e); break;
						case fourPlayerMode: handleEvent_4PlayerMode(e); break;
						case result : handleEvent_result(e) ; break ; 
						case twoPlayerIntro : handleEvent_twoPlayerIntro(e) ;break;
						case fourPlayerIntro : handleEvent_fourPlayerIntro(e) ; break ;
						case singlePlayerIntro : handleEvent_singlePlayerIntro(e) ; break ;
						case pauseState : handleEvent_pauseManu(e) ; break ; 
						case singleModeScreen :handleEvent_singleMode(e); break ;
						// case result: handleEvent_result(e); break;
					}
				}

				switch(currState){
					case mainMenu: handle_mainMenu(); break;
					case allModes: handle_allModes(); break;
					case playerIntro: handle_playerIntro(); break;
					case rules: handle_rules(); break;
					case leaderboard: handle_leaderboard(); break;
					case singlePlayerMode: handle_singlePlayerMode(); break;
					case twoPlayerMode: handle_2PlayerMode(); break;
					case fourPlayerMode: handle_4PlayerMode(); break;
					case result : handle_result(); break ;
					case twoPlayerIntro : handle_twoPlayerIntro() ; break ; 
					case fourPlayerIntro : handle_fourPlayerIntro(); break ;
					case singlePlayerIntro : handle_singlePlayerIntro(); break ; 
					case pauseState : handle_pauseMenu();break ;
					case singleModeScreen : handle_singleMode(); break; 
					// case result: handle_result(); break;
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}