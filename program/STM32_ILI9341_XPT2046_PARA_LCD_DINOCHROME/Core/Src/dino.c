/*
 * dino.c
 *
 *  Created on: Jan 17, 2025
 *      Author: Maksymilian
 */
#include "delay.h"
#include "ILI9341_paradriver.h"

// #TESTING
// HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

// REFRESHING
extern uint8_t refresh;			// dino, ground & game pending
extern uint8_t obs_refresh; 	// obstacle


// ANIMATIONS
// -> dino jumping:
uint16_t jump_step=0;	// step control for dino jumping
uint16_t jump_pos=0;	// dino position on screen while jumping
int8_t velocity=0;		// velocity of dino while jumping (4 max)

// -> dino walking
uint8_t walk_step=0;	// step control for walking animation

// -> obstacle moving
uint16_t obs_step=0;		// step control for obstacle moving
uint16_t obs_pos=0;			// obstacle position on screen while moving
uint8_t obs_acc_temp=0;		// obstacle temporary accelerate
uint8_t obstacle_type=0;	// type of the obstacle (1, 2 or 3)

// -> game pending
uint16_t time=0;	// time reference
uint8_t spd=1;		// obstacle actual speed


// FIGURES DEFINITIONS
void DrawDino(uint16_t altitude,uint8_t left_leg, uint8_t right_leg);	// dino
void DrawObstacle1(uint16_t shift);		// single BIG
void DrawObstacle2(uint16_t shift);		// single small
void DrawObstacle3(uint16_t shift);		// BIG & small


// DETECTOR DEFINITONS
void DinoDetector(uint16_t altitude);
void ObsDetector(uint16_t shift);
uint8_t CollisionDetector(void);


// RANDOMIZER DEFINITIONS
uint32_t RandomNumbers(void);



// ------------------------------------------------------------------------------------
// ----------------------------------- ANIMATIONS -------------------------------------
// ------------------------------------------------------------------------------------
void DinoAnimation(void)
{
	if((TOUCH==0) || (jump_step != 0))	// jumping
	{
		if(refresh == 1)
		{
			switch(jump_step)
			{
				case 0:
					velocity = 4;	//30
					break;
				case 30:
					velocity = 2;	//10
					break;
				case 40:
					velocity = 1;	//3
					break;
				case 43:
					velocity = 0;	//3
					break;
				case 46:
					velocity = -1;	//3
					break;
				case 49:
					velocity = -2;	//10
					break;
				case 59:
					velocity = -4;	//30
					break;
				default:
					break;
			}

			jump_step++;

			if(jump_step>88)	// in fact "jumping resolution"
			{
				jump_step = 0;	// get back on base position
			}

			jump_pos = jump_pos + velocity;		// calculate position while jumping

			DrawDino(jump_pos,0,0);
			DinoDetector(jump_pos);
		}
	}

	else	// walking with leg animation
	{
		if(refresh == 1)
		{
			walk_step++;

			if(walk_step>40)		//delay between steps
			{
				walk_step = 0;
			}

			if(walk_step < 20)
			{
				DrawDino(0,0,3);
			}

			else if(walk_step >= 20)
			{
				DrawDino(0,3,0);
			}

		}
	}
}


/*void ObstacleRandomizer(void)
{
	if(RandomNumbers() < 1500)
	{
		obstacle_type = 1;
	}
	else if(RandomNumbers() < 2500)
	{
		obstacle_type = 2;
	}
	else
	{
		obstacle_type = 3;
	}
}*/


void ObstacleAnimation(uint8_t obs_type, uint8_t obs_acc)		// obs_acc - obstacle velocity (max 7)
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

		obs_pos = obs_pos + obs_acc;	// calculate obstacle position

		if(obs_pos>410)		// do not go too far..
		{
			obs_step = 0;	// reset step
			obs_pos = 0;	// place obstacle on base position - out of screen (TYPE OF OBSTACLE CAN BE CHANGED NOW)
		}

		switch(obs_type)	// which obstacle to draw
		{
			case 1:
				DrawObstacle1(obs_pos);
				obstacle_type = 1;
				ObsDetector(obs_pos);
				break;
			case 2:
				DrawObstacle2(obs_pos);
				obstacle_type = 2;
				ObsDetector(obs_pos);
				break;
			case 3:
				DrawObstacle3(obs_pos);
				obstacle_type = 3;
				ObsDetector(obs_pos);
				break;
			default:
				break;
		}
	}
}



// ------------------------------------------------------------------------------------
// ---------------------------------- GAME PENDING ------------------------------------
// ------------------------------------------------------------------------------------
void GAME(void)
{
	if(CollisionDetector() == 0)
	{
		if(refresh==0)
		{
			POINT_COLOR=GRAY;
			LCD_DrawLine(0,190,320,190);	// draw ground


			time++;

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
		ObstacleAnimation(3,3);
	}
	else if(CollisionDetector() == 1)
	{

			DinoAnimation();
			POINT_COLOR=RED;
			LCD_DrawLine(0,190,320,190);	// DO SOMETHING AT THE END
			while(1);	// game over

	}
	else if(CollisionDetector() == 2)		// game pending for a moment (get closer to obstacle, but over the game anyway)
	{
		if(refresh==0)
		{
			POINT_COLOR=GRAY;
			LCD_DrawLine(0,190,320,190);

		}

		DinoAnimation();
		ObstacleAnimation(3,3);
	}


}
