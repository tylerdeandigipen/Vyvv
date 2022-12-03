//---------------------------------------------------------
// file:	gamestate_orangewins.c
// author:	Michael Howard
// email:	michael.howard@digipen.edu
//
// brief:	orange win screen
//
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
//
// Copyright � 2022 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "gamestate_gameplay.h"
#include "gamestate_menu.h"
#include "gamestate_orangewins.h"
// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
CP_Color bgColor;
float timer4 = 0;
void gamestate_orange_init(void)
{
	timer4 = 0;
	bgColor = CP_Color_Create(100, 100, 100, 255);
	CP_Graphics_ClearBackground(bgColor);
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void gamestate_orange_update(void)
{
	CP_Graphics_ClearBackground(bgColor);
	if (timer4 >= 5)
	{
		CP_Engine_SetNextGameStateForced(gamestate_menu_init, gamestate_menu_update, gamestate_menu_exit);
	}
	else
	{
		CP_Settings_TextSize(150.0f);
		char buffer[500] = { 0 };
		sprintf_s(buffer, _countof(buffer), "ORANGE WINS");
		CP_Font_DrawText(buffer, -5, 150);
	}
	timer4 += CP_System_GetDt();
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void gamestate_orange_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
}
