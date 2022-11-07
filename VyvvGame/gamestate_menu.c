//---------------------------------------------------------
// file:	gamestate_menu.c
// author:	tyler dean
// email:	tyler.dean@digipen.edu
//
// brief:	Main menu file
//
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "math.h"
#include "gamestate_menu.h"
#include "gamestate_gameplay.h"
#include "gamestate_credits.h"
#include <stdio.h>
#include <stdlib.h>
double buttonX = 200;
double buttonY = 300;
float buttonRadius = 100;
// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void gamestate_menu_init(void)
{
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	CP_Settings_StrokeWeight(3.0f);
}
// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void gamestate_menu_update(void)
{
	CP_Settings_TextSize(20.0f);
	char buffer[500] = { 0 };
	double tempX = CP_Input_GetMouseX();
	double tempY = CP_Input_GetMouseY();
	CP_Graphics_DrawCircle(200, 300, buttonRadius);
	sprintf_s(buffer, _countof(buffer), "Game Play");
	CP_Font_DrawText(buffer, 155, 225);
	if (CP_Input_KeyDown(KEY_SPACE) && buttonRadius > sqrt((tempX - buttonX) * (tempX - buttonX)) + ((tempY - buttonY) * (tempY - buttonY)))
	{
		CP_Engine_SetNextGameStateForced(gamestate_gameplay_init, gamestate_gameplay_update, gamestate_gameplay_exit);
	}
	CP_Graphics_DrawCircle(600, 300, buttonRadius);
	sprintf_s(buffer, _countof(buffer), "Death");
	CP_Font_DrawText(buffer, 575, 225);
	if (CP_Input_KeyDown(KEY_SPACE) && buttonRadius > sqrt((tempX - 600) * (tempX - 600)) + ((tempY - buttonY) * (tempY - buttonY)))
	{
		CP_Engine_SetNextGameStateForced(gamestate_credits_init, gamestate_credits_update, gamestate_credits_exit);
	}
	if (CP_Input_KeyTriggered(KEY_ESCAPE))
	{
		CP_Engine_Terminate();
	}
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void gamestate_menu_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
}
