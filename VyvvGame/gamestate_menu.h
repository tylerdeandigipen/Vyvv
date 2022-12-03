//---------------------------------------------------------
// file:	gamestate_menu.h
// author:	tyler dean, michael howard
// email:	tyler.dean@digipen.edu
//			michael.howard@didipen.edu
// 
// brief:	menu helper file
//
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once
//use this to add more icons
struct MenuIcons
{
	CP_Color lineColor;
	CP_Color fillColor;
	float iconX;
	float iconY;
	float buttonRadius;
	int red;
	int green;
	int blue;
	int highlighterR;
	int highlighterG;
	int highlighterB;

};

void gamestate_menu_init(void);
void gamestate_menu_update(void);
void gamestate_menu_exit(void);
//initialize icons
inline void initialize_icons(struct MenuIcons* icon)
{
	icon->iconX = 300;
	icon->iconY = 200;
	icon->red = 120;
	icon->green = 120;
	icon->blue = 255;
	//outline color
	icon->buttonRadius = 100;
	icon->highlighterR = 15;
	icon->highlighterG = 150;
	icon->highlighterB = 167;
}
//draw icons
inline void DrawIcons(struct MenuIcons* icon)
{
	CP_Settings_Fill(icon->fillColor);
	CP_Settings_Stroke(icon->lineColor);
	CP_Settings_StrokeWeight(5.0f);
	CP_Graphics_DrawCircle(icon->iconX, icon->iconY, icon->buttonRadius);
}