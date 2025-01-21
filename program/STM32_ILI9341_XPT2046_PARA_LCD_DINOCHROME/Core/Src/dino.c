/*
 * dino.c
 *
 *  Created on: Jan 17, 2025
 *      Author: Maksymilian
 */
#include "delay.h"
#include "ILI9341_paradriver.h"


extern uint8_t refresh;
extern uint8_t obs_refresh;

uint16_t jump_step=0;
uint16_t jump=0;
uint16_t leg_step=0;

int8_t velocity=0;

// figures definitions
void DrawDino(uint16_t altitude,uint8_t left_leg, uint8_t right_leg);	// dino
void DrawObstacle1(uint16_t shift);		// single BIG
void DrawObstacle2(uint16_t shift);		// single small
void DrawObstacle3(uint16_t shift);		// BIG & small



void DinoAnimation(void)
{
	if((TOUCH==0) || (jump_step != 0))	// jumping
	{
		if(refresh == 1)
		{

			switch(jump_step)
			{
			case 0:
			    velocity = 3;	//30
			    break;
			case 30:
			    velocity = 2;	//14
			    break;
			case 44:
			    velocity = 1;	//7
			    break;
			case 51:
			    velocity = 0;	//3
			    break;
			case 54:
			    velocity = -1;	//7
			    break;
			case 61:
			    velocity = -2;	//14
			    break;
			case 75:
			    velocity = -3;	//30
			    break;
			default:
			    break;
			}

			jump_step++;

			if(jump_step>104)
			{
				jump_step = 0;
			}

			jump = jump + velocity;

			DrawDino(jump,0,0);
			//dino_drawed = 1;
		}


	}
	else	// walking with leg animation
	{
		if(refresh == 1)
		{
			leg_step++;

			if(leg_step>60)		//delay between steps
			{
				leg_step = 0;
			}

			if(leg_step < 30)
			{
				DrawDino(0,0,3);
				//dino_drawed = 1;
			}

			else if(leg_step >= 30)
			{
				//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
				DrawDino(0,3,0);
				//dino_drawed = 1;
			}

		}
	}
}


uint16_t obs_step=0;	// obstacle step
uint16_t obs_pos=0;			// obstacle position
uint8_t obs_acc_temp=0;	// obstacle temporary accelerate

void ObstacleAnimation(uint8_t obs_acc)		// obs_acc - obstacle velocity (max 7)
{
	if(obs_refresh==0)
	{
		obs_step++;

		switch(obs_acc)		// calculate obstacle speed
		{
			case 1:
				obs_acc_temp = 1;
				break;
			case 2:
				if(obs_step%2 == 0) obs_acc_temp = 1; else obs_acc_temp = 2;
				break;
			case 3:
				obs_acc_temp = 2;
				break;
			case 4:
				if(obs_step%2 == 0) obs_acc_temp = 2; else obs_acc_temp = 3;
				break;
			case 5:
				obs_acc_temp = 3;
				break;
			case 6:
				if(obs_step%2 == 0) obs_acc_temp = 3; else obs_acc_temp = 4;
				break;
			case 7:
				obs_acc_temp = 4;
				break;
			default:
				obs_acc_temp = 0;
				break;
		}

		obs_pos = obs_pos + obs_acc;

		if(obs_pos>410)
		{
			obs_step = 0;
			obs_pos = 0;
		}

		DrawObstacle3(obs_pos);
		//obs_drawed = 1;
	}


}

uint16_t time=0;
uint8_t spd=1;

void GAME(void)
{


	if((refresh==0)) //&& (dino_drawed == 1)) //&& (obs_drawed == 1))
	{
/*		dino_drawed = 0;
		obs_drawed = 0;*/

		POINT_COLOR=GRAY;
		LCD_DrawLine(0,190,320,190);	// draw ground


		time++;
		//spd = 1;

		if(time == 400)
		{
			spd = 2;
		}
		else if(time == 800)
		{
			spd = 3;
		}
		else if(time == 1200)
		{
			spd = 4;
		}
		else if(time == 1600)
		{
			spd = 5;
		}
		else if(time == 2000)
		{
			spd = 6;
		}
		else if(time == 2400)
		{
			spd = 7;
		}

		if(time>2800)
		{
			time = 0;
		}
	}

	DinoAnimation();


	ObstacleAnimation(spd);
}
