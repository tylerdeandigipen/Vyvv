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
#include "gamestate_tutorial.h"
#include <stdio.h>
#include <stdlib.h>
CP_Color bgColor;
float timer3 = 0;
// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void gamestate_tutorial_init(void)
{
	timer3 = 0;
	bgColor = CP_Color_Create(100, 100, 100, 255);
	CP_Graphics_ClearBackground(bgColor);
}
// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame

void gamestate_tutorial_update(void)
{
	CP_Graphics_ClearBackground(bgColor);
	if (timer3 >= 10)
	{
		CP_Engine_SetNextGameStateForced(gamestate_menu_init, gamestate_menu_update, gamestate_menu_exit);
	}
	else
	{
		CP_Settings_TextSize(20.0f);
		char buffer[500] = { 0 };
		sprintf_s(buffer, _countof(buffer), "Two Boxes One Goal");
		CP_Font_DrawText(buffer, 1, 15);
		sprintf_s(buffer, _countof(buffer), "MOVEMENT USE LEFTJOYSTICK, ARROWS, OR WASD TO MOVE");
		CP_Font_DrawText(buffer, 1, 45);
		sprintf_s(buffer, _countof(buffer), "JUMP GAMEPAD A UPARROW W");
		CP_Font_DrawText(buffer, 1, 75);
		sprintf_s(buffer, _countof(buffer), "ATTACK E DOWNARROW GAMEPAD RIGHT TRIGGER");
		CP_Font_DrawText(buffer, 1, 105);
	}
	timer3 += CP_System_GetDt();
}
// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void gamestate_tutorial_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
}