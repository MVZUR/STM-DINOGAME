/*
 * detector.c
 *
 *  Created on: Jan 22, 2025
 *      Author: Maksymilian
 */

#include "delay.h"
#include "ILI9341_paradriver.h"

extern uint8_t obstacle_type;	// which obstacle?

// dino coordinates (always the same)
static uint16_t dino_x1 = 7;
static uint16_t dino_y1 = 160;
static uint16_t dino_x2 = 45;
static uint16_t dino_y2 = 207;

// obstacle coordinates
static uint16_t obs_x1=0;
static uint16_t obs_y1=0;
static uint16_t obs_x2=0;
static uint16_t obs_y2=0;
static uint16_t obs_x1s=0;	// small one
static uint16_t obs_y1s=0;	// ..
static uint16_t obs_x2s=0;	// ..
static uint16_t obs_y2s=0;	// ..



void DinoDetector(uint16_t altitude)
{
	// include jumping
	dino_y1 = 160 - altitude;
	dino_y2 = 207 - altitude;
}



void ObsDetector(uint16_t shift)
{
	if(obstacle_type == 1)
	{
		// include moving
		obs_x1 = 340 - shift;
		obs_y1 = 150;
		obs_x2 = 372 - shift;
		obs_y2 = 210;
	}

	else if(obstacle_type == 2)
	{
		// include moving
		obs_x1 = 344 - shift;
		obs_y1 = 170;
		obs_x2 = 366 - shift;
		obs_y2 = 210;
	}

	else if(obstacle_type == 3)
	{
		// include moving
		obs_x1 = 344 - shift;
		obs_y1 = 150;
		obs_x2 = 376 - shift;
		obs_y2 = 210;

		// second one
		obs_x1s = 379 - shift;
		obs_y1s = 170;
		obs_x2s = 401 - shift;
		obs_y2s = 210;
	}
}



uint8_t CollisionDetector(void)
{
	// check if coordinates do not overlap
	if(((obs_x1 <= dino_x2) && (obs_x2 >= dino_x1) && (dino_y2 >= obs_y1)) ||			// first obstacle
			((obs_x1s <= dino_x2) && (obs_x2s >= dino_x1) && (dino_y2 >= obs_y1s)) )	// second obstacle (if is)
	{
		// obstacle get into half of dino while jumping XD
		if( ((obs_x1 < dino_x2) && (obs_x1 > ((dino_x1+dino_x2)/2)) && (dino_y2 < ((obs_y1+obs_y2)/2))) ||			// first obstacle
				((obs_x1s < dino_x2) && (obs_x1s > ((dino_x1+dino_x2)/2)) && (dino_y2 < ((obs_y1s+obs_y2s)/2))) )	// second obstacle (if is)
		{
			return 2;	// wait a moment
		}

		else return 1;	// collision detected GAME OVER
	}

	else return 0;	// no colision WP
}

