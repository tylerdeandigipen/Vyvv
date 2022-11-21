//---------------------------------------------------------
// file:	gamestate_gameplay.c
// author:	tyler dean
// email:	tyler.dean@digipen.edu
//
// brief:	Main gameplay file
//
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "gamestate_gameplay.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "gamestate_credits.h"
#pragma warning(disable:4996)
CP_Color bgColor;
float globalXOffset = 0;
float globalYOffset = 0;
struct Box enviornment[20];
char levels[20][20] = { "level_Arena.txt", "level2.txt"};
struct Player player1, player2;
struct Arrow arrow1, arrow2;
struct Knife knife1, knife2;
struct Lazer lazer1, lazer2;
struct Controllers controller1, controller2;
void TestMoveCamera()
{
	if (CP_Input_KeyDown(KEY_I))
	{
		globalYOffset -= 7;
	}
	if (CP_Input_KeyDown(KEY_J))
	{
		globalXOffset -= 7;
	}
	if (CP_Input_KeyDown(KEY_L))
	{
		globalXOffset += 7;
	}
	if (CP_Input_KeyDown(KEY_K))
	{
		globalYOffset += 7;
	}
}
void ApplyTransformations()
{
	for (int i = 0; i < 20; i++)
	{
		enviornment[i].topLeftCorner.y = enviornment[i].globalTopLeftCorner.y + globalYOffset;
		enviornment[i].topLeftCorner.x = enviornment[i].globalTopLeftCorner.x + globalXOffset;

		enviornment[i].bottomRightCorner.y = enviornment[i].globalBottomRightCorner.y + globalYOffset;
		enviornment[i].bottomRightCorner.x = enviornment[i].globalBottomRightCorner.x + globalXOffset;
	}
}

void DrawDebugText()
{
	CP_Settings_Fill(CP_Color_Create(200, 200, 200, 255));
	CP_Settings_TextSize(20.0f);
	char buffer[500] = { 0 };
	sprintf_s(buffer, _countof(buffer), "X: %f, Y: %f     X Max: %f, X Min: %f   Y Max: %f, Y Min: %f", player1.playerX, player1.playerY, player1.maxLimitX, player1.minLimitX, player1.maxLimitY, player1.minLimitY);
	CP_Font_DrawText(buffer, 1, 15);
	sprintf_s(buffer, _countof(buffer), "Prev Distance: %f, Prev Y Distance %f, Prev Y Min %f", player1.previousDistance, player1.previousY, player1.previousYmin);
	CP_Font_DrawText(buffer, 1, 35);
}
void LoadLevelData(char levelName[20], struct Player* playerOne, struct Player* playerTwo)
{
	for (int i = 0; i < 20; i++)
	{

		enviornment[i].topLeftCorner.x = 0;
		enviornment[i].topLeftCorner.y = 0;
		enviornment[i].bottomRightCorner.y = 0;
		enviornment[i].bottomRightCorner.x = 0;
		enviornment[i].boxColor = CP_Color_Create(255, 255, 255, 255);
		enviornment[i].groundColor = CP_Color_Create(100, 100, 100, 255);
	}
	FILE* levelFile = fopen(levelName, "r");
	float tempNum = 0;
	if (levelFile == NULL)
	{
		return;
	}

	int numOfLoops = 0;
	int envNum = 0;
	int playerSpawnRead = 0;
	//read file data and add to enviornment array
	while (!feof(levelFile))
	{
		if (playerSpawnRead == 0)
		{
			fscanf(levelFile, "%f", &tempNum);
			if (numOfLoops == 0)
			{
				playerOne->playerX = tempNum;
				numOfLoops += 1;
			}
			else if (numOfLoops == 1)
			{
				playerOne->playerY = tempNum;
				numOfLoops += 1;
			}
			else if (numOfLoops == 2)
			{
				playerTwo->playerX = tempNum;
				numOfLoops += 1;
			}
			else if (numOfLoops == 3)
			{
				playerTwo->playerY = tempNum;
				numOfLoops += 1;
			}
			else if (numOfLoops == 4)
			{
				numOfLoops = 0;
				playerSpawnRead = 1;
			}
		}
		else
		{
			while (numOfLoops != 5)
			{
				fscanf(levelFile, "%f", &tempNum);
				if (numOfLoops == 0)
				{
					enviornment[envNum].topLeftCorner.x = tempNum;
					numOfLoops += 1;
				}
				else if (numOfLoops == 1)
				{
					enviornment[envNum].topLeftCorner.y = tempNum;
					numOfLoops += 1;
				}
				else if (numOfLoops == 2)
				{
					enviornment[envNum].bottomRightCorner.x = tempNum;
					numOfLoops += 1;
				}
				else if (numOfLoops == 3)
				{
					enviornment[envNum].bottomRightCorner.y = tempNum;
					numOfLoops += 1;
				}
				else if (numOfLoops == 4)
				{
					envNum += 1;
					numOfLoops = 5;
				}
			}
			enviornment[envNum].boxColor = CP_Color_Create(255, 255, 255, 255);
			numOfLoops = 0;
		}
	}

	fclose(levelFile);
}

void gamestate_gameplay_init(void)
{
	initializeControllers(&controller1);
	initializeControllers(&controller1);
	ResetProjectile(&arrow1);
	ResetProjectile(&arrow2);
	srand(1);
	bgColor = CP_Color_Create(160, 160, 250, 255);
	//reset deaths
	player1.deaths = 0;
	player1.isDead = 0;
	player2.deaths = 0;
	player2.isDead = 0;

	//set player colors
	player1.fillColor = CP_Color_Create(120, 120, 255, 255);
	player1.lineColor = CP_Color_Create(60, 60, 200, 255);
	player2.lineColor = CP_Color_Create(255, 120, 0, 255);
	player2.fillColor = CP_Color_Create(255, 180, 100, 255);
	//init player kebinds
	player1.left = KEY_A;
	player1.right = KEY_D;
	player1.jump = KEY_W;
	player1.attack = KEY_E;
	
	player2.left = KEY_LEFT;
	player2.right = KEY_RIGHT;
	player2.jump = KEY_UP;
	player2.attack = KEY_DOWN;

	InitializePlayer(&player1);
	InitializePlayer(&player2);
	globalXOffset = 0;
	globalYOffset = 0;
	CP_Settings_RectMode(CP_POSITION_CENTER);
	RandomizeLevelAndPowerup(levels, &player1, &player2, &arrow1, &arrow2);
}

void gamestate_gameplay_update(void)
{
	player1.currentPowerup = 3;
	DrawEnviornment(bgColor);
	Physics(&player2, enviornment);
	Physics(&player1, enviornment);
	ProjectilePhysics(&arrow1);
	PlayerInput(&player1, &arrow1, &knife1, &lazer1, &controller1);
	PlayerInput(&player2, &arrow2, &knife2, &lazer2, &controller2);
	DrawPlayer(player2);
	DrawPlayer(player1);
	drawKnife(&player1, &knife1);
	drawKnife(&player2, &knife2);
	drawLazer(&player1, &lazer1);
	drawLazer(&player2, &lazer2);
	DisplayWins(&player1, &player2);
	LevelManager(levels, &player1, &player2, &arrow1, &arrow2);
	
	if (IsColliding(player1.playerX - 14, player1.playerY - 14, player1.playerX + 14, player1.playerY + 14,
		knife2.knifeX - knife2.knifeOffset, knife2.knifeY - knife2.knifeOffset, knife2.knifeX + knife2.knifeOffset, knife2.knifeY + knife2.knifeOffset) == 1 && knife2.isAnimating == 1)
	{
		player1.isDead = 1;
	}

	if (IsColliding(player2.playerX - 14, player2.playerY - 14, player2.playerX + 14, player2.playerY + 14,
		knife1.knifeX - 35, knife1.knifeY - 35, knife1.knifeX + 35, knife1.knifeY + 35) == 1 && knife1.isAnimating == 1)
	{
		player2.isDead = 1;
	}

	// lazer coll
	if (IsColliding(player1.playerX - 14, player1.playerY - 14, player1.playerX + 14, player1.playerY + 14,
		lazer2.lazerX - 5, lazer2.lazerY - 5, lazer2.lazerX + 5, lazer2.lazerY + 5))
	{
		player1.isDead = 1;
	}

	if (IsColliding(player2.playerX - 14, player2.playerY - 14, player2.playerX + 14, player2.playerY + 14,
		lazer1.lazerX - 5, lazer1.lazerY - 5, lazer1.lazerX + 5, lazer1.lazerY + 5))
	{
		player2.isDead = 1;
	}

	//DrawDebugText();
	if (CP_Input_KeyDown(KEY_F))
	{
		player1.isDead = 1;
	}
	if (CP_Input_KeyDown(KEY_G))
	{
		player2.isDead = 1;
	}
	if (CP_Input_KeyTriggered(KEY_R))
	{
		CP_Engine_SetNextGameStateForced(gamestate_gameplay_init, gamestate_gameplay_update, gamestate_gameplay_exit);
	}

}

void gamestate_gameplay_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
}