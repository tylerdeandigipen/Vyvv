//---------------------------------------------------------
// file:	gamestate_credits.c
// author:	tyler dean
// email:	tyler.dean@digipen.edu
//
// brief: credits scene
//
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "cprocessing.h"
#include "gamestate_menu.h"
#include "gamestate_credits.h"
#include <stdio.h>
#include <stdlib.h>
CP_Color bgColor;
float timer2 = 0;
// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void gamestate_credits_init(void)
{
	timer2 = 0;
	bgColor = CP_Color_Create(130, 10, 150, 255);
	CP_Graphics_ClearBackground(bgColor);
}
// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame

void gamestate_credits_update(void)
{
	CP_Graphics_ClearBackground(bgColor);
	if (timer2 >= 3)
	{
		CP_Engine_SetNextGameStateForced(gamestate_menu_init, gamestate_menu_update, gamestate_menu_exit);
	}
	else
	{
		CP_Settings_TextSize(20.0f);
		char buffer[500] = { 0 };
		sprintf_s(buffer, _countof(buffer), "Credits: Made by Tyler Dean  Michael Howard  TayLee Young, Instructed by Andy Ellinger");
		CP_Font_DrawText(buffer, 1, 15);
		sprintf_s(buffer, _countof(buffer), "All content Copyright 2022 DigiPen (USA) Corporation, all rights reserved.");
		CP_Font_DrawText(buffer, 1, 40);
	}
	timer2 += CP_System_GetDt();
}
// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void gamestate_credits_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
}