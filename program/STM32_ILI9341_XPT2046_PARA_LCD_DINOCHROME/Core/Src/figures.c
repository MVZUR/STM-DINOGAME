/*
 * figures.c
 *
 *  Created on: Jan 20, 2025
 *      Author: Maksymilian
 */

#include "delay.h"
#include "ILI9341_paradriver.h"

extern uint16_t jump_step;	// GOING UP < jump_step/2
							// GOING DOWN > jump_step/2

// -------------------------------------------------------------------------------------
// ------------------------------------- FIGURES ---------------------------------------
// -------------------------------------------------------------------------------------

// dino
void DrawDino(uint16_t altitude,uint8_t left_leg, uint8_t right_leg)
{
	// ----------- HEAD -----------
	LCD_Fill(27,160 - altitude,42,170 - altitude,GRAY);      // head
	LCD_Fill(25,162 - altitude,45,172 - altitude,GRAY);

	if(jump_step>=44 || (jump_step == 0))	// going down
	{
		LCD_Fill(25,158 - altitude,26,161 - altitude,WHITE);		// clear head top 1
		LCD_Fill(27,156 - altitude,42,159 - altitude,WHITE);		// clear head top 2
		LCD_Fill(43,158 - altitude,45,161 - altitude,WHITE);		// clear head top 3
	}
	if(jump_step <=44)	// going up
	{
		LCD_Fill(37,173 - altitude,45,174 - altitude,WHITE);		// clear head bottom
		LCD_Fill(42,173 - altitude,45,177 - altitude,WHITE);		// clear head bottom
	}



	// ----------- EYE -----------
	LCD_Fill(30,163 - altitude,32,166 - altitude,WHITE);     // eye



	// ----------- MOUTH -----------
	LCD_Fill(25,172 - altitude,36,175 - altitude,GRAY);      // mouth
	LCD_Fill(25,175 - altitude,41,176 - altitude,GRAY);      // mouth (tongue)

	if(jump_step>=44 || (jump_step == 0))	// going down
	{
		LCD_Fill(37,173 - altitude,45,174 - altitude,WHITE);		// clear mouth (tongue) top
	}

	if(jump_step <=44)	// going up
	{
		LCD_Fill(36,177 - altitude,41,180 - altitude,WHITE);      // clear mouth (tongue) bottom
	}



	// ----------- NECK & CHEST -----------
	LCD_Fill(25,176 - altitude,35,177 - altitude,GRAY);     // neck
	LCD_Fill(23,177 - altitude,35,192 - altitude,GRAY);     // neck & chest

	if(jump_step>=44 || (jump_step == 0))	// going down
	{
		LCD_Fill(23,173 - altitude,24,176 - altitude,WHITE);     // clear neck & chest top
	}
	if(jump_step <=44)	// going up
	{
		LCD_Fill(33,193 - altitude,35,196 - altitude,WHITE);    // clear neck & chest bottom
	}



	// ----------- ARM -----------
	LCD_Fill(35,181 - altitude,39,183 - altitude,GRAY);      // arm1
	LCD_Fill(37,181 - altitude,39,185 - altitude,GRAY);		 // arm2

	if(jump_step>=44 || (jump_step == 0))	// going down
	{
		LCD_Fill(36,177 - altitude,41,178 - altitude,WHITE);      // clear arm1 & arm2 top
	}

	if(jump_step <=44)	// going up
	{
		LCD_Fill(35,184 - altitude,36,187 - altitude,WHITE);      // clear arm1 bottom
		LCD_Fill(37,186 - altitude,39,189 - altitude,WHITE);      // clear arm2 bottom
	}



	// ----------- BODY -----------
	LCD_Fill(20,180 - altitude,32,195 - altitude,GRAY);     // body1
	LCD_Fill(17,182 - altitude,30,197 - altitude,GRAY);     // body2
	LCD_Fill(12,185 - altitude,27,197 - altitude,GRAY);     // body3

	if(jump_step>=44 || (jump_step == 0))	// going down
	{
		LCD_Fill(20,176 - altitude,22,179 - altitude,WHITE);    // clear body1 top
		LCD_Fill(17,178 - altitude,19,181 - altitude,WHITE);    // clear body2 top
		LCD_Fill(13,181 - altitude,16,184 - altitude,WHITE);    // clear body3 top (tail part)
	}
	if(jump_step <=44)	// going up
	{
		LCD_Fill(31,196 - altitude,32,199 - altitude,WHITE);    // clear body1 bottom
		LCD_Fill(28,198 - altitude,30,201 - altitude,WHITE);    // clear body2 bottom
		LCD_Fill(12,197 - altitude,14,200 - altitude,WHITE);    // clear body3 leg break 1 bottom
		LCD_Fill(20,197 - altitude,22,202 - altitude,WHITE);    // clear body3 leg break 2 bottom
	}



	// ----------- TAIL -----------
	LCD_Fill(10,182 - altitude,12,195 - altitude,GRAY);     // tail1
	LCD_Fill(8,180 - altitude,10,192 - altitude,GRAY);		// tail2
	LCD_Fill(7,175 - altitude,8,188 - altitude,GRAY);		// tail3

	if(jump_step>=44 || (jump_step == 0))	// going down
	{
		LCD_Fill(10,178 - altitude,12,181 - altitude,WHITE);	// clear tail1 top
		LCD_Fill(9,176 - altitude,10,179 - altitude,WHITE);		// clear tail2 top
		LCD_Fill(7,171 - altitude,8,174 - altitude,WHITE);		// clear tail3 top
	}
	if(jump_step <=44)	// going up
	{
		LCD_Fill(10,196 - altitude,12,199 - altitude,WHITE);	// clear tail1 bottom
		LCD_Fill(8,193 - altitude,9,196 - altitude,WHITE);		// clear tail2 bottom
		LCD_Fill(6,189 - altitude,7,192 - altitude,WHITE);		// clear tail3 bottom
	}



	// ----------- RIGHT LEG -----------
	LCD_Fill(23,197 - altitude,27,200 - altitude,GRAY);      						// right leg
	LCD_Fill(25,200 - altitude - right_leg,27,207 - altitude - right_leg,GRAY);		// leg animation
	LCD_Fill(27,205 - altitude - right_leg,30,207 - altitude - right_leg,GRAY);

	LCD_Fill(23,202 - altitude - right_leg,24,204 - altitude - right_leg,WHITE);	// clear thigh

	if(jump_step>=44 || (jump_step == 0))	// going down
	{
		LCD_Fill(28,202 - altitude - right_leg,30,204 - altitude - right_leg,WHITE);	// clear foot top

	}
	if(jump_step <=44)	// going up
	{
		LCD_Fill(23,209 - altitude - right_leg,30,212 - altitude - right_leg,WHITE);	// clear foot bottom
	}



	// ----------- LEFT LEG -----------
	LCD_Fill(15,197 - altitude,19,200 - altitude,GRAY);      						// left leg
	LCD_Fill(15,200 - altitude - left_leg,17,207 - altitude - left_leg,GRAY);		// leg animation
	LCD_Fill(17,205 - altitude - left_leg,20,207 - altitude - left_leg,GRAY);

	if((jump_step>=44) || (jump_step == 0))	// going down
	{
		LCD_Fill(18,202 - altitude - left_leg,20,204 - altitude - left_leg,WHITE);		// clear foot top with thigh
	}
	if(jump_step <=44)	// going up
	{
		LCD_Fill(18,202 - altitude - left_leg,20,204 - altitude - left_leg,WHITE);		// clear foot top with thigh
		LCD_Fill(15,209 - altitude - left_leg,20,212 - altitude - left_leg,WHITE);		// clear foot bottom
	}
}


// obstacle 1
void DrawObstacle1(uint16_t shift)	// single BIG
{
	// ----------- VERTICAL PART -----------
	LCD_Fill(353 - shift,150,359 - shift,153,GRAY);     // vertical
	LCD_Fill(352 - shift,153,360 - shift,210,GRAY);

	LCD_Fill(360 - shift,150,363 - shift,153,WHITE);		// clear vertical
	LCD_Fill(361 - shift,153,364 - shift,174,WHITE);		// clear vertical upper
	LCD_Fill(361 - shift,181,364 - shift,210,WHITE);		// clear vertical lower


	// ----------- HORIZONTAL PART -----------
	LCD_Fill(342 - shift,175,370 - shift,180,GRAY);     // horizontal


	// ----------- LEFT PART -----------
	LCD_Fill(341 - shift,165,345 - shift,167,GRAY);     // left
	LCD_Fill(340 - shift,167,346 - shift,178,GRAY);

	LCD_Fill(346 - shift,165,349 - shift,167,WHITE);		// clear left
	LCD_Fill(347 - shift,167,350 - shift,174,WHITE);		// clear left


	// ----------- RIGHT PART -----------
	LCD_Fill(367 - shift,160,371 - shift,162,GRAY);     // right
	LCD_Fill(366 - shift,162,372 - shift,178,GRAY);

	LCD_Fill(372 - shift,160,375 - shift,162,WHITE);     // clear right
	LCD_Fill(373 - shift,162,376 - shift,178,WHITE);
	LCD_Fill(371 - shift,179,375 - shift,181,WHITE);
}


// obstacle 2
void DrawObstacle2(uint16_t shift)	// single small
{
	// ----------- VERTICAL PART -----------
	LCD_Fill(353 - shift,170,357 - shift,172,GRAY);      // vertical
	LCD_Fill(352 - shift,172,358 - shift,210,GRAY);

	LCD_Fill(358 - shift,170,361 - shift,172,WHITE);		// clear vertical
	LCD_Fill(359 - shift,172,362 - shift,179,WHITE);		// clear vertical upper1
	LCD_Fill(359 - shift,180,361 - shift,185,WHITE);		// clear vertical upper2
	LCD_Fill(359 - shift,191,362 - shift,210,WHITE);		// clear vertical lower

	// ----------- HORIZONTAL PART -----------
	LCD_Fill(346 - shift,186,364 - shift,190,GRAY);      // horizontal


	// ----------- LEFT PART -----------
	LCD_Fill(345 - shift,177,347 - shift,178,GRAY);     // left
	LCD_Fill(344 - shift,178,348 - shift,188,GRAY);

	LCD_Fill(348 - shift,177,351 - shift,178,WHITE);		// clear left1
	LCD_Fill(349 - shift,179,351 - shift,185,WHITE);		// clear left2

	// ----------- RIGHT PART -----------
	LCD_Fill(362 - shift,180,365 - shift,181,GRAY);      // right
	LCD_Fill(361 - shift,181,366 - shift,188,GRAY);

	LCD_Fill(366 - shift,180,369 - shift,181,WHITE);      // clear right
	LCD_Fill(367 - shift,181,370 - shift,188,WHITE);
	LCD_Fill(365 - shift,189,369 - shift,190,WHITE);
}


// obstacle 3
void DrawObstacle3(uint16_t shift)		// BIG & small
{
	// big one
	// ----------- VERTICAL PART -----------
	LCD_Fill(357 - shift,150,361 - shift,153,GRAY);     // vertical1
	LCD_Fill(356 - shift,153,362 - shift,210,GRAY);

	LCD_Fill(362 - shift,150,365 - shift,152,WHITE);		// clear vertical1
	LCD_Fill(363 - shift,153,366 - shift,174,WHITE);		// clear vertical1 upper
	LCD_Fill(363 - shift,181,366 - shift,210,WHITE);		// clear vertical1 lower


	// ----------- HORIZONTAL PART -----------
	LCD_Fill(346 - shift,175,374 - shift,180,GRAY);     // horizontal1


	// ----------- LEFT PART -----------
	LCD_Fill(345 - shift,160,349 - shift,162,GRAY);     // left1
	LCD_Fill(344 - shift,162,350 - shift,178,GRAY);

	LCD_Fill(350 - shift,160,353 - shift,162,WHITE);     // clear left1
	LCD_Fill(351 - shift,162,354 - shift,174,WHITE);

	// ----------- RIGHT PART -----------
	LCD_Fill(371 - shift,165,375 - shift,167,GRAY);     // right1
	LCD_Fill(370 - shift,167,376 - shift,178,GRAY);

	LCD_Fill(376 - shift,165,379 - shift,167,WHITE);     // clear right1
	LCD_Fill(377 - shift,167,380 - shift,178,WHITE);
	LCD_Fill(375 - shift,179,378 - shift,180,WHITE);



	// small one
	// ----------- VERTICAL PART -----------
	LCD_Fill(388 - shift,170,392 - shift,172,GRAY);     // vertical2
	LCD_Fill(387 - shift,172,393 - shift,210,GRAY);

	LCD_Fill(393 - shift,170,396 - shift,172,WHITE);		// clear vertical2
	LCD_Fill(394 - shift,172,397 - shift,176,WHITE);		// clear vertical2 upper1
	LCD_Fill(394 - shift,177,396 - shift,185,WHITE);		// clear vertical2 upper2
	LCD_Fill(394 - shift,191,397 - shift,210,WHITE);		// clear vertical2 lower


	// ----------- HORIZONTAL PART -----------
	LCD_Fill(381 - shift,186,399 - shift,190,GRAY);     // horizontal2


	// ----------- LEFT PART -----------
	LCD_Fill(380 - shift,180,382 - shift,181,GRAY);     // left2
	LCD_Fill(379 - shift,181,383 - shift,188,GRAY);

	LCD_Fill(383 - shift,180,386 - shift,181,WHITE);     // clear left2
	LCD_Fill(384 - shift,181,386 - shift,185,WHITE);


	// ----------- RIGHT PART -----------
	LCD_Fill(397 - shift,177,400 - shift,178,GRAY);     // right2
	LCD_Fill(396 - shift,178,401 - shift,188,GRAY);

	LCD_Fill(401 - shift,177,404 - shift,178,WHITE);     // clear right2
	LCD_Fill(402 - shift,178,405 - shift,188,WHITE);
	LCD_Fill(400 - shift,189,403 - shift,190,WHITE);
	LCD_Fill(399 - shift,190,402 - shift,191,WHITE);
}
