//---------------------------------------------------------
// file:	gamestate_purplewins.c
// author:	Michael Howard
// email:	michael.howard@digipen.edu
//
// brief:	purple win screen
//
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
//
// Copyright � 2022 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "gamestate_gameplay.h"
#include "gamestate_menu.h"
#include "gamestate_purplewins.h"
CP_Color bgColor;
float timer5 = 0;
void gamestate_purple_init(void)
{
	timer5 = 0;
	bgColor = CP_Color_Create(100, 100, 100, 255);
	CP_Graphics_ClearBackground(bgColor);
}
void gamestate_purple_update(void)
{
	CP_Graphics_ClearBackground(bgColor);
	if (timer5 >= 5)
	{
		CP_Engine_SetNextGameStateForced(gamestate_menu_init, gamestate_menu_update, gamestate_menu_exit);
	}
	else
	{
		CP_Settings_TextSize(150.0f);
		char buffer[500] = { 0 };
		sprintf_s(buffer, _countof(buffer), "PURPLE WINS");
		CP_Font_DrawText(buffer, 10, 150);
	}
	timer5 += CP_System_GetDt();
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void gamestate_purple_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
}
