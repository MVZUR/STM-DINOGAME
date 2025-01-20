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
#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"	 
#include "stdlib.h"

//LCD important parameter set
typedef struct  
{										    
	u16 width;			// LCD width
	u16 height;			// LCD height
	u16 id;				// LCD ID
	u8  dir;			// Horizontal or portrait screen control: 0, portrait screen; 1, landscape screen.
	u16	 wramcmd;		// Start writing gram instructions
	u16  rramcmd;   	// Start reading gram instructions
	u16  setxcmd;		// Set x coordinate command
	u16  setycmd;		// Set y coordinate command
}_lcd_dev; 	

//LCD parameters
extern _lcd_dev lcddev;	// Manage important LCD parameters
/////////////////////////////////////User configuration area//////////////////////////////////////
#define USE_HORIZONTAL  	 1 // Define the clockwise rotation direction of the LCD screen: 0-0 degree rotation, 1-90 degree rotation, 2-180 degree rotation, 3-270 degree rotation
#define LCD_USE8BIT_MODEL   0	// Defines whether the data bus uses 8-bit mode 0, uses 16-bit mode. 1, uses 8-bit mode

//////////////////////////////////////////////////////////////////////////////////	  
//Define the size of the LCD
#define LCD_W 240
#define LCD_H 320

//Functions to be called outside the Tftlcd section
extern u16  POINT_COLOR;	//Default red
extern u16  BACK_COLOR; 	//Background color. Default is white

////////////////////////////////////////////////////////////////////
//----------------- LCD port definiton ----------------

// Official Author's note:
//If you use the official library function to define the following bottom layer,
//the speed will drop to 14 frames per second. It is recommended to use the method recommended by our company.

//The following IO definition directly operates the register,
//fast IO operation, and the screen refresh rate can reach 28 frames per second!

//QDtech's full series of modules use triodes to control the backlight on and off.
//Users can also connect PWM to adjust the backlight brightness.


//#define LED   -	//PC-   // backlight control pin (not in use)
#define RST   0     //PC0   // reset pin
#define CS    1		//PC1	// chip select pin
#define RS    2 	//PC2   // register / data selection pin
#define WR    3		//PC3   // write control pin
#define RD    4		//PC4   // read control pin


// LCD port control (with BIT BANDING)
/*
#define LCD_RST PCout(RST)
#define LCD_CS  PCout(CS)
#define LCD_RS  PCout(RS)
#define LCD_WR  PCout(WR)
#define LCD_RD  PCout(RD)

#define	LCD_RST_SET	(LCD_RST = 1)
#define	LCD_CS_SET  (LCD_CS = 1)
#define	LCD_RS_SET	(LCD_RS = 1)
#define	LCD_WR_SET	(LCD_WR = 1)
#define	LCD_RD_SET	(LCD_RD = 1)
*/


//LCD port control (without BIT BANDING)
#define GPIO_TYPE GPIOC		// LCD control pins port

//Gpio set (pulled high)
#define	LCD_RST_SET	GPIO_TYPE->BSRR=(1<<RST)
#define	LCD_CS_SET  GPIO_TYPE->BSRR=(1<<CS)
#define	LCD_RS_SET	GPIO_TYPE->BSRR=(1<<RS)
#define	LCD_WR_SET	GPIO_TYPE->BSRR=(1<<WR)
#define	LCD_RD_SET	GPIO_TYPE->BSRR=(1<<RD)
 							    
//Gpio reset (pull low)
#define	LCD_RST_CLR	GPIO_TYPE->BRR=(1<<RST)
#define	LCD_CS_CLR  GPIO_TYPE->BRR=(1<<CS)
#define	LCD_RS_CLR	GPIO_TYPE->BRR=(1<<RS)
#define	LCD_WR_CLR	GPIO_TYPE->BRR=(1<<WR)
#define	LCD_RD_CLR	GPIO_TYPE->BRR=(1<<RD)

				
//PB0~15, used as data line
//Note: If the 8-bit mode data bus is used, the upper 8 bits of the LCD screen data are connected to the upper 8 bits of the MCU bus.
//Example: If connected to 8-bit mode, the wiring in this example is that the LCD screen LCD_D0-D7 is connected to the microcontroller GPIOB_Pin8-GPIOB_Pin15.


// DATA IN/OUT bit assign macro
#define DATAOUT(x) 	GPIOB->ODR=x;	//Data output
#define DATAIN 		GPIOB->IDR; 	//Data input

//brush color
#define WHITE       	0xFFFF
#define BLACK      		0x0000
#define BLUE       		0x001F
#define BRED        	0XF81F
#define GRED 			0XFFE0
#define GBLUE			0X07FF
#define RED         	0xF800
#define MAGENTA     	0xF81F
#define GREEN       	0x07E0
#define CYAN        	0x7FFF
#define YELLOW      	0xFFE0
#define BROWN 			0XBC40
#define BRRED 			0XFC07
#define GRAY  			0X52AA	//DINOOO
#define DARKBLUE    	0X01CF
#define LIGHTBLUE  	 	0X7D7C
#define GRAYBLUE    	0X5458
#define LIGHTGREEN  	0X841F
#define LIGHTGRAY   	0XEF5B
#define LGRAY 			0XC618
#define LGRAYBLUE   	0XA651
#define LBBLUE      	0X2B12
	    															  
void LCD_Init(void);
void LCD_write(u16 VAL);
u16 LCD_read(void);
void LCD_Clear(u16 Color);	 
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_DrawPoint(u16 x,u16 y);							// painting point
u16  LCD_ReadPoint(u16 x,u16 y);							// reading point
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);
u16 LCD_RD_DATA(void);										// read LCD data
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
void LCD_WR_REG(u16 data);
void LCD_WR_DATA(u16 data);
void LCD_ReadReg(u16 LCD_Reg,u8 *Rval,int n);
void LCD_WriteRAM_Prepare(void);
void LCD_ReadRAM_Prepare(void);   
void Lcd_WriteData_16Bit(u16 Data);
u16 Lcd_ReadData_16Bit(void);
void LCD_direction(u8 direction );
u16 Color_To_565(u8 r, u8 g, u8 b);
u16 LCD_Read_ID(void);


////////////////////////////////////////////////////////////////////
//----------------- GUI functions ----------------
void GUI_DrawPoint(u16 x,u16 y,u16 color);
void LCD_Fill(int16_t sx,u16 sy,int16_t ex,u16 ey,u16 color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void Draw_Circle(u16 x0,u16 y0,u16 fc,u8 r);
void Draw_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
void Fill_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len,u8 size,u8 mode);
void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode);
void GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void GUI_DrawFont24(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void GUI_DrawFont32(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void Show_Str(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode);	// mode 0/1 - with/without background color
void Gui_Drawbmp16(u16 x,u16 y,const unsigned char *p); //Display 40*40 QQ pictures
void gui_circle(int xc, int yc,u16 c,int r, int fill);
void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode);
void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
////////////////////////////////////////////////////////////////////









//If you still feel that the speed is not fast enough, you can use the following macro definition to increase the speed.
//Be careful to remove the void LCD_WR_DATA(u16 data) function definition in lcd.c.

#if LCD_USE8BIT_MODEL==1			//Use 8-bit parallel data bus mode
	#define LCD_WR_DATA(data){\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	DATAOUT(data<<8);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	}
	#else							//Use 16-bit parallel data bus mode
	#define LCD_WR_DATA(data){\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	} 	
#endif

				  		 
#endif  
	 
	 



