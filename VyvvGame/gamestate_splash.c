//---------------------------------------------------------
// file:	gamestate_splash.c
// author:	tyler dean, michael howard
// email:	tyler.dean@digipen.edu
//			michael.howard@digipen.edu
//
// brief:	Splash screen
//
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
//
// Copyright © 2022 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "gamestate_splash.h"
#include "gamestate_menu.h"
#include <stdio.h>
#include <stdlib.h>
CP_Color bgColor;
CP_Image digiLogo;
CP_Image fmodLogo;
// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void gamestate_splash_init(void)
{
	digiLogo = CP_Image_Load("./Assets/DigiPen_BLACK.png");
	fmodLogo = CP_Image_Load("./Assets/FMOD Logo Black - White Background.png");
	bgColor = CP_Color_Create(100, 100, 100, 255);
	CP_Settings_RectMode(CP_POSITION_CENTER);
}
float timer = 0;
int opacityTemp = 255;
int opacityTemp2 = 0;

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void gamestate_splash_update(void)
{
	if (timer >= 2)
	{
		CP_Image_Draw(digiLogo, 400, 300, 1026 * (.7f - ((timer - 2) / 6)), 248 * (.7f - ((timer - 2) / 6)), CP_Math_LerpInt(opacityTemp, 0, timer - 2));
	}
	else
	{
		CP_Image_Draw(digiLogo, 400, 300, 1026 * .7f, 248 * .7f, opacityTemp);
	}
	if (timer >= 3.5)
	{
		CP_Image_Draw(fmodLogo, 400, 300, 728, 192, opacityTemp2);
		opacityTemp2 += 7;
		if (timer >= 6)
		{
			CP_Engine_SetNextGameStateForced(gamestate_menu_init, gamestate_menu_update, gamestate_menu_exit);
		}
	}
	else
	{
		CP_Image_Draw(fmodLogo, 400, 300, 728 * .7f, 192 * .7f, opacityTemp2);
	}
	timer += CP_System_GetDt();
	CP_Graphics_ClearBackground(bgColor);

	CP_Settings_TextSize(20.0f);
	char buffer[500] = { 0 };

	CP_Font_DrawText(buffer, 1, 90);
	sprintf_s(buffer, _countof(buffer), "All content Copyright 2022 DigiPen (USA) Corporation, all rights reserved.");
	CP_Font_DrawText(buffer, 5, 590);
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void gamestate_splash_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
}
