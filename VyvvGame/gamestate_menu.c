//---------------------------------------------------------
// file:	gamestate_menu.c
// authors:	tyler dean, Michael Howard
// email:	tyler.dean@digipen.edu 
//			michael.howard@digipen.edu
//
// brief:	Main menu file
//
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
//
// Copyright © 2022 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "math.h"
#include "gamestate_menu.h"
#include "gamestate_gameplay.h"
#include "gamestate_credits.h"
#include "gamestate_tutorial.h"
#include <stdio.h>
#include <stdlib.h>

//helps with controller input
int highlight;
//to add more icons add number and make changes accordingly
struct MenuIcons icon1, icon2, icon3, icon4;
CP_Sound select;
// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void gamestate_menu_init(void)
{

	select = CP_Sound_Load("./Assets/blipSelect.wav");
	if (!CP_Input_GamepadConnected())
		highlight = -1;
	else
		highlight = 0;
	CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 255));
	initialize_icons(&icon1);
	initialize_icons(&icon2);
	initialize_icons(&icon3);
	initialize_icons(&icon4);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_Stroke(CP_Color_Create(255, 255, 255, 255));
	CP_Settings_StrokeWeight(3.0f);
}
// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void gamestate_menu_update(void)
{
	double tempX = CP_Input_GetMouseX();
	double tempY = CP_Input_GetMouseY();
	//make icons
	icon1.iconX = 200;
	icon1.iconY = 300;
	icon1.fillColor = CP_Color_Create(icon1.red, icon1.green, icon1.blue, 255);
	icon1.lineColor = CP_Color_Create(icon1.highlighterR, icon1.highlighterG, icon1.highlighterB, 255);
	icon2.iconX = 600;
	icon2.iconY = 300;
	icon2.lineColor = CP_Color_Create(icon2.highlighterR, icon2.highlighterG, icon2.highlighterB, 255);
	icon2.fillColor = CP_Color_Create(icon2.red, icon2.green, icon2.blue, 255);
	icon3.iconX = 200;
	icon3.iconY = 500;
	icon3.fillColor = CP_Color_Create(icon3.red, icon3.green, icon3.blue, 255);
	icon3.lineColor = CP_Color_Create(icon3.highlighterR, icon3.highlighterG, icon3.highlighterB, 255);
	icon4.iconX = 600;
	icon4.iconY = 500;
	icon4.lineColor = CP_Color_Create(icon4.highlighterR, icon4.highlighterG, icon4.highlighterB, 255);
	icon4.fillColor = CP_Color_Create(icon4.red, icon4.green, icon4.blue, 255);
	DrawIcons(&icon1);
	DrawIcons(&icon2);
	DrawIcons(&icon3);
	DrawIcons(&icon4);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));

	
	//navigate menu with controller
	if (highlight == 0)
	{
		if (CP_Input_GamepadTriggered(GAMEPAD_DPAD_UP))
			highlight = 0;
		if (CP_Input_GamepadTriggered(GAMEPAD_DPAD_LEFT))
			highlight = 0;
		if (CP_Input_GamepadTriggered(GAMEPAD_DPAD_RIGHT))
		{
			CP_Sound_Play(select);
			highlight = 1;
		}
		if (CP_Input_GamepadTriggered(GAMEPAD_DPAD_DOWN))
		{
			CP_Sound_Play(select);
			highlight = 2;
		}
	}
	if (highlight == 1)
	{
		if (CP_Input_GamepadTriggered(GAMEPAD_DPAD_UP))
			highlight = 1;
		if (CP_Input_GamepadTriggered(GAMEPAD_DPAD_LEFT))
		{
			CP_Sound_Play(select);
			highlight = 0;
		}
		if (CP_Input_GamepadTriggered(GAMEPAD_DPAD_RIGHT))
			highlight = 1;
		if (CP_Input_GamepadTriggered(GAMEPAD_DPAD_DOWN))
		{
			CP_Sound_Play(select);
			highlight = 3;
		}
	}
	if (highlight == 2)
	{
		if (CP_Input_GamepadTriggered(GAMEPAD_DPAD_UP))
		{
			CP_Sound_Play(select);
			highlight = 0;
		}
		if (CP_Input_GamepadTriggered(GAMEPAD_DPAD_LEFT))
			highlight = 2;
		if (CP_Input_GamepadTriggered(GAMEPAD_DPAD_RIGHT))
		{
			CP_Sound_Play(select);
			highlight = 3;
		}
		if (CP_Input_GamepadTriggered(GAMEPAD_DPAD_DOWN))
			highlight = 2;
	}
	if (highlight == 3)
	{
		if (CP_Input_GamepadTriggered(GAMEPAD_DPAD_UP))
		{
			CP_Sound_Play(select);
			highlight = 1;
		}
		if (CP_Input_GamepadTriggered(GAMEPAD_DPAD_LEFT))
		{
			CP_Sound_Play(select);
			highlight = 2;
		}
		if (CP_Input_GamepadTriggered(GAMEPAD_DPAD_RIGHT))
			highlight = 3;
		if (CP_Input_GamepadTriggered(GAMEPAD_DPAD_DOWN))
			highlight = 3;
	}
	//start
	//highlight icons
	if (icon1.buttonRadius / 2 > sqrt(((tempX - icon1.iconX) * (tempX - icon1.iconX)) + ((tempY - icon1.iconY) * (tempY - icon1.iconY))) || highlight == 0)
	{
		highlight = 0;
		icon1.highlighterR = 120;
		icon1.highlighterG = 120;
		icon1.highlighterB = 255;
		icon1.red = 255;
		icon1.green = 180;
		icon1.blue = 100;
	}
	else
	{
		if (!CP_Input_GamepadConnected())
			highlight = -1;
		else
			highlight = highlight;
		icon1.highlighterR = 255;
		icon1.highlighterG = 180;
		icon1.highlighterB = 100;
		icon1.red = 120;
		icon1.green = 120;
		icon1.blue = 255;
	}
	//tut
	if (icon2.buttonRadius / 2 > sqrt(((tempX - icon2.iconX) * (tempX - icon2.iconX)) + ((tempY - icon2.iconY) * (tempY - icon2.iconY))) || highlight == 1)
	{
		highlight = 1;
		icon2.highlighterR = 120;
		icon2.highlighterG = 120;
		icon2.highlighterB = 255;
		icon2.red = 255;
		icon2.green = 180;
		icon2.blue = 100;
	}
	else
	{
		if (!CP_Input_GamepadConnected())
			highlight = -1;
		else
			highlight = highlight;
		icon2.highlighterR = 255;
		icon2.highlighterG = 180;
		icon2.highlighterB = 100;
		icon2.red = 120;
		icon2.green = 120;
		icon2.blue = 255;
	}
	//cred
	if (icon3.buttonRadius / 2 > sqrt(((tempX - icon3.iconX) * (tempX - icon3.iconX)) + ((tempY - icon3.iconY) * (tempY - icon3.iconY))) || highlight == 2)
	{
		highlight = 2;
		icon3.highlighterR = 120;
		icon3.highlighterG = 120;
		icon3.highlighterB = 255;
		icon3.red = 255;
		icon3.green = 180;
		icon3.blue = 120;
	}
	else
	{
		if (!CP_Input_GamepadConnected())
			highlight = -1;
		else
			highlight = highlight;
		icon3.highlighterR = 255;
		icon3.highlighterG = 180;
		icon3.highlighterB = 100;
		icon3.red = 120;
		icon3.green = 120;
		icon3.blue = 255;
	}
	//ded(exit)
	if (icon4.buttonRadius / 2 > sqrt(((tempX - icon4.iconX) * (tempX - icon4.iconX)) + ((tempY - icon4.iconY) * (tempY - icon4.iconY))) || highlight == 3)
	{
		highlight = 3;
		icon4.highlighterR = 120;
		icon4.highlighterG = 120;
		icon4.highlighterB = 255;
		icon4.red = 255;
		icon4.green = 180;
		icon4.blue = 100;
	}
	else
	{
		if (!CP_Input_GamepadConnected())
			highlight = -1;
		else
			highlight = highlight;
		icon4.highlighterR = 255;
		icon4.highlighterG = 180;
		icon4.highlighterB = 100;
		icon4.red = 120;
		icon4.green = 120;
		icon4.blue = 255;
	}
	//start
	if (((highlight == 0 && CP_Input_GamepadDown(GAMEPAD_A))) || (CP_Input_KeyDown(KEY_SPACE)) && (icon1.buttonRadius / 2 > sqrt(((tempX - icon1.iconX) * (tempX - icon1.iconX)) + ((tempY - icon1.iconY) * (tempY - icon1.iconY)))))
	{
		CP_Engine_SetNextGameStateForced(gamestate_gameplay_init, gamestate_gameplay_update, gamestate_gameplay_exit);
	}
	//tut
	if (((highlight == 1 && CP_Input_GamepadDown(GAMEPAD_A))) || (CP_Input_KeyDown(KEY_SPACE)) && (icon2.buttonRadius / 2 > sqrt(((tempX - icon2.iconX) * (tempX - icon2.iconX)) + ((tempY - icon2.iconY) * (tempY - icon2.iconY)))))
	{
		CP_Engine_SetNextGameStateForced(gamestate_tutorial_init, gamestate_tutorial_update, gamestate_tutorial_exit);
	}
	//cred
	if (((highlight == 2 && CP_Input_GamepadDown(GAMEPAD_A))) || (CP_Input_KeyDown(KEY_SPACE)) && (icon3.buttonRadius / 2 > sqrt(((tempX - icon3.iconX) * (tempX - icon3.iconX)) + ((tempY - icon3.iconY) * (tempY - icon3.iconY)))))
	{
		CP_Engine_SetNextGameStateForced(gamestate_credits_init, gamestate_credits_update, gamestate_credits_exit);
	}
	//ded(exit)
	if (CP_Input_KeyTriggered(KEY_ESCAPE) || (highlight == 3 && CP_Input_GamepadDown(GAMEPAD_A)) || (CP_Input_KeyDown(KEY_SPACE) && (icon4.buttonRadius / 2 > sqrt(((tempX - icon4.iconX) * (tempX - icon4.iconX)) + ((tempY - icon4.iconY) * (tempY - icon4.iconY))))))
	{
		CP_Engine_Terminate();
	}
	CP_Settings_TextSize(150.0f);
	char buffer[500] = { 0 };
	sprintf_s(buffer, _countof(buffer), "BOX BRAWL");
	CP_Font_DrawText(buffer, 20, 150);
	CP_Settings_TextSize(20.0f);
	sprintf_s(buffer, _countof(buffer), "Game Play");
	CP_Font_DrawText(buffer, 155, 225);
	sprintf_s(buffer, _countof(buffer), "Tutorial");
	CP_Font_DrawText(buffer, 575, 225);
	sprintf_s(buffer, _countof(buffer), "Credits");
	CP_Font_DrawText(buffer, 155, 425);
	sprintf_s(buffer, _countof(buffer), "Death");
	CP_Font_DrawText(buffer, 575, 425);
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void gamestate_menu_exit(void)
{
	CP_Sound_Free(&select);
	// shut down the gamestate and cleanup any dynamic memory
}
