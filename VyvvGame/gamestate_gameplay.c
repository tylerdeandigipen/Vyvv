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
struct Player player1, player2;
struct Arrow arrow1, arrow2;
struct Knife knife1, knife2;


float moveSpeed = 7;
void PlayerInput(void)
{
	if (CP_Input_KeyDown(KEY_W) && player1.hasJump == 1)
	{
		player1.playerVelocity.y = -25;
		player1.hasJump = 0;
	}
	if (CP_Input_KeyDown(KEY_A))
	{
		player1.playerVelocity.x = -moveSpeed;
		player1.facingDirection = -1;
	}
	if (CP_Input_KeyDown(KEY_D))
	{
		player1.playerVelocity.x = moveSpeed;
		player1.facingDirection = 1;
	}
	if (CP_Input_KeyReleased(KEY_D) || CP_Input_KeyReleased(KEY_A))
	{
		player1.playerVelocity.x = 0;
	}
	if (CP_Input_KeyDown(KEY_Q))
	{
		SpawnProjectile(&player1, &arrow1);
	}
}
void PlayerInput2(void)
{
	if (CP_Input_KeyDown(KEY_UP) && player2.hasJump == 1)
	{
		player2.playerVelocity.y = -25;
		player2.hasJump = 0;
	}
	if (CP_Input_KeyDown(KEY_LEFT))
	{
		player2.playerVelocity.x = -moveSpeed;
		player2.facingDirection = 1;

	}
	if (CP_Input_KeyDown(KEY_RIGHT))
	{
		player2.playerVelocity.x = moveSpeed;
		player2.facingDirection = -1;
	}
	if (CP_Input_KeyReleased(KEY_RIGHT) || CP_Input_KeyReleased(KEY_LEFT))
	{
		player2.playerVelocity.x = 0;
	}

}

void TestMoveCamera()
{
	if (CP_Input_KeyDown(KEY_I))
	{
		globalYOffset -= moveSpeed;
	}
	if (CP_Input_KeyDown(KEY_J))
	{
		globalXOffset -= moveSpeed;
	}
	if (CP_Input_KeyDown(KEY_L))
	{
		globalXOffset += moveSpeed;
	}
	if (CP_Input_KeyDown(KEY_K))
	{
		globalYOffset += moveSpeed;
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
void DrawEnviornment()
{
	CP_Graphics_ClearBackground(bgColor);
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Fill(CP_Color_Create(40, 40, 40, 255));
	CP_Settings_Stroke(CP_Color_Create(60, 60, 60, 255));
	CP_Settings_StrokeWeight(20.0f);
	CP_Graphics_DrawRect(0, 0, 800, 800);
	CP_Settings_RectMode(CP_POSITION_CENTER);
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

float knifeOffset = 10;
float knifeTimer = 0;
int isAnimating = 0;
float powerUpRadius = 30.0f;

BOOLEAN knifePickedUp1 = FALSE;
BOOLEAN knifePickedUp2 = FALSE;
BOOLEAN knifePickupSpawn = TRUE;

void drawKnife(struct Player* player, struct Knife* knife)
{

	knife->knifeX = player->playerX;
	knife->knifeY = player->playerY;

	CP_Settings_NoStroke();
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));

	if (CP_Input_KeyTriggered(KEY_E))
	{
		isAnimating = 1;
	}

	if (isAnimating == 1)
	{
		knifeTimer += CP_System_GetDt();
		knifeOffset = CP_Math_LerpFloat(knifeOffset, 23, knifeTimer);

		if (player->facingDirection == 1)
		{
			CP_Graphics_DrawRectAdvanced((knife->knifeX + knifeOffset), knife->knifeY, 5, 25, 90, 0);
		}
		else if (player->facingDirection == -1)
		{
			CP_Graphics_DrawRectAdvanced((knife->knifeX - knifeOffset), knife->knifeY, 5, 25, 90, 0);
		}

		if (knifeTimer > 0.3)
		{
			isAnimating = 0;
			knifeTimer = 0;
			knifeOffset = 10;
		}
	}
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
	//generateBoxes();
	InitializePlayer(&player1);
	InitializePlayer(&player2);
	player1.fillColor = CP_Color_Create(120, 120, 255, 255);
	player1.lineColor = CP_Color_Create(60, 60, 200, 255);
	player2.lineColor = CP_Color_Create(255, 120, 0, 255);
	player2.fillColor = CP_Color_Create(255, 180, 100, 255);
	globalXOffset = 0;
	globalYOffset = 0;
	srand(1);
	bgColor = CP_Color_Create(160, 160, 250, 255);
	CP_Settings_RectMode(CP_POSITION_CENTER);
	LoadLevelData("level_Arena.txt", &player1, &player2);
}

void gamestate_gameplay_update(void)
{
	//TestMoveCamera();
	//ApplyTransformations();
	DrawEnviornment();
	Physics(&player2, enviornment);
	Physics(&player1, enviornment);
	ProjectilePhysics(&arrow1);
	PlayerInput();
	PlayerInput2();
	DrawPlayer(player2);
	DrawPlayer(player1);
	//drawKnife(&player1, &knife1);
	//drawKnife(&player2, &knife2);

	//DrawDebugText();
	if (CP_Input_KeyTriggered(KEY_R))
	{
		CP_Engine_SetNextGameStateForced(gamestate_gameplay_init, gamestate_gameplay_update, gamestate_gameplay_exit);
	}
	if (CP_Input_KeyTriggered(KEY_F))
	{
		LoadLevelData("level2.txt", &player1, &player2);
	}
}

void gamestate_gameplay_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
}