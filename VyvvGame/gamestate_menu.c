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
float buttonX = 400;
float buttonY = 300;
float buttonRadius = 100;
// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void gamestate_menu_init(void)
{

}
// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void gamestate_menu_update(void)
{
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Fill(CP_Color_Create(40, 40, 40, 255));
	CP_Settings_Stroke(CP_Color_Create(60, 60, 60, 255));
	CP_Settings_StrokeWeight(20.0f);
	CP_Graphics_DrawRect(0, 0, 800, 800);
	CP_Settings_RectMode(CP_POSITION_CENTER);

	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	CP_Settings_StrokeWeight(3.0f);
	char buffer[500] = { 0 };
	double tempX = CP_Input_GetMouseX();
	double tempY = CP_Input_GetMouseY();
	CP_Graphics_DrawCircle(buttonX, buttonY, buttonRadius);
	sprintf_s(buffer, _countof(buffer), "Play");
	CP_Settings_Fill(CP_Color_Create(40, 40, 40, 255));
	CP_Settings_Stroke(CP_Color_Create(40, 40, 40, 255));
	CP_Settings_TextSize(40.0f);
	CP_Font_DrawText(buffer, 367.50f, 311.50f);
	if (CP_Input_KeyDown(KEY_SPACE) && buttonRadius > sqrt((tempX - buttonX) * (tempX - buttonX)) + ((tempY - buttonY) * (tempY - buttonY)))
	{
		CP_Engine_SetNextGameStateForced(gamestate_gameplay_init, gamestate_gameplay_update, gamestate_gameplay_exit);
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
