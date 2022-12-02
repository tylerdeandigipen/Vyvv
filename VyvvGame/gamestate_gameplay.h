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

struct Lazer
{
	int isShooting; 
	float lazerVelocity;
	float lazerY;
	float lazerX;
	float lazerRadius;

	int inMotion;
	// sorry tyler im stealing ur code heheheheh
	float maxLimitX; //right walls
	float minLimitX; //left walls
};
struct Knife
{
	float knifeX;
	float knifeY;

	float knifeOffset;
	float knifeTimer;
	int isAnimating;
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
	int buttonHeld;
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

	//controller bits
	int number;
	CP_Vector leftStick;
	float rightTrigger;
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

inline int IsColliding(float x1, float y1, float x2, float y2, float boxX1, float boxY1, float boxX2, float boxY2)
{
	if (boxX1 >= x2 || boxY1 >= y2 || boxX2 <= x1 || boxY2 <= y1)
	{
		// no overlap
		return 0;
	}
	else
	{
		// overlap
		return 1;
	}
}

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
			CP_Graphics_DrawRect(enviornment[i].topLeftCorner.x + 23, enviornment[i].topLeftCorner.y + 23, enviornment[i].bottomRightCorner.x - enviornment[i].topLeftCorner.x - 27, enviornment[i].bottomRightCorner.y - enviornment[i].topLeftCorner.y - 12); // add 15 to for right edges
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
	player->maxLimitX = 778;
	player->minLimitX = 2;
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
	player->maxLimitX = 775;//right walls
	player->minLimitY = 0;//left walls
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

/*inline void initializeControllers(struct Controllers* controller)
{
	player->leftStick = CP_Input_GamepadLeftStickAdvanced(0);
	controller->buttonA = GAMEPAD_A;
	controller->rightTrigger = ;
}*/

inline void initalizeLazer(struct Player* player, struct Lazer* lazer)
{
	lazer->inMotion = 0;
	lazer->lazerVelocity = 20 * player->facingDirection;
	lazer->lazerRadius = 40.0f;
	lazer->lazerX = player->playerX - 30;
	lazer->lazerY = player->playerY;
	lazer->maxLimitX = 900;
	lazer->minLimitX = -300;
}

inline void drawLazer(struct Player* player, struct Lazer* lazer)
{
	CP_Settings_NoStroke();
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));

	if (lazer->inMotion == 1)
	{
		lazer->lazerX += lazer->lazerVelocity;

		if (player->facingDirection == 1)
		{
			CP_Graphics_DrawCircle(lazer->lazerX, lazer->lazerY + 10, 7);
		}
		else if (player->facingDirection == -1)
		{
			CP_Graphics_DrawCircle(lazer->lazerX + 80, lazer->lazerY + 10, 7);
		}
	}

	if (lazer->lazerX >= lazer->maxLimitX)
	{
		lazer->inMotion = 0;
	}

	if (lazer->lazerX <= lazer->minLimitX)
	{
		lazer->inMotion = 0;
	}
}

inline void initalizeKnife(struct Knife* knife)
{
	knife->knifeOffset = 0;
	knife->knifeTimer = 0;
	knife->isAnimating = 10;
}

inline void drawKnife(struct Player* player, struct Knife* knife)
{
	knife->knifeX = player->playerX;
	knife->knifeY = player->playerY;

	CP_Settings_NoStroke();
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));

	if (knife->isAnimating == 1)
	{
		knife->knifeTimer += CP_System_GetDt();
		knife->knifeOffset = CP_Math_LerpFloat(knife->knifeOffset, 20, knife->knifeTimer);

		if (player->facingDirection == 1)
		{
			CP_Graphics_DrawRectAdvanced((knife->knifeX + knife->knifeOffset + 45), knife->knifeY + 7, 4.5, 33, 90, 0);
		}
		else if (player->facingDirection == -1)
		{
			CP_Graphics_DrawRectAdvanced((knife->knifeX - knife->knifeOffset + 10), knife->knifeY + 7, 4.5, 33, 90, 0);
		}

		if (knife->knifeTimer > 0.3)
		{
			knife->isAnimating = 0;
			knife->knifeTimer = 0;
			knife->knifeOffset = 0;
		}
	}
}
inline void ProjectilePhysics(struct Arrow* arrow)
{
	arrow->velocity.y += arrow->gravity;
	arrow->position.y += arrow->velocity.y;
	arrow->position.x += arrow->velocity.x;
	CP_Graphics_DrawRect(arrow->position.x, arrow->position.y, 10, 10);
}
inline void ResetProjectile(struct Arrow* arrow)
{
	//reset projectiles
	arrow->position.x = -1000; //send into space so is not seen
	arrow->position.y = -1000; // the arrow in da sky, #astronaut 
	arrow->velocity.x = 0;
	arrow->velocity.y = 0;
	arrow->gravity = 0;
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
	float gravity = .5f;
	player->playerX += player->playerVelocity.x;
	player->playerY += player->playerVelocity.y;

	//playerX = CP_Math_ClampFloat(playerX, 15, 795);
	if (player->playerY <= player->maxLimitY + 1)
	{
		player->hasJump = 0;
		player->gravity += gravity;
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

inline void Attack(struct Player* player, struct Arrow* arrow, struct Knife* knife, struct Lazer* lazer)
{
	switch (player->currentPowerup)
	{
		case 0:
			break;
		case 1:
			SpawnProjectile(player, arrow);
			break;
		case 2:
			knife->isAnimating = 1;
			break;
		case 3:
			initalizeLazer(player, lazer);
			lazer->inMotion = 1;
			break;
		case 4:
			break;
	}
}
inline void PlayerInput(struct Player* player, struct Arrow* arrow, struct Knife* knife, struct Lazer* lazer)
{
	float moveSpeed = 7;
	if (CP_Input_GamepadConnectedAdvanced(0) || CP_Input_GamepadConnectedAdvanced(1))
	{
		if (player->jump == 1 && player->hasJump == 1)
		{
			player->playerVelocity.y = -17;
			player->hasJump = 0;
		}
		if (player->leftStick.x < 0)
		{
			if (player->playerVelocity.x > -moveSpeed + 1)
			{
				player->playerVelocity.x += -moveSpeed / 3;
			}
			player->facingDirection = -1;
			player->buttonHeld = 1;
		}
		if (player->leftStick.x > 0)
		{
			if (player->playerVelocity.x < moveSpeed - 1)
			{
				player->playerVelocity.x += moveSpeed / 3;
			}
			player->facingDirection = 1;
			player->buttonHeld = 1;
		}
		if (player->leftStick.x == 0)
		{
			player->buttonHeld = 0;
		}
		if (player->hasJump == 1 && player->buttonHeld == 0)//make drag
		{
			player->playerVelocity.x = player->playerVelocity.x * .4f;
		}
		if (CP_Input_GamepadTriggeredAdvanced(player->attack, player->number))
		{
			Attack(player, arrow, knife, lazer);
		}
	}
	else
	{
		if ((CP_Input_KeyDown(player->jump) && player->hasJump == 1))
		{
			player->playerVelocity.y = -17;
			player->hasJump = 0;
		}
		if (CP_Input_KeyDown(player->left))
		{
			if (player->playerVelocity.x > -moveSpeed + 1)
			{
				player->playerVelocity.x += -moveSpeed / 3;
			}
			player->facingDirection = -1;
			player->buttonHeld = 1;
		}
		if (CP_Input_KeyDown(player->right))
		{
			if (player->playerVelocity.x < moveSpeed - 1)
			{
				player->playerVelocity.x += moveSpeed / 3;
			}
			player->facingDirection = 1;
			player->buttonHeld = 1;
		}
		if (CP_Input_KeyReleased(player->right) || CP_Input_KeyReleased(player->left))
		{
			player->buttonHeld = 0;
		}
		if (player->hasJump == 1 && player->buttonHeld == 0)//make drag
		{
			player->playerVelocity.x = player->playerVelocity.x * .4f;
		}
		if (CP_Input_KeyTriggered(player->attack))
		{
			Attack(player, arrow, knife, lazer);
		}
	}
}
inline void RandomizeLevelAndPowerup(char levels[20][20], struct Player* player1, struct Player* player2, struct Arrow* arrow1, struct Arrow* arrow2)
{
	ResetProjectile(arrow1);
	ResetProjectile(arrow2);
	int currentLevelsImplemented = 3;
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
inline void LevelManager(char levels[20][20], struct Player* player1, struct Player* player2, struct Arrow* arrow1, struct Arrow* arrow2)
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
		RandomizeLevelAndPowerup(levels, player1, player2, arrow1, arrow2);
	}
}