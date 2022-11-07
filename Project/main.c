//---------------------------------------------------------
// file:	main.c
// author:	tyler dean
// email:	tyler.dean@digipen.edu
//
// brief:	Main entry point for the sample project
//			of the CProcessing library
//
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "gamestate_splash.h"

void game_init(void);
void game_update(void);
void game_exit(void);

void pre_update()
{
	if (CP_Input_KeyTriggered(KEY_ESCAPE))
		CP_Engine_Terminate();
}
// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void game_init(void)
{
	// initialize variables and CProcessing settings for this gamestate
	
	CP_System_SetWindowSize(800, 600);
	CP_Graphics_ClearBackground(CP_Color_Create(130, 10, 150, 255));
	CP_Engine_SetNextGameState(gamestate_splash_init, gamestate_splash_update, gamestate_splash_exit);
}

void game_update(void)
{
	CP_Engine_SetNextGameState(gamestate_splash_init, gamestate_splash_update, gamestate_splash_exit);
}
void game_exit(void)
{
}

// main() the starting point for the program
// CP_Engine_SetNextGameState() tells CProcessing which functions to use for init, update and exit
// CP_Engine_Run() is the core function that starts the simulation
int main(void)
{
	//CP_Engine_SetPreUpdateFunction(pre_update);
	CP_Engine_SetNextGameState(game_init, game_update, game_exit);
	CP_Engine_Run();
	return 0;
}
