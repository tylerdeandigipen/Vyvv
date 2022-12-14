//---------------------------------------------------------
// file:	gamestate_splash.c
// author:	tyler dean
// email:	tyler.dean@digipen.edu
//
// brief:	Splash screen
//
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
//
// Copyright ? 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "gamestate_splash.h"
#include "gamestate_menu.h"
CP_Color bgColor;
CP_Image digiLogo;
// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void gamestate_splash_init(void)
{
	digiLogo = CP_Image_Load("./Assets/DigiPen_BLACK.png");
	bgColor = CP_Color_Create(130, 10, 150, 255);
	CP_Settings_RectMode(CP_POSITION_CENTER);
}
float timer = 0;
int opacityTemp = 255;
// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void gamestate_splash_update(void)
{
	if (timer >= 2)
	{
		CP_Image_Draw(digiLogo, 400, 300, 1026 * (.7f - ((timer - 2) / 6)), 248 * (.7f - ((timer - 2) / 6)), CP_Math_LerpInt(opacityTemp, 0, timer - 2));
		if (timer >= 3)
		{
			CP_Engine_SetNextGameStateForced(gamestate_menu_init, gamestate_menu_update, gamestate_menu_exit);
		}
	}
	else
	{
		CP_Image_Draw(digiLogo, 400, 300, 1026 * .7f, 248 * .7f, opacityTemp);
	}
	timer += CP_System_GetDt();
	CP_Graphics_ClearBackground(bgColor);

}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void gamestate_splash_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
}
