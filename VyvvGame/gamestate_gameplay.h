//---------------------------------------------------------
// file:	gamestate_gameplay.h
// author:	tyler dean
// email:	tyler.dean@digipen.edu
//
// brief:	gameplay helper file
//
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#pragma warning(disable:4996)
#pragma once
struct Knife
{
	float knifeX;
	float knifeY;
	float knifeXSpawn;
	float knifeYSpawn;
	float knifeX2Spawn;
	float knifeY2Spawn;
};
struct Box
{
	CP_Vector bottomRightCorner;
	CP_Vector topLeftCorner;
	CP_Vector globalBottomRightCorner;
	CP_Vector globalTopLeftCorner;
	CP_Color boxColor;
	CP_Color groundColor;
	int useCustomColor;
};
struct Player
{
	int isDead;
	int deaths;
	float playerX;
	float playerY;
	CP_Vector playerVelocity;
	float maxLimitY;//floor
	float maxLimitX;//right walls
	float minLimitY;//left walls
	float minLimitX;//ceiling
	int higherGround;
	int resetXMax;
	float previousDistance;
	float previousDistanceY;
	double previousYmin;
	int closestBox;
	double previousY;
	double previousCeiling;
	int resetYMax;
	int resetYMin;
	int hasJump;
	float gravity;
	CP_Vector currentWorldOffset;
	CP_Color lineColor;
	CP_Color fillColor;
	float facingDirection;
	int currentPowerup;

	//keybinds
	CP_KEY left;
	CP_KEY right;
	CP_KEY jump;
	CP_KEY attack;
};
struct Arrow
{
	CP_Vector position;
	CP_Vector velocity;
	float gravity;
};
void gamestate_gameplay_init(void);
void gamestate_gameplay_update(void);
void gamestate_gameplay_exit(void);
void LoadLevelData(char levelName[20], struct Player* playerOne, struct Player* playerTwo);
inline void PlayerCollisions(struct Player* player, struct Box enviornment[20])
{
	player->previousDistance = 1000;
	player->previousCeiling = 1000;
	player->previousY = 1000;
	player->previousYmin = 1000;
	player->resetYMin = 1;
	player->resetYMax = 1;
	player->playerY = CP_Math_ClampFloat(player->playerY, player->minLimitY, player->maxLimitY);
	player->playerX = CP_Math_ClampFloat(player->playerX, player->minLimitX, player->maxLimitX);
	//prevent indesive collsions when two boxes share are on the same plane, by seeing which one is closer
	for (int i = 0; i < 20; i++)
	{
		if (enviornment[i].topLeftCorner.y != 0)
		{
			if (fabs(player->playerX - enviornment[i].topLeftCorner.x) <= player->previousDistance || fabs(player->playerX - enviornment[i].bottomRightCorner.x) <= player->previousDistance) //check for what box is the closest
			{
				if (player->playerY >= enviornment[i].topLeftCorner.y && player->playerY <= enviornment[i].bottomRightCorner.y)//check if player is on same y as vertical walls
				{
					if (fabs(player->playerX - enviornment[i].topLeftCorner.x) < fabs(player->playerX - enviornment[i].bottomRightCorner.x))
						player->previousDistance = (float)fabs(player->playerX - enviornment[i].topLeftCorner.x);
					else
						player->previousDistance = (float)fabs(player->playerX - enviornment[i].bottomRightCorner.x);

					player->closestBox = i;
				}
			}
			if (player->playerX > enviornment[i].topLeftCorner.x && player->playerX < enviornment[i].bottomRightCorner.x && player->playerY <= enviornment[i].topLeftCorner.y) //if player is above box
			{
				if (fabs(player->playerY - enviornment[i].topLeftCorner.y) <= player->previousY)
				{
					player->maxLimitY = enviornment[i].topLeftCorner.y;
					player->resetYMax = 0;
					player->previousY = fabs(player->playerY - enviornment[i].topLeftCorner.y);
				}
			}
			if (player->playerX > enviornment[i].topLeftCorner.x + 1 && player->playerX < enviornment[i].bottomRightCorner.x - 1 && player->playerY >= enviornment[i].bottomRightCorner.y) //if player is below box
			{
				if (fabs(player->playerY - enviornment[i].bottomRightCorner.y) <= player->previousYmin)
				{
					player->minLimitY = enviornment[i].bottomRightCorner.y + 15;
					player->resetYMin = 0;
					player->previousYmin = fabs(player->playerY - enviornment[i].bottomRightCorner.y);
				}
			}
			//draw box
			CP_Settings_RectMode(CP_POSITION_CORNER);
			if (enviornment[i].useCustomColor == 1)
			{
				CP_Settings_Stroke(enviornment[i].boxColor);
				CP_Settings_Fill(enviornment[i].boxColor);
			}
			else
			{
				CP_Settings_Stroke(enviornment[i].groundColor);
				CP_Settings_Fill(enviornment[i].groundColor);
			}
			CP_Settings_StrokeWeight(3.0f);
			CP_Graphics_DrawRect(enviornment[i].topLeftCorner.x + 23, enviornment[i].topLeftCorner.y + 23, enviornment[i].bottomRightCorner.x - enviornment[i].topLeftCorner.x - 27, enviornment[i].bottomRightCorner.y - enviornment[i].topLeftCorner.y); // add 15 to for right edges
			//error correction, if player is in box set position to nearest edge
			if (player->playerX > enviornment[i].topLeftCorner.x && player->playerX < enviornment[i].bottomRightCorner.x) // if player is inbetween both vertical edges
			{
				if (player->playerY < enviornment[i].bottomRightCorner.y && player->playerY > enviornment[i].topLeftCorner.y) // if player is inbetween both horizontal edges
				{
					if (fabs(player->playerY - enviornment[i].topLeftCorner.y) || fabs(player->playerY - enviornment[i].bottomRightCorner.y) > fabs(player->playerX - enviornment[i].topLeftCorner.x) || fabs(player->playerX - enviornment[i].bottomRightCorner.x))
					{
						if (fabs(player->playerY - enviornment[i].bottomRightCorner.y) > fabs(player->playerY - enviornment[i].topLeftCorner.y))
						{
							player->playerY = enviornment[i].topLeftCorner.y;
							player->maxLimitY = enviornment[i].topLeftCorner.y;
							player->previousY = fabs(player->playerY - enviornment[i].topLeftCorner.y);
							player->resetYMax = 0;
						}
						else
						{
							player->playerY = enviornment[i].bottomRightCorner.y + 15;
							player->minLimitY = enviornment[i].bottomRightCorner.y + 15;
							player->previousYmin = fabs(player->playerY - enviornment[i].bottomRightCorner.y);
							player->resetYMin = 0;
						}
					}
					else
					{
						if (fabs(player->playerX - enviornment[i].topLeftCorner.x) > fabs(player->playerX - enviornment[i].bottomRightCorner.x))
						{
							player->maxLimitX = enviornment[i].topLeftCorner.x;
							player->playerX = enviornment[i].topLeftCorner.x;
							player->previousDistance = (float)fabs(player->playerX - enviornment[i].topLeftCorner.x);
						}
						else
						{
							player->minLimitX = enviornment[i].bottomRightCorner.x;
							player->playerX = enviornment[i].bottomRightCorner.x;
							player->previousDistance = (float)fabs(player->playerX - enviornment[i].bottomRightCorner.x);
						}
					}
				}
			}
		}
	}
	player->maxLimitX = 788;
	player->minLimitX = 12;
	if (player->playerX <= enviornment[player->closestBox].topLeftCorner.x && player->playerX <= enviornment[player->closestBox].bottomRightCorner.x) // if player is on the left of the box
	{
		if (player->playerX <= enviornment[player->closestBox].topLeftCorner.x && player->playerY >= enviornment[player->closestBox].topLeftCorner.y) // if player is not ontop of box and to left
		{
			player->maxLimitX = enviornment[player->closestBox].topLeftCorner.x;
		}
	}
	else if (player->playerX >= enviornment[player->closestBox].topLeftCorner.x && player->playerX >= enviornment[player->closestBox].bottomRightCorner.x)// if the player is on the right of the box
	{
		if (player->playerX >= enviornment[player->closestBox].bottomRightCorner.x && player->playerY >= enviornment[player->closestBox].topLeftCorner.y) // if player is not ontop of box and to right 
		{
			player->minLimitX = enviornment[player->closestBox].bottomRightCorner.x;
		}
	}
	if (player->resetYMax == 1)
	{
		player->maxLimitY = 585;
	}
	if (player->resetYMin == 1)
	{
		player->minLimitY = 12;
	}
};
inline void InitializePlayer(struct Player* player)
{
	player->currentWorldOffset.x = 0;
	player->currentWorldOffset.y = 0;
	player->playerX = 400;
	player->playerY = 300;
	player->playerVelocity.y = 0;
	player->playerVelocity.x = 0;
	player->maxLimitY = 585;//floor
	player->maxLimitX = 788;//right walls
	player->minLimitY = 15;//left walls
	player->minLimitX = 20;//ceiling
	player->higherGround = 0;
	player->resetXMax = 0;
	player->previousDistance = 1000;
	player->previousDistanceY = 1000;
	player->closestBox = 0;
	player->previousY = 1000;
	player->previousCeiling = 1000;
	player->resetYMax = 1;
	player->resetYMin = 1;
	player->hasJump = 1;
	player->gravity = 0;
	player->currentPowerup = 0;
}
inline void SpawnProjectile(struct Player* player, struct Arrow* arrow)
{
	arrow->position.x = player->playerX;
	arrow->position.y = player->playerY;
	arrow->velocity.x = 20 * player->facingDirection + player->playerVelocity.x;
	arrow->velocity.y = -15;
	arrow->gravity = 2;
}
inline void ProjectilePhysics(struct Arrow* arrow)
{
	arrow->velocity.y += arrow->gravity;
	arrow->position.y += arrow->velocity.y;
	arrow->position.x += arrow->velocity.x;
	CP_Graphics_DrawRect(arrow->position.x, arrow->position.y, 10, 10);
}
inline void DrawPlayer(struct Player player)
{
	CP_Settings_Fill(player.fillColor);
	CP_Settings_Stroke(player.lineColor);
	CP_Settings_StrokeWeight(5.0f);
	CP_Graphics_DrawRect(player.playerX, player.playerY, 20, 20);
}
inline void Physics(struct Player* player, struct Box enviornment[20])
{
	/*
	*uncomment if cam movements are wanted
	if (player->currentWorldOffset.y != globalXOffset || player->currentWorldOffset.x != globalXOffset)
	{
		player->playerX += globalXOffset - player->currentWorldOffset.x;
		player->playerY += globalYOffset - player->currentWorldOffset.y;
		player->currentWorldOffset.y = globalYOffset;
		player->currentWorldOffset.x = globalXOffset;
	}
	*/
	player->playerX += player->playerVelocity.x;
	player->playerY += player->playerVelocity.y;

	//playerX = CP_Math_ClampFloat(playerX, 15, 795);
	if (player->playerY <= player->maxLimitY + 1)
	{
		player->hasJump = 0;
		player->gravity += 1.25f;
		player->playerVelocity.y += player->gravity;
		player->playerVelocity.y = CP_Math_ClampFloat(player->playerVelocity.y, -25, 25);
	}
	else
	{
		player->hasJump = 1;
		player->playerVelocity.y = 0;
		player->gravity = 0;
	}
	PlayerCollisions(player, enviornment);
}
inline void DrawEnviornment(CP_Color bgColor)
{
	CP_Graphics_ClearBackground(bgColor);
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Fill(CP_Color_Create(40, 40, 40, 255));
	CP_Settings_Stroke(CP_Color_Create(60, 60, 60, 255));
	CP_Settings_StrokeWeight(20.0f);
	CP_Graphics_DrawRect(0, 0, 800, 800);
	CP_Settings_RectMode(CP_POSITION_CENTER);
}
inline void Attack(struct Player* player, struct Arrow* arrow)
{
	switch (player->currentPowerup)
	{
		case 0:
			break;
		case 1:
			SpawnProjectile(player, arrow);
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
	}
}
inline void PlayerInput(struct Player* player, struct Arrow* arrow)
{
	float moveSpeed = 7;
	if (CP_Input_KeyDown(player->jump) && player->hasJump == 1)
	{
		player->playerVelocity.y = -25;
		player->hasJump = 0;
	}
	if (CP_Input_KeyDown(player->left))
	{
		player->playerVelocity.x = -moveSpeed;
		player->facingDirection = -1;
	}
	if (CP_Input_KeyDown(player->right))
	{
		player->playerVelocity.x = moveSpeed;
		player->facingDirection = 1;
	}
	if (CP_Input_KeyReleased(player->left) || CP_Input_KeyReleased(player->right))
	{
		player->playerVelocity.x = 0;
	}
	if (CP_Input_KeyDown(player->attack))
	{
		Attack(player, arrow);
	}
}
inline void RandomizeLevelAndPowerup(char levels[20][20], struct Player* player1, struct Player* player2)
{
	int currentLevelsImplemented = 2;
	int levelNum = rand() % currentLevelsImplemented;
	int powerUpNum = (rand() % 3) + 1;
	player1->currentPowerup = powerUpNum;
	player2->currentPowerup = powerUpNum;
	LoadLevelData(levels[levelNum], player1, player2);
}
inline void DisplayWins(struct Player* player1, struct Player* player2)
{
	CP_Settings_Fill(CP_Color_Create(200, 200, 200, 255));
	CP_Settings_TextSize(20.0f);
	char buffer[500] = { 0 };
	sprintf_s(buffer, _countof(buffer), "Blue Wins: %i", player2->deaths);
	CP_Font_DrawText(buffer, 1, 15);
	sprintf_s(buffer, _countof(buffer), "Orange Wins: %i", player1->deaths);
	CP_Font_DrawText(buffer, 1, 35);
};
inline void LevelManager(char levels[20][20], struct Player* player1, struct Player* player2)
{
	if (player1->isDead == 1 || player2->isDead == 1)
	{
		int winThreshold = 10;
		if (player1->isDead == 1)
		{
			player1->deaths += 1;
			player1->isDead = 0;
			if (player1->deaths >= winThreshold)
			{
				//player 2 wins
				CP_Engine_SetNextGameStateForced(gamestate_gameplay_init, gamestate_gameplay_update, gamestate_gameplay_exit);
			}
		}
		if (player2->isDead == 1)
		{
			player2->deaths += 1;
			player2->isDead = 0;
			if (player2->deaths >= winThreshold)
			{
				//player 1 wins
				CP_Engine_SetNextGameStateForced(gamestate_gameplay_init, gamestate_gameplay_update, gamestate_gameplay_exit);
			}
		}
		RandomizeLevelAndPowerup(levels, player1, player2);
	}
}