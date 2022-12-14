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
// Copyright ? 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "gamestate_gameplay.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "gamestate_credits.h"
CP_Color bgColor;
CP_Color groundColor;
float globalXOffset = 0;
float globalYOffset = 0;


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
	int useCustomColor;
};
struct Player
{ 
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
};
struct Arrow
{
	CP_Vector position;
	CP_Vector velocity;
	float gravity;
};
struct Box enviornment[20];
struct Player player1, player2;

void PlayerCollisions(struct Player *player)
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
				CP_Settings_Stroke(groundColor);
				CP_Settings_Fill(groundColor);
			}
			CP_Settings_StrokeWeight(3.0f);
			CP_Graphics_DrawRect(enviornment[i].topLeftCorner.x + 15, enviornment[i].topLeftCorner.y + 15, enviornment[i].bottomRightCorner.x - enviornment[i].topLeftCorner.x - 30, enviornment[i].bottomRightCorner.y - enviornment[i].topLeftCorner.y - 15); // add 15 to for right edges
			CP_Settings_RectMode(CP_POSITION_CENTER);
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
	    if (player->playerX <= enviornment[player->closestBox].topLeftCorner.x  && player->playerY >= enviornment[player->closestBox].topLeftCorner.y) // if player is not ontop of box and to left
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
}

struct Box TestBox, TestBox2, TestBox3, TestBox4, TestBox5, TestBox6, TestBox7, TestBox8;

void generateBoxes(void)
{
	//make janky box to test if thing works
	TestBox.globalTopLeftCorner.x = 300;
	TestBox.globalTopLeftCorner.y = 525;
	TestBox.globalBottomRightCorner.y = 700;
	TestBox.globalBottomRightCorner.x = 500;
	TestBox.boxColor = CP_Color_Create(255, 255, 255, 255);

	TestBox2.globalTopLeftCorner.x = -30;
	TestBox2.globalTopLeftCorner.y = 550;
	TestBox2.globalBottomRightCorner.y = 700;
	TestBox2.globalBottomRightCorner.x = 200;
	TestBox2.boxColor = CP_Color_Create(255, 255, 255, 255);

	TestBox3.globalTopLeftCorner.x = 700;
	TestBox3.globalTopLeftCorner.y = 515;
	TestBox3.globalBottomRightCorner.y = 700;
	TestBox3.globalBottomRightCorner.x = 900;
	TestBox3.boxColor = CP_Color_Create(255, 255, 255, 255);

	TestBox4.globalTopLeftCorner.x = 620;
	TestBox4.globalTopLeftCorner.y = 545;
	TestBox4.globalBottomRightCorner.y = 700;
	TestBox4.globalBottomRightCorner.x = 900;
	TestBox4.boxColor = CP_Color_Create(255, 255, 255, 255);

	TestBox5.globalTopLeftCorner.x = 350;
	TestBox5.globalTopLeftCorner.y = 490;
	TestBox5.globalBottomRightCorner.y = 700;
	TestBox5.globalBottomRightCorner.x = 475;
	TestBox5.boxColor = CP_Color_Create(255, 255, 255, 255);

	TestBox6.globalTopLeftCorner.x = 520;
	TestBox6.globalTopLeftCorner.y = 490;
	TestBox6.globalBottomRightCorner.y = 530;
	TestBox6.globalBottomRightCorner.x = 600;
	TestBox6.boxColor = CP_Color_Create(255, 255, 255, 255);

	TestBox7.globalTopLeftCorner.x = 575;
	TestBox7.globalTopLeftCorner.y = 450;
	TestBox7.globalBottomRightCorner.y = 490;
	TestBox7.globalBottomRightCorner.x = 675;
	TestBox7.boxColor = CP_Color_Create(255, 255, 255, 255);

	TestBox8.globalTopLeftCorner.x = -30;
	TestBox8.globalTopLeftCorner.y = 580;
	TestBox8.globalBottomRightCorner.y = 700;
	TestBox8.globalBottomRightCorner.x = 800;
	TestBox8.boxColor = CP_Color_Create(255, 255, 255, 255);

	enviornment[0] = TestBox;
	enviornment[1] = TestBox2;
	enviornment[2] = TestBox3;
	enviornment[3] = TestBox4;
	enviornment[4] = TestBox5;
	enviornment[5] = TestBox6;
	enviornment[6] = TestBox7;
	enviornment[7] = TestBox8;
}

void InitializePlayer(struct Player *player)
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
}

struct Arrow arrow1, arrow2;
struct Knife knife1, knife2;

void SpawnProjectile(struct Player* player, struct Arrow* arrow)
{
	arrow->position.x = player->playerX;
	arrow->position.y = player->playerY;
	arrow->velocity.x = 20 * player->facingDirection;
	arrow->velocity.y = -15;
	arrow->gravity = 2;

}

void ProjectilePhysics(struct Arrow* arrow)
{
	arrow->velocity.y += arrow->gravity;
	arrow->position.y += arrow->velocity.y;
	arrow->position.x += arrow->velocity.x;
	CP_Graphics_DrawRect(arrow->position.x, arrow->position.y, 10, 10);
}

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

void DrawPlayer(struct Player player)
{
	CP_Settings_Fill(player.fillColor);
	CP_Settings_Stroke(player.lineColor);
	CP_Settings_StrokeWeight(5.0f);
	CP_Graphics_DrawRect(player.playerX, player.playerY, 20, 20);
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

void Physics(struct Player *player)
{
	if (player->currentWorldOffset.y != globalXOffset || player->currentWorldOffset.x != globalXOffset)
	{
		player->playerX += globalXOffset - player->currentWorldOffset.x;
		player->playerY += globalYOffset - player->currentWorldOffset.y;
		player->currentWorldOffset.y = globalYOffset;
		player->currentWorldOffset.x = globalXOffset;
	}
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
	PlayerCollisions(player);
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

float knifeOffset = 10;
float knifeTimer = 0;
int isAnimating = 0;
float powerUpRadius = 30.0f;

BOOLEAN knifePickedUp1 = FALSE;
BOOLEAN knifePickedUp2 = FALSE;
BOOLEAN knifePickupSpawn = TRUE;

void drawTestKnifePowerUp(struct Player* player, struct Knife* knife)
{
	knife->knifeXSpawn = 560;
	knife->knifeYSpawn = 400;
	knife->knifeX2Spawn = 100;
	knife->knifeY2Spawn = 450;

	if (knifePickupSpawn == TRUE)
	{
		CP_Graphics_DrawCircle(knife->knifeXSpawn, knife->knifeYSpawn, 30);
		CP_Graphics_DrawCircle(knife->knifeX2Spawn, knife->knifeY2Spawn, 30);

		// broad phase collision detection
		if (powerUpRadius > sqrt((player1.playerX - knife->knifeXSpawn) * (player1.playerX - knife->knifeXSpawn)) + (player1.playerY - knife->knifeYSpawn) * (player1.playerY - knife->knifeYSpawn))
		{
			knifePickedUp1 = TRUE;
			knifePickupSpawn = FALSE;
		}

		if (powerUpRadius > sqrt((player2.playerX - knife->knifeX2Spawn) * (player2.playerX - knife->knifeX2Spawn)) + (player2.playerY - knife->knifeY2Spawn) * (player2.playerY - knife->knifeY2Spawn))
		{
			knifePickedUp2 = TRUE;
			knifePickupSpawn = FALSE;
		}
	}
}

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


void gamestate_gameplay_init(void)
{
	generateBoxes();
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
	groundColor = CP_Color_Create(100, 100, 100, 255);
	CP_Settings_RectMode(CP_POSITION_CENTER);
}

void gamestate_gameplay_update(void)
{
	TestMoveCamera();
	ApplyTransformations();
	DrawEnviornment();
	Physics(&player2);
	Physics(&player1);
	ProjectilePhysics(&arrow1);
	PlayerInput();
	PlayerInput2();
	DrawPlayer(player2);
	DrawPlayer(player1);
	drawTestKnifePowerUp(&player1, &knife1);
	drawTestKnifePowerUp(&player2, &knife2);
	if (knifePickedUp1 == TRUE)
	{
		drawKnife(&player1, &knife1);
	}

	if (knifePickedUp2 == TRUE)
	{
		drawKnife(&player2, &knife2);
	}

	DrawDebugText();
	if (CP_Input_KeyTriggered(KEY_R))
	{
		CP_Engine_SetNextGameStateForced(gamestate_gameplay_init, gamestate_gameplay_update, gamestate_gameplay_exit);
	}
}

void gamestate_gameplay_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
}