//////////////////////////////////////////////////////////////////////////////////
// Official Author's note:
// This program is for learning purposes only. Without the author's permission,
// it may not be used for any other purpose.
// Test hardware: MCU STM32F429IGT6, Apollo STM32F4/F7 development board by ALIENTEK, main frequency 180MHz, crystal oscillator 12MHz
// QDtech-TFT LCD driver for STM32 IO simulation
// Xiao Feng @ShenZhen QDtech co.,LTD
// Company website: www.qdtft.com
// Taobao website: http://qdtech.taobao.com
// Wiki technical site: http://www.lcdwiki.com
// Technical support is provided by our company. Any technical questions are welcome for exchange and learning.
// Phone (Fax): +86 0755-23594567
// Mobile: 15989313508 (Mr. Feng)
// Email: lcdwiki01@gmail.com, support@lcdwiki.com, goodtft@163.com
// Technical Support QQ: 3002773612, 3002778157
// Technical exchange QQ group: 324828016
// Creation date: 2018/08/09
// Version: V1.0
// All rights reserved. Unauthorized use is prohibited.
// Copyright(C) Shenzhen QD Electronic Technology Co., Ltd. 2018-2028
//****************************************************************************************************
//
//////////////////////////////////////////////////////////////////////////////////
// LGVL REMAKE by Maksymilian Mazur
// AGH University of Krakow
// This remake was made for educational purpose only
// Test hardware: MCU STM32F303RET6, Nucleo development board by ST Microelectronics, main frequency 72MHz
// Creation date: 2024/12/26
// Version: V1.0
//****************************************************************************************************
//
//=========================================Power Connection=========================================//
//     LCD Module                 	STM32 Microcontroller
//      VCC          Connects to  	DC5V/3.3V        			// Power supply
//      GND          Connects to  	GND              			// Ground
//=========================================LCD Data Line Connection=================================//
// The data bus type of this module is an 8-bit parallel bus.
//     LCD Module                 	STM32 Microcontroller
//     LCD_D0~D7     Connects to  	PB8~PB15         			// LCD 8-bit parallel data signal
//=========================================LCD Control Line Connection=============================//
//     LCD Module                 	STM32 Microcontroller
//     LCD_RST       Connects to  	PC0 						// LCD reset control signal
//     LCD_CS        Connects to  	PC1              			// LCD chip select control signal
//     LCD_RS        Connects to  	PC2              			// LCD data/command control signal
//     LCD_WR        Connects to  	PC3              			// LCD write data control signal
//     LCD_RD        Connects to  	PC4             			// LCD read data control signal
//=========================================Touch Screen Connection=================================//
// If the module does not have touch functionality or touch functionality is not needed, touch connections are not required.
//     LCD Module                 	STM32 Microcontroller
//      T_IRQ        Connects to  	-            				// Touch screen interrupt signal
//      T_DO         Connects to  	-           	 			// Touch screen SPI bus read signal
//      T_DIN        Connects to  	-       	     			// Touch screen SPI bus write signal
//      T_CS         Connects to  	-   	         			// Touch screen chip select signal
//      T_CLK        Connects to  	-	             			// Touch screen SPI bus clock signal
//**************************************************************************************************/
/* @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/
#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "sys.h"

#define TP_PRES_DOWN 0x80  //Touch screen is pressed
#define TP_CATH_PRES 0x40  //A button is pressed

										    
//touch screen controller
typedef struct
{
	u8 (*init)(void);			//Initialize the touch screen controller
	u8 (*scan)(u8);				//Scan the touch screen.0, screen scan; 1, physical coordinates;
	void (*adjust)(void);		//Touch screen calibration
	u16 x0;						//Original coordinates (coordinates when first pressed)
	u16 y0;
	u16 x; 						//Current coordinates (the coordinates of the touch screen during this scan)
	u16 y;						   	    
	u8  sta;					//pen status
								//b7: Press 1/release 0;
								//b6:0, no button is pressed; 1, there is a button pressed.

////////////////////////Touch screen calibration parameters////////////////////////////
	float xfac;					
	float yfac;
	short xoff;
	short yoff;	   
//The newly added parameter is needed when the left and right of the touch screen are completely reversed.
//When touchtype=0, it is suitable for TP where the left and right are X coordinates and the top and bottom are Y coordinates.
//When touchtype=1, it is suitable for TP where the left and right are Y coordinates and the top and bottom are X coordinates.

	u8 touchtype;
}_m_tp_dev;

extern _m_tp_dev tp_dev;	 	//The touch screen controller is defined in touch.c


//Connect pins with touch screen chip

// TOUCH port control (with BIT BANDING)
/*
#define PEN  PHin(10)    	//T_PEN
#define DOUT PHin(11)     	//T_MISO
#define TDIN PHout(12)    	//T_MOSI
#define TCLK PHout(9)    	//T_SCK
#define TCS  PHout(13)   	//T_CS
*/

// TOUCH port control (without BIT BANDING)
#define TCS_PIN  3	//PA3	//T_CS		//(output, PUSH-PULL, PULL-UP, HI-SPD)
#define PEN_PIN  4	//PA4	//T_PEN		//(input, PULL-UP)
#define TCLK_PIN 1	//PA1	//T_SCK		//(output, SCK, PUSH-PULL, PULL-UP, HI-SPD)
#define DOUT_PIN 2	//PA2	//T_MISO	//(input, MISO, PULL-UP)
#define TDIN_PIN 7	//PA7	//T_MOSI	//(output, MOSI,  PUSH-PULL, PULL-UP, HI-SPD)


#define TGPIO_TYPE GPIOA		// LCD control pins port

#define TOUCH_TCS_SET	TGPIO_TYPE->BSRR=(1<<TCS_PIN)
#define TOUCH_TCLK_SET	TGPIO_TYPE->BRR=(1<<TCLK_PIN) //TGPIO_TYPE->BSRR=(1<<TCLK_PIN)
#define TOUCH_TDIN_SET	TGPIO_TYPE->BSRR=(1<<TDIN_PIN)

#define TOUCH_TCS_CLR	TGPIO_TYPE->BRR=(1<<TCS_PIN)
#define TOUCH_TCLK_CLR	TGPIO_TYPE->BRR=(1<<TCLK_PIN)
#define TOUCH_TDIN_CLR	TGPIO_TYPE->BRR=(1<<TDIN_PIN)

//#define PEN			((MEM_ADDR(GPIOA_IDR_Addr) >> PEN_PIN) & 1U)
//#define DOUT		((MEM_ADDR(GPIOA_IDR_Addr) >> DOUT_PIN) & 1U)

#define PEN			HAL_GPIO_ReadPin(TGPIO_TYPE, GPIO_PIN_4)
#define DOUT		HAL_GPIO_ReadPin(TGPIO_TYPE, GPIO_PIN_2)

	   
void TP_Write_Byte(u8 num);							//Write a data to the control chip
u16 TP_Read_AD(u8 CMD);								//Read ad conversion value
u16 TP_Read_XOY(u8 xy);								//Coordinate reading (x/y) with filtering
u8 TP_Read_XY(u16 *x,u16 *y);						//Reading in both directions (x+y)
u8 TP_Read_XY2(u16 *x,u16 *y);						//Bidirectional coordinate reading with enhanced filtering
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color);	//Draw a coordinate calibration point
void TP_Draw_Big_Point(u16 x,u16 y,u16 color);		//draw a bigger one
u8 TP_Scan(u8 tp);									//scanning
void TP_Save_Adjdata(void);							//Save calibration parameters
u8 TP_Get_Adjdata(void);							//Read calibration parameters
void TP_Adjust(void);								//Touch screen calibration
u8 TP_Init(void);									//initialization
																 
void TP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac);		//Display calibration information
 		  
#endif

















