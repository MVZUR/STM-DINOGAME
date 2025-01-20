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
#include <XPT2046_driver.h>
#include "ILI9341_paradriver.h"
#include "delay.h"
#include "stdlib.h"
#include "math.h"

//#include "24cxx.h"


_m_tp_dev tp_dev=
{
	TP_Init,
	TP_Scan,
	TP_Adjust,
	0,
	0,
 	0,
	0,
	0,
	0,
	0,
	0,	  	 		
	0,
	0,	  	 		
};					
//The default is data with touchtype=0.
u8 CMD_RDX=0XD0;	//208
u8 CMD_RDY=0X90;	//144

/*****************************************************************************
 * @name       :void TP_Write_Byte(u8 num)   
 * @date       :2018-08-09 
 * @function   :Write a byte data to the touch screen IC with SPI bus
 * @parameters :num:Data to be written
 * @retvalue   :None
******************************************************************************/  	 			    					   
void TP_Write_Byte(u8 num)    
{  
	u8 count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		//if(num&0x80)TDIN=1;
		if(num&0x80) TOUCH_TDIN_SET;
		//else TDIN=0;
		else TOUCH_TDIN_CLR;
		num<<=1;    
		//TCLK=0;
		TOUCH_TCLK_CLR;
		delay_us(1);
		//TCLK=1;		//Valid on rising edge
		TOUCH_TCLK_SET;
	}		 			    
}

/*****************************************************************************
 * @name       :u16 TP_Read_AD(u8 CMD)	  
 * @date       :2018-08-09 
 * @function   :Reading adc values from touch screen IC with SPI bus
 * @parameters :CMD:Read command,0xD0 for x,0x90 for y
 * @retvalue   :Read data
******************************************************************************/    
u16 TP_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 
	//TCLK=0;		//Pull down the clock first
	TOUCH_TCLK_CLR;
	//TDIN=0; 	//Pull the data line low
	TOUCH_TDIN_CLR;;
	//TCS=0; 		//Select touch screen ic
	TOUCH_TCS_CLR;
	TP_Write_Byte(CMD);	//Send command words
	delay_us(6);	//The conversion time of ADS7846 is up to 6us
	//TCLK=0;
	TOUCH_TCLK_CLR;
	delay_us(1);    	   
	//TCLK=1;			//Give 1 clock and clear busy
	TOUCH_TCLK_SET;
	delay_us(1);    
	//TCLK=0;
	TOUCH_TCLK_CLR;
	for(count=0;count<16;count++)	//Read out 16-bit data, only the upper 12 bits are valid
	{ 				  
		Num<<=1; 	 
		//TCLK=0;			//Valid on falling edge
		TOUCH_TCLK_CLR;
		delay_us(1);    
 		//TCLK=1;
		TOUCH_TCLK_SET;
 		if(DOUT)Num++; 		 
	}  	
	Num>>=4;   		//Only the upper 12 bits are valid.
	//TCS=1;			//release film selection
	TOUCH_TCS_SET;
	return(Num);  
//#endif
}

#define READ_TIMES 5 	//Read times
#define LOST_VAL 1	  	//discard valueֵ
/*****************************************************************************
 * @name       :u16 TP_Read_XOY(u8 xy)  
 * @date       :2018-08-09 
 * @function   :Read the touch screen coordinates (x or y),
								Read the READ_TIMES secondary data in succession 
								and sort the data in ascending order,
								Then remove the lowest and highest number of LOST_VAL 
								and take the average
 * @parameters :xy:Read command(CMD_RDX/CMD_RDY)
 * @retvalue   :Read data
******************************************************************************/  
u16 TP_Read_XOY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)buf[i]=TP_Read_AD(xy);		 		    
	for(i=0;i<READ_TIMES-1; i++)	//sort
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])	//Ascending order
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 

/*****************************************************************************
 * @name       :u8 TP_Read_XY(u16 *x,u16 *y)
 * @date       :2018-08-09 
 * @function   :Read touch screen x and y coordinates,
								The minimum value can not be less than 100
 * @parameters :x:Read x coordinate of the touch screen
								y:Read y coordinate of the touch screen
 * @retvalue   :0-fail,1-success
******************************************************************************/ 
u8 TP_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=TP_Read_XOY(CMD_RDX);
	ytemp=TP_Read_XOY(CMD_RDY);	  												   
	//if(xtemp<100||ytemp<100)return 0; //Reading failed
	*x=xtemp;
	*y=ytemp;
	return 1;	//Reading successful
}

#define ERR_RANGE 50 	//error range
/*****************************************************************************
 * @name       :u8 TP_Read_XY2(u16 *x,u16 *y) 
 * @date       :2018-08-09 
 * @function   :Read the touch screen coordinates twice in a row, 
								and the deviation of these two times can not exceed ERR_RANGE, 
								satisfy the condition, then think the reading is correct, 
								otherwise the reading is wrong.
								This function can greatly improve the accuracy.
 * @parameters :x:Read x coordinate of the touch screen
								y:Read y coordinate of the touch screen
 * @retvalue   :0-fail,1-success
******************************************************************************/ 
u8 TP_Read_XY2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=TP_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))	//The two samples before and after are within +50
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
} 

/*****************************************************************************
 * @name       :void TP_Drow_Touch_Point(u16 x,u16 y,u16 color)
 * @date       :2018-08-09 
 * @function   :Draw a touch point,Used to calibrate							
 * @parameters :x:Read x coordinate of the touch screen
								y:Read y coordinate of the touch screen
								color:the color value of the touch point
 * @retvalue   :None
******************************************************************************/  
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color)
{
	POINT_COLOR=color;
	LCD_DrawLine(x-12,y,x+13,y);	//horizontal line
	LCD_DrawLine(x,y-12,x,y+13);	//vertical line
	LCD_DrawPoint(x+1,y+1);
	LCD_DrawPoint(x-1,y+1);
	LCD_DrawPoint(x+1,y-1);
	LCD_DrawPoint(x-1,y-1);
	gui_circle(x,y,POINT_COLOR,6,0);	//draw center circle
}	

/*****************************************************************************
 * @name       :void TP_Draw_Big_Point(u16 x,u16 y,u16 color)
 * @date       :2018-08-09 
 * @function   :Draw a big point(2*2)					
 * @parameters :x:Read x coordinate of the point
								y:Read y coordinate of the point
								color:the color value of the point
 * @retvalue   :None
******************************************************************************/   
void TP_Draw_Big_Point(u16 x,u16 y,u16 color)
{	    
	POINT_COLOR=color;
	LCD_DrawPoint(x,y);	//center point
	LCD_DrawPoint(x+1,y);
	LCD_DrawPoint(x,y+1);
	LCD_DrawPoint(x+1,y+1);	 	  	
}	

/*****************************************************************************
 * @name       :u8 TP_Scan(u8 tp)
 * @date       :2018-08-09 
 * @function   :Scanning touch event				
 * @parameters :tp:0-screen coordinate 
									 1-Physical coordinates(For special occasions such as calibration)
 * @retvalue   :Current touch screen status,
								0-no touch
								1-touch
******************************************************************************/  					  
u8 TP_Scan(u8 tp)
{			   
	if(PEN==0)	//A button is pressed
	{
		if(tp)TP_Read_XY2(&tp_dev.x,&tp_dev.y);		//Read physical coordinates
		else if(TP_Read_XY2(&tp_dev.x,&tp_dev.y))	//Read screen coordinates
		{
	 		tp_dev.x=tp_dev.xfac*tp_dev.x+tp_dev.xoff;	//Convert the result to screen coordinates
			tp_dev.y=tp_dev.yfac*tp_dev.y+tp_dev.yoff;  
	 	} 
		if((tp_dev.sta&TP_PRES_DOWN)==0)	//has not been pressed before
		{		 
			tp_dev.sta=TP_PRES_DOWN|TP_CATH_PRES;	//button pressed
			tp_dev.x0=tp_dev.x;	//Record the coordinates when first pressed
			tp_dev.y0=tp_dev.y;  	   			 
		}			   
	}else
	{
		if(tp_dev.sta&TP_PRES_DOWN)		//It was pressed before
		{
			tp_dev.sta&=~(1<<7);	//Mark button released
		}else	//Never been pressed before
		{
			tp_dev.x0=0;
			tp_dev.y0=0;
			tp_dev.x=0xffff;
			tp_dev.y=0xffff;
		}	    
	}
	return tp_dev.sta&TP_PRES_DOWN; ////Return to current touch screen state
}
	  
//////////////////////////////////////////////////////////////////////////	 
//The base address of the address range stored in EEPROM takes up 13 bytes (RANGE: SAVE_ADDR_BASE~SAVE_ADDR_BASE+12)
#define SAVE_ADDR_BASE 40
/*****************************************************************************
 * @name       :void TP_Save_Adjdata(void)
 * @date       :2018-08-09 
 * @function   :Save calibration parameters		
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 										    
void TP_Save_Adjdata(void)
{
/*
	s32 temp;			 
	//Save calibration results!
	temp=tp_dev.xfac*100000000;	//Save x correction factor
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE,temp,4);   
	temp=tp_dev.yfac*100000000;	//Save y correction factors
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE+4,temp,4);
    //Save x offset
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE+8,tp_dev.xoff,2);		    
    //Save y offset
	AT24CXX_WriteLenByte(SAVE_ADDR_BASE+10,tp_dev.yoff,2);	
	//Save touch screen type
	AT24CXX_WriteOneByte(SAVE_ADDR_BASE+12,tp_dev.touchtype);	
	temp=0X0A;	//Marker calibrated
	AT24CXX_WriteOneByte(SAVE_ADDR_BASE+13,temp); 
*/
}

/*****************************************************************************
 * @name       :u8 TP_Get_Adjdata(void)
 * @date       :2018-08-09 
 * @function   :Gets the calibration values stored in the EEPROM		
 * @parameters :None
 * @retvalue   :1-get the calibration values successfully
								0-get the calibration values unsuccessfully and Need to recalibrate
******************************************************************************/ 	
u8 TP_Get_Adjdata(void)
{
/*
	s32 tempfac;
	tempfac=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+13);	//Read the mark word to see if it has been calibrated!
	if(tempfac==0X0A)	//The touch screen has been calibrated
	{    												 
		tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE,4);		   
		tp_dev.xfac=(float)tempfac/100000000;	//Get x calibration parameters
		tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+4,4);			          
		tp_dev.yfac=(float)tempfac/100000000;	//Get y calibration parameters
		//get x offset
		tp_dev.xoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+8,2);			   	  
		//get y offset
		tp_dev.yoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+10,2);				 	  
 		tp_dev.touchtype=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+12);	//Read touch screen type flag
		if(tp_dev.touchtype)	//The X and y directions are opposite to the screen
		{
			CMD_RDX=0X90;
			CMD_RDY=0XD0;	 
		}else				   //The X and y directions are the same as the screen
		{
			CMD_RDX=0XD0;
			CMD_RDY=0X90;	 
		}		 
		return 1;	 
	}
	return 0;
*/
}	
 
//prompt string
const u8* TP_REMIND_MSG_TBL="Please use the stylus click the cross on the screen.The cross will always move until the screen adjustment is completed.";

/*****************************************************************************
 * @name       :void TP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac)
 * @date       :2018-08-09 
 * @function   :Display calibration results	
 * @parameters :x0:the x coordinates of first calibration point
								y0:the y coordinates of first calibration point
								x1:the x coordinates of second calibration point
								y1:the y coordinates of second calibration point
								x2:the x coordinates of third calibration point
								y2:the y coordinates of third calibration point
								x3:the x coordinates of fourth calibration point
								y3:the y coordinates of fourth calibration point
								fac:calibration factor 
 * @retvalue   :None
******************************************************************************/ 	 					  
void TP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac)
{	  
	POINT_COLOR=RED;
	LCD_ShowString(40,140,16,"x1:",1);
 	LCD_ShowString(40+80,140,16,"y1:",1);
 	LCD_ShowString(40,160,16,"x2:",1);
 	LCD_ShowString(40+80,160, 16,"y2:",1);
	LCD_ShowString(40,180, 16,"x3:",1);
 	LCD_ShowString(40+80,180, 16,"y3:",1);
	LCD_ShowString(40,200, 16,"x4:",1);
 	LCD_ShowString(40+80,200, 16,"y4:",1);  
 	LCD_ShowString(40,220, 16,"fac is:",1);     
	LCD_ShowNum(40+24,140,x0,4,16);		//Display value
	LCD_ShowNum(40+24+80,140,y0,4,16);	//Display value
	LCD_ShowNum(40+24,160,x1,4,16);		//Display value
	LCD_ShowNum(40+24+80,160,y1,4,16);	//Display value
	LCD_ShowNum(40+24,180,x2,4,16);		//Display value
	LCD_ShowNum(40+24+80,180,y2,4,16);	//Display value
	LCD_ShowNum(40+24,200,x3,4,16);		//Display value
	LCD_ShowNum(40+24+80,200,y3,4,16);	//Display value
 	LCD_ShowNum(40+56,220,fac,3,16); 	//Display a numerical value, which must be within the range of 95~105.
}

/*****************************************************************************
 * @name       :u8 TP_Get_Adjdata(void)
 * @date       :2018-08-09 
 * @function   :Calibration touch screen and Get 4 calibration parameters
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 		 
void TP_Adjust(void)
{								 
	u16 pos_temp[4][2];	//Coordinate cache value
	u8  cnt=0;	
	u16 d1,d2;
	u32 tem1,tem2;
	double fac; 	
	u16 outtime=0;
 	cnt=0;				
	POINT_COLOR=BLUE;
	BACK_COLOR =WHITE;
	LCD_Clear(WHITE);	//clear screen
	POINT_COLOR=RED;	//red
	LCD_Clear(WHITE);	//clear screen
	POINT_COLOR=BLACK;
	LCD_ShowString(10,40,16,"Please use the stylus click",1);	//Show prompt information
	LCD_ShowString(10,56,16,"the cross on the screen.",1);	//Show prompt information
	LCD_ShowString(10,72,16,"The cross will always move",1);	//Show prompt information
	LCD_ShowString(10,88,16,"until the screen adjustment",1);	//Show prompt information
	LCD_ShowString(10,104,16,"is completed.",1);	//Show prompt information
	 
	TP_Drow_Touch_Point(20,20,RED);	//Paint point 1
	tp_dev.sta=0;	//Eliminate trigger signals
	tp_dev.xfac=0;	//Xfac is used to mark whether it has been calibrated, so it must be cleared before calibration! To avoid errors
	while(1)	//If it is not pressed for 10 consecutive seconds, it will automatically exit.
	{
		tp_dev.scan(1);	//Scan physical coordinates
		if((tp_dev.sta&0xc0)==TP_CATH_PRES)	//The button was pressed once (the button was released at this time.)
		{	
			outtime=0;		
			tp_dev.sta&=~(1<<6);	//Mark keys have been processed.
						   			   
			pos_temp[cnt][0]=tp_dev.x;
			pos_temp[cnt][1]=tp_dev.y;
			cnt++;	  
			switch(cnt)
			{			   
				case 1:						 
					TP_Drow_Touch_Point(20,20,WHITE);				//Clear point 1
					TP_Drow_Touch_Point(lcddev.width-20,20,RED);	//Paint point 2
					break;
				case 2:
 					TP_Drow_Touch_Point(lcddev.width-20,20,WHITE);	//Clear point 2
					TP_Drow_Touch_Point(20,lcddev.height-20,RED);	//Paint point 3
					break;
				case 3:
 					TP_Drow_Touch_Point(20,lcddev.height-20,WHITE);			//Clear point 3
 					TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,RED);	//Paint point 4
					break;
				case 4:	  //All four points have been obtained
					//Opposite sides are equal
					tem1=abs(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
					tem2=abs(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//Get the distance of 1,2
					
					tem1=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
					tem2=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);	//Get the distance of 3,4
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05||d1==0||d2==0)	//Unqualified
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	//Clear point 4
   	 					TP_Drow_Touch_Point(20,20,RED);								//Paint point 1
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//��ʾ����   
 						continue;
					}
					tem1=abs(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);	//Get the distance of 1,3
					
					tem1=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);	//Get the distance of 2,4
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)	//Unqualified
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	//Clear point 4
   	 					TP_Drow_Touch_Point(20,20,RED);								//Paint point 1
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//��ʾ����   
						continue;
					}	//Correct
								   
					//Diagonals are equal
					tem1=abs(pos_temp[1][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[1][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);	//get the distance of 1,4
	
					tem1=abs(pos_temp[0][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[0][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);	//Get the distance of 2,3
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)	//Unqualified
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	//Clear point 4
   	 					TP_Drow_Touch_Point(20,20,RED);								//Paint point 1
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//��ʾ����   
						continue;
					}	//Correct
					//Calculation result
					tp_dev.xfac=(float)(lcddev.width-40)/(pos_temp[1][0]-pos_temp[0][0]);	//getxfac
					tp_dev.xoff=(lcddev.width-tp_dev.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;	//getxoff
						  
					tp_dev.yfac=(float)(lcddev.height-40)/(pos_temp[2][1]-pos_temp[0][1]);	//getyfac
					tp_dev.yoff=(lcddev.height-tp_dev.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;	//getyoff
					if(abs(tp_dev.xfac)>2||abs(tp_dev.yfac)>2)	//The touch screen is the opposite of the default.
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	//Clear point 4
   	 					TP_Drow_Touch_Point(20,20,RED);								//Paint point 1
						LCD_ShowString(40,26, 16,"TP Need readjust!",1);
						tp_dev.touchtype=!tp_dev.touchtype;	//Modify the touch screen type.
						if(tp_dev.touchtype)	//The X and y directions are opposite to the screen
						{
							CMD_RDX=0X90;
							CMD_RDY=0XD0;	 
						}else				   //The X and y directions are the same as the screen
						{
							CMD_RDX=0XD0;
							CMD_RDY=0X90;	 
						}			    
						continue;
					}		
					POINT_COLOR=BLUE;
					LCD_Clear(WHITE);	//clear screen
					LCD_ShowString(35,110, 16,"Touch Screen Adjust OK!",1);	//Calibration completed
					delay_ms(1000);
					TP_Save_Adjdata();  
 					LCD_Clear(WHITE);	//clear screen
					return;	//Calibration completed
			}
		}
		delay_ms(10);
		outtime++;
		if(outtime>1000)
		{
			TP_Get_Adjdata();
			break;
	 	} 
 	}
}		

/*****************************************************************************
 * @name       :u8 TP_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization touch screen
 * @parameters :None
 * @retvalue   :0-no calibration
								1-Has been calibrated
******************************************************************************/  
u8 TP_Init(void)
{			    		   
	//Note that operations on GPIO are only effective after the clock is enabled.
	//Therefore, before pulling up, the clock must be enabled to achieve true pull-up output.


	// GPIO CONFIG HAD BEEN MADE IN main.c WITH CUBE MX
/*
		GPIO_InitTypeDef GPIO_Initure;	//GPIO
		__HAL_RCC_GPIOH_CLK_ENABLE();			//enable clock for gpio
        
		//PH9,12,13
		GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_12|GPIO_PIN_13;            
		GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //push-pull output
		GPIO_Initure.Pull=GPIO_PULLUP;          //Pull up
		GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //high speed
		HAL_GPIO_Init(GPIOH,&GPIO_Initure);     //initialization
              
		//PH10,11
		GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11;           
		GPIO_Initure.Mode=GPIO_MODE_INPUT;      //input
		HAL_GPIO_Init(GPIOH,&GPIO_Initure);     //initialization
 */
		    		     
  	TP_Read_XY(&tp_dev.x,&tp_dev.y);	//First read initialization
 	//AT24CXX_Init();	//initialize24cxx
	if(TP_Get_Adjdata())return 0;	//Already calibrated
	else			   //Not calibrated?
	{ 										    
		LCD_Clear(WHITE);	//clear screen
	    TP_Adjust();   		//Screen calibration
		TP_Save_Adjdata();	 
	}			
	TP_Get_Adjdata();	
	return 1; 									 
}


