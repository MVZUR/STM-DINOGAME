/*
 * figures.h
 *
 *  Created on: Jan 21, 2025
 *      Author: Maksymilian
 */

#ifndef INC_FIGURES_H_
#define INC_FIGURES_H_

#define RUBBER_TEST 0	// 1 - ON, 0 - OFF   (dino game figure erasing test)


#if RUBBER_TEST
	#define RUBBER       	BLUE	// define rubber as blue
#else
	#define RUBBER       	WHITE	// rubber as white
#endif

#endif /* INC_FIGURES_H_ */
