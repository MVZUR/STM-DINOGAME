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
#include "ILI9341_paradriver.h"
#include "stdlib.h"
#include "delay.h"
#include "font.h"


	   
//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

//������ɫ,������ɫ
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 



void LCD_write(u16 VAL)
{
	LCD_CS_CLR;  
	DATAOUT(VAL);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
}


u16 LCD_read(void)
{
	u16 data;
	LCD_CS_CLR;
	LCD_RD_CLR;
	delay_us(1);//��ʱ1us	
	data = DATAIN;
	LCD_RD_SET;
	LCD_CS_SET;
	return data;
}

/*****************************************************************************
 * @name       :void LCD_WR_REG(u16 data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(u16 data)
{ 
   LCD_RS_CLR;     
	 #if LCD_USE8BIT_MODEL
	 LCD_write(data<<8);
	 #else
	 LCD_write(data);
	 #endif
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u16 data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
/*
void LCD_WR_DATA(u16 data)
{
	 LCD_RS_SET;
	 #if LCD_USE8BIT_MODEL
	 LCD_write(data<<8);
	 #else
	 LCD_write(data);
	 #endif
}
*/

/*****************************************************************************
 * @name       :u16 LCD_RD_DATA(void)
 * @date       :2018-11-13 
 * @function   :Read an 16-bit value from the LCD screen
 * @parameters :None
 * @retvalue   :read value
******************************************************************************/
u16 LCD_RD_DATA(void)
{
	LCD_RS_SET; 
	#if LCD_USE8BIT_MODEL
	return (LCD_read()>>8);
	#else
	return LCD_read();
	#endif
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   

/*****************************************************************************
 * @name       :u16 LCD_ReadReg(u16 LCD_Reg)
 * @date       :2018-11-13 
 * @function   :read value from specially registers
 * @parameters :LCD_Reg:Register address
 * @retvalue   :read value
******************************************************************************/
void LCD_ReadReg(u16 LCD_Reg,u8 *Rval,int n)
{
	LCD_WR_REG(LCD_Reg); 
	GPIOE->MODER=0;               //����
	GPIOE->BSRR=0X0000FFFF; //PE0-15 ����
	while(n--)
	{		
		*(Rval++) = LCD_RD_DATA();
	}
	
	GPIOE->MODER=0X55555555;      //���
	GPIOE->BSRR=0X0000FFFF; //PE0-15 ����
}

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 

/*****************************************************************************
 * @name       :void LCD_ReadRAM_Prepare(void)
 * @date       :2018-11-13 
 * @function   :Read GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void LCD_ReadRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.rramcmd);
}

/*****************************************************************************
 * @name       :void Lcd_WriteData_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void Lcd_WriteData_16Bit(u16 Data)
{	
   LCD_RS_SET; 
	 #if LCD_USE8BIT_MODEL
		LCD_CS_CLR;
		DATAOUT(Data);
		LCD_WR_CLR; 
		LCD_WR_SET;
		DATAOUT(Data<<8);
		LCD_WR_CLR; 
		LCD_WR_SET;
		LCD_CS_SET;
 //  LCD_write(Data&0xFF00);
//	 LCD_write(Data<<8);
	 #else
	 LCD_write(Data);
	 #endif
}

u16 Color_To_565(u8 r, u8 g, u8 b)
{
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

/*****************************************************************************
 * @name       :u16 Lcd_ReadData_16Bit(void)
 * @date       :2018-11-13 
 * @function   :Read an 16-bit value from the LCD screen
 * @parameters :None
 * @retvalue   :read value
******************************************************************************/	
u16 Lcd_ReadData_16Bit(void)
{
	u16 r,g,b;
	LCD_RS_SET;
	LCD_CS_CLR;
	
	//dummy data
	LCD_RD_CLR;
	delay_us(1);//��ʱ1us	
	r = DATAIN;
	LCD_RD_SET;
	
	//8bit:red data
	//16bit:red and green data
	LCD_RD_CLR;
	delay_us(1);//��ʱ1us	
	r = DATAIN;
	LCD_RD_SET;
	
	//8bit:green data
	//16bit:blue data
	LCD_RD_CLR;
	delay_us(1);//��ʱ1us	
	g = DATAIN;
	LCD_RD_SET;
	
	#if LCD_USE8BIT_MODEL	
	//blue data
	LCD_RD_CLR;
	delay_us(1);//��ʱ1us	
	b = DATAIN;
	LCD_RD_SET;
	r >>= 8;
	g >>= 8;
	b >>= 8;
	#else
	b = g>>8;
	r = r>>8;
	g = r&0xFF; 
	#endif
	LCD_CS_SET;
	return Color_To_565(r, g, b);
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//���ù��λ�� 
	Lcd_WriteData_16Bit(POINT_COLOR); 
}

/*****************************************************************************
 * @name       :u16 LCD_ReadPoint(u16 x,u16 y)
 * @date       :2018-11-13 
 * @function   :Read a pixel color value at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :the read color value
******************************************************************************/	
u16 LCD_ReadPoint(u16 x,u16 y)
{
	u16 color;
	if(x>=lcddev.width||y>=lcddev.height)
	{
		return 0;	//�����˷�Χ,ֱ�ӷ���	
	}
	LCD_SetCursor(x,y);//���ù��λ�� 
	LCD_ReadRAM_Prepare();
	GPIOE->MODER=0;               //����
	GPIOE->BSRR=0X0000FFFF; //PE0-15 ����
	
	color = Lcd_ReadData_16Bit();
	
	GPIOE->MODER=0X55555555;      //���
	GPIOE->BSRR=0X0000FFFF; //PE0-15 ����
	return color;
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void LCD_Clear(u16 Color)
{
  unsigned int i;//,m;  
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);   
	for(i=0;i<lcddev.height*lcddev.width;i++)
	{
 //   for(m=0;m<lcddev.width;m++)
  //  {	
			Lcd_WriteData_16Bit(Color);
	//	}
	}
} 

/*****************************************************************************
 * @name       :void LCD_GPIOInit(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_GPIOInit(void)
{
		GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOC_CLK_ENABLE();           //ʹ��GPIOCʱ��
		__HAL_RCC_GPIOE_CLK_ENABLE();           //ʹ��GPIOEʱ��
    
    GPIO_Initure.Pin=GPIO_PIN_10| GPIO_PIN_7|GPIO_PIN_8| GPIO_PIN_9|GPIO_PIN_6;            //PC10,6,7,8,9
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_LOW;     //����         
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);     //��ʼ��
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10| GPIO_PIN_7|GPIO_PIN_8| GPIO_PIN_9|GPIO_PIN_6, GPIO_PIN_SET);
		
		GPIO_Initure.Pin=GPIO_PIN_All;    //GPIOE
		HAL_GPIO_Init(GPIOE,&GPIO_Initure);     //��ʼ��
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_All, GPIO_PIN_SET);
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_RESET(void)
{
	LCD_RST_CLR;
	delay_ms(100);	
	LCD_RST_SET;
	delay_ms(50);
}

/*****************************************************************************
 * @name       :void LCD_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void ILI9341_paradriver_Init(void)
{  
	//LCD_GPIOInit();	// already done with CUBE MX
 	LCD_RESET(); //LCD ��λ
//*************2.4inch ILI9341��ʼ��**********//	
	LCD_WR_REG(0xCF);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0xD9); //0xC1 
	LCD_WR_DATA(0X30); 
	LCD_WR_REG(0xED);  
	LCD_WR_DATA(0x64); 
	LCD_WR_DATA(0x03); 
	LCD_WR_DATA(0X12); 
	LCD_WR_DATA(0X81); 
	LCD_WR_REG(0xE8);  
	LCD_WR_DATA(0x85); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x7A); 
	LCD_WR_REG(0xCB);  
	LCD_WR_DATA(0x39); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x34); 
	LCD_WR_DATA(0x02); 
	LCD_WR_REG(0xF7);  
	LCD_WR_DATA(0x20); 
	LCD_WR_REG(0xEA);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_REG(0xC0);    //Power control 
	LCD_WR_DATA(0x1B);   //VRH[5:0] 
	LCD_WR_REG(0xC1);    //Power control 
	LCD_WR_DATA(0x12);   //SAP[2:0];BT[3:0] 0x01
	LCD_WR_REG(0xC5);    //VCM control 
	LCD_WR_DATA(0x08); 	 //30
	LCD_WR_DATA(0x26); 	 //30
	LCD_WR_REG(0xC7);    //VCM control2 
	LCD_WR_DATA(0XB7); 
	LCD_WR_REG(0x36);    // Memory Access Control 
	LCD_WR_DATA(0x08); 
	LCD_WR_REG(0x3A);   
	LCD_WR_DATA(0x55); 
	LCD_WR_REG(0xB1);   
	LCD_WR_DATA(0x00);   
	LCD_WR_DATA(0x1A); 
	LCD_WR_REG(0xB6);    // Display Function Control 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0xA2); 
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
	LCD_WR_DATA(0x00); 
	LCD_WR_REG(0x26);    //Gamma curve selected 
	LCD_WR_DATA(0x01); 
	LCD_WR_REG(0xE0);    //Set Gamma 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x1D); 
	LCD_WR_DATA(0x1A); 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0x0D); 
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x49); 
	LCD_WR_DATA(0X66); 
	LCD_WR_DATA(0x3B); 
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x11); 
	LCD_WR_DATA(0x01); 
	LCD_WR_DATA(0x09); 
	LCD_WR_DATA(0x05); 
	LCD_WR_DATA(0x04); 		 
	LCD_WR_REG(0XE1);    //Set Gamma 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x18); 
	LCD_WR_DATA(0x1D); 
	LCD_WR_DATA(0x02); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x04); 
	LCD_WR_DATA(0x36); 
	LCD_WR_DATA(0x13); 
	LCD_WR_DATA(0x4C); 
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x13); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x2E); 
	LCD_WR_DATA(0x2F); 
	LCD_WR_DATA(0x05); 
	LCD_WR_REG(0x2B); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x3f);
	LCD_WR_REG(0x2A); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xef);	 
	LCD_WR_REG(0x11); //Exit Sleep
	delay_ms(120);
	LCD_WR_REG(0x29); //display on		

  LCD_direction(USE_HORIZONTAL);//����LCD��ʾ����
//	LCD_LED=1;//��������	 
	LCD_Clear(WHITE);//��ȫ����ɫ
}
 
/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);

	LCD_WriteRAM_Prepare();	//��ʼд��GRAM			
}   

/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void LCD_direction(u8 direction)
{ 
			lcddev.setxcmd=0x2A;
			lcddev.setycmd=0x2B;
			lcddev.wramcmd=0x2C;
			lcddev.rramcmd=0x2E;
	switch(direction){		  
		case 0:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;		
			LCD_WriteReg(0x36,(1<<3));
		break;
		case 1:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(1<<5)|(1<<6));
		break;
		case 2:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<4)|(1<<6));
		break;
		case 3:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<5)|(1<<4));
		break;	
		default:break;
	}		
}	 

/*****************************************************************************
 * @name       :u16 LCD_Read_ID(void)
 * @date       :2018-11-13 
 * @function   :Read ID
 * @parameters :None
 * @retvalue   :ID value
******************************************************************************/ 
u16 LCD_Read_ID(void)
{
	u8 val[4] = {0};
	LCD_ReadReg(0xD3,val,4);
	return (val[2]<<8)|val[3];
}



/*******************************************************************
 * @name       :void GUI_DrawPoint(u16 x,u16 y,u16 color)
 * @date       :2018-08-09
 * @function   :draw a point in LCD screen
 * @parameters :x:the x coordinate of the point
                y:the y coordinate of the point
								color:the color value of the point
 * @retvalue   :None
********************************************************************/
void GUI_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_SetCursor(x,y);//���ù��λ��
	Lcd_WriteData_16Bit(color);
}

/*******************************************************************
 * @name       :void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
 * @date       :2018-08-09
 * @function   :fill the specified area
 * @parameters :sx:the bebinning x coordinate of the specified area
                sy:the bebinning y coordinate of the specified area
								ex:the ending x coordinate of the specified area
								ey:the ending y coordinate of the specified area
								color:the filled color value
 * @retvalue   :None
********************************************************************/
void LCD_Fill(int16_t sx,u16 sy,int16_t ex,u16 ey,u16 color)	// x must be not less than 0 for obstacle shifting
{																// changing u16 to int16_t

	if(sx<0)
	{
		sx=0;
	}

	if(ex<0)
	{
		ex=0;
	}

	u16 i,j;
	u16 width=ex-sx+1; 		//�õ����Ŀ��
	u16 height=ey-sy+1;		//�߶�
	LCD_SetWindows(sx,sy,ex,ey);//������ʾ����
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		Lcd_WriteData_16Bit(color);	//д������
	}
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ���������Ϊȫ��
}

/*******************************************************************
 * @name       :void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
 * @date       :2018-08-09
 * @function   :Draw a line between two points
 * @parameters :x1:the bebinning x coordinate of the line
                y1:the bebinning y coordinate of the line
								x2:the ending x coordinate of the line
								y2:the ending y coordinate of the line
 * @retvalue   :None
********************************************************************/
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;

	delta_x=x2-x1; //������������
	delta_y=y2-y1;
	uRow=x1;
	uCol=y1;
	if(delta_x>0)incx=1; //���õ�������
	else if(delta_x==0)incx=0;//��ֱ��
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if(delta_y==0)incy=0;//ˮƽ��
	else{incy=-1;delta_y=-delta_y;}
	if( delta_x>delta_y)distance=delta_x; //ѡȡ��������������
	else distance=delta_y;
	for(t=0;t<=distance+1;t++ )//�������
	{
		LCD_DrawPoint(uRow,uCol);//����
		xerr+=delta_x ;
		yerr+=delta_y ;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

/*****************************************************************************
 * @name       :void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
 * @date       :2018-08-09
 * @function   :Draw a rectangle
 * @parameters :x1:the bebinning x coordinate of the rectangle
                y1:the bebinning y coordinate of the rectangle
								x2:the ending x coordinate of the rectangle
								y2:the ending y coordinate of the rectangle
 * @retvalue   :None
******************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}

/*****************************************************************************
 * @name       :void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
 * @date       :2018-08-09
 * @function   :Filled a rectangle
 * @parameters :x1:the bebinning x coordinate of the filled rectangle
                y1:the bebinning y coordinate of the filled rectangle
								x2:the ending x coordinate of the filled rectangle
								y2:the ending y coordinate of the filled rectangle
 * @retvalue   :None
******************************************************************************/
void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_Fill(x1,y1,x2,y2,POINT_COLOR);
}

/*****************************************************************************
 * @name       :void _draw_circle_8(int xc, int yc, int x, int y, u16 c)
 * @date       :2018-08-09
 * @function   :8 symmetry circle drawing algorithm (internal call)
 * @parameters :xc:the x coordinate of the Circular center
                yc:the y coordinate of the Circular center
								x:the x coordinate relative to the Circular center
								y:the y coordinate relative to the Circular center
								c:the color value of the circle
 * @retvalue   :None
******************************************************************************/
void _draw_circle_8(int xc, int yc, int x, int y, u16 c)
{
	GUI_DrawPoint(xc + x, yc + y, c);

	GUI_DrawPoint(xc - x, yc + y, c);

	GUI_DrawPoint(xc + x, yc - y, c);

	GUI_DrawPoint(xc - x, yc - y, c);

	GUI_DrawPoint(xc + y, yc + x, c);

	GUI_DrawPoint(xc - y, yc + x, c);

	GUI_DrawPoint(xc + y, yc - x, c);

	GUI_DrawPoint(xc - y, yc - x, c);
}

/*****************************************************************************
 * @name       :void gui_circle(int xc, int yc,u16 c,int r, int fill)
 * @date       :2018-08-09
 * @function   :Draw a circle of specified size at a specified location
 * @parameters :xc:the x coordinate of the Circular center
                yc:the y coordinate of the Circular center
								r:Circular radius
								fill:1-filling,0-no filling
 * @retvalue   :None
******************************************************************************/
void gui_circle(int xc, int yc,u16 c,int r, int fill)
{
	int x = 0, y = r, yi, d;

	d = 3 - 2 * r;


	if (fill)
	{
		// �����䣨��ʵ��Բ��
		while (x <= y) {
			for (yi = x; yi <= y; yi++)
				_draw_circle_8(xc, yc, x, yi, c);

			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	} else
	{
		// �������䣨������Բ��
		while (x <= y) {
			_draw_circle_8(xc, yc, x, y, c);
			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
}

/*****************************************************************************
 * @name       :void Draw_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
 * @date       :2018-08-09
 * @function   :Draw a triangle at a specified position
 * @parameters :x0:the bebinning x coordinate of the triangular edge
                y0:the bebinning y coordinate of the triangular edge
								x1:the vertex x coordinate of the triangular
								y1:the vertex y coordinate of the triangular
								x2:the ending x coordinate of the triangular edge
								y2:the ending y coordinate of the triangular edge
 * @retvalue   :None
******************************************************************************/
void Draw_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
{
	LCD_DrawLine(x0,y0,x1,y1);
	LCD_DrawLine(x1,y1,x2,y2);
	LCD_DrawLine(x2,y2,x0,y0);
}

static void _swap(u16 *a, u16 *b)
{
	u16 tmp;
  tmp = *a;
	*a = *b;
	*b = tmp;
}

/*****************************************************************************
 * @name       :void Fill_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
 * @date       :2018-08-09
 * @function   :filling a triangle at a specified position
 * @parameters :x0:the bebinning x coordinate of the triangular edge
                y0:the bebinning y coordinate of the triangular edge
								x1:the vertex x coordinate of the triangular
								y1:the vertex y coordinate of the triangular
								x2:the ending x coordinate of the triangular edge
								y2:the ending y coordinate of the triangular edge
 * @retvalue   :None
******************************************************************************/
void Fill_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
{
	u16 a, b, y, last;
	int dx01, dy01, dx02, dy02, dx12, dy12;
	long sa = 0;
	long sb = 0;
 	if (y0 > y1)
	{
    _swap(&y0,&y1);
		_swap(&x0,&x1);
 	}
 	if (y1 > y2)
	{
    _swap(&y2,&y1);
		_swap(&x2,&x1);
 	}
  if (y0 > y1)
	{
    _swap(&y0,&y1);
		_swap(&x0,&x1);
  }
	if(y0 == y2)
	{
		a = b = x0;
		if(x1 < a)
    {
			a = x1;
    }
    else if(x1 > b)
    {
			b = x1;
    }
    if(x2 < a)
    {
			a = x2;
    }
		else if(x2 > b)
    {
			b = x2;
    }
		LCD_Fill(a,y0,b,y0,POINT_COLOR);
    return;
	}
	dx01 = x1 - x0;
	dy01 = y1 - y0;
	dx02 = x2 - x0;
	dy02 = y2 - y0;
	dx12 = x2 - x1;
	dy12 = y2 - y1;

	if(y1 == y2)
	{
		last = y1;
	}
  else
	{
		last = y1-1;
	}
	for(y=y0; y<=last; y++)
	{
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
    sb += dx02;
    if(a > b)
    {
			_swap(&a,&b);
		}
		LCD_Fill(a,y,b,y,POINT_COLOR);
	}
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y<=y2; y++)
	{
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		if(a > b)
		{
			_swap(&a,&b);
		}
		LCD_Fill(a,y,b,y,POINT_COLOR);
	}
}

/*****************************************************************************
 * @name       :void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode)
 * @date       :2018-08-09
 * @function   :Display a single English character
 * @parameters :x:the bebinning x coordinate of the Character display position
                y:the bebinning y coordinate of the Character display position
								fc:the color value of display character
								bc:the background color of display character
								num:the ascii code of display character(0~94)
								size:the size of display character
								mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode)
{
    u8 temp;
    u8 pos,t;
	u16 colortemp=POINT_COLOR;

	num=num-' ';//�õ�ƫ�ƺ��ֵ
	LCD_SetWindows(x,y,x+size/2-1,y+size-1);//���õ���������ʾ����
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//����1206����
			else temp=asc2_1608[num][pos];		 //����1608����
			for(t=0;t<size/2;t++)
		    {
		        if(temp&0x01)Lcd_WriteData_16Bit(fc);
				else Lcd_WriteData_16Bit(bc);
				temp>>=1;

		    }

		}
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//����1206����
			else temp=asc2_1608[num][pos];		 //����1608����
			for(t=0;t<size/2;t++)
		    {
				POINT_COLOR=fc;
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//��һ����
		        temp>>=1;
		    }
		}
	}
	POINT_COLOR=colortemp;
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ�����Ϊȫ��
}

/*****************************************************************************
 * @name       :void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode)
 * @date       :2018-08-09
 * @function   :Display English string
 * @parameters :x:the bebinning x coordinate of the English string
                y:the bebinning y coordinate of the English string
								p:the start address of the English string
								size:the size of display character
								mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/
void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode)
{
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {
		if(x>(lcddev.width-1)||y>(lcddev.height-1))
		return;
        LCD_ShowChar(x,y,POINT_COLOR,BACK_COLOR,*p,size,mode);
        x+=size/2;
        p++;
    }
}

/*****************************************************************************
 * @name       :u32 mypow(u8 m,u8 n)
 * @date       :2018-08-09
 * @function   :get the nth power of m (internal call)
 * @parameters :m:the multiplier
                n:the power
 * @retvalue   :the nth power of m
******************************************************************************/
u32 mypow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)result*=m;
	return result;
}

/*****************************************************************************
 * @name       :void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
 * @date       :2018-08-09
 * @function   :Display number
 * @parameters :x:the bebinning x coordinate of the number
                y:the bebinning y coordinate of the number
								num:the number(0~4294967295)
								len:the length of the display number
								size:the size of display number
 * @retvalue   :None
******************************************************************************/
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{
	u8 t,temp;
	u8 enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,' ',size,0);
				continue;
			}else enshow=1;

		}
	 	LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,temp+'0',size,0);
	}
}


/*****************************************************************************
 * @name       :void Show_Str(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
 * @date       :2018-08-09
 * @function   :Display English strings
 * @parameters :x:the bebinning x coordinate of the English strings
                y:the bebinning y coordinate of the English strings
								fc:the color value of English strings
								bc:the background color of English strings
								str:the start address of the English strings
								size:the size of English strings
								mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/
void Show_Str(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
{
	u16 x0=x;
  	u8 bHz=0;     //�ַ���������
    while(*str!=0)//����δ����
    {
        if(!bHz)
        {
			if(x>(lcddev.width-size/2)||y>(lcddev.height-size))
			return;
	        if(*str>0x80)bHz=1;//����
	        else              //�ַ�
	        {
		        if(*str==0x0D)//���з���
		        {
		            y+=size;
					x=x0;
		            str++;
		        }
		        else
				{
					if(size>16)//�ֿ���û�м���12X24 16X32��Ӣ������,��8X16����
					{
					LCD_ShowChar(x,y,fc,bc,*str,16,mode);
					x+=8; //�ַ�,Ϊȫ�ֵ�һ��
					}
					else
					{
					LCD_ShowChar(x,y,fc,bc,*str,size,mode);
					x+=size/2; //�ַ�,Ϊȫ�ֵ�һ��
					}
				}
				str++;

	        }
        }
    }
}

/*****************************************************************************
 * @name       :void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
 * @date       :2018-08-09
 * @function   :Centered display of English and Chinese strings
 * @parameters :x:the bebinning x coordinate of the Chinese and English strings
                y:the bebinning y coordinate of the Chinese and English strings
								fc:the color value of Chinese and English strings
								bc:the background color of Chinese and English strings
								str:the start address of the Chinese and English strings
								size:the size of Chinese and English strings
								mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/
void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
{
	u16 len=strlen((const char *)str);
	u16 x1=(lcddev.width-len*8)/2;
	Show_Str(x1,y,fc,bc,str,size,mode);
}

/*****************************************************************************
 * @name       :void Gui_Drawbmp16(u16 x,u16 y,const unsigned char *p)
 * @date       :2018-08-09
 * @function   :Display a 16-bit BMP image
 * @parameters :x:the bebinning x coordinate of the BMP image
                y:the bebinning y coordinate of the BMP image
								p:the start address of image array
 * @retvalue   :None
******************************************************************************/
void Gui_Drawbmp16(u16 x,u16 y,const unsigned char *p) //��ʾ40*40 QQͼƬ
{
  	int i;
	unsigned char picH,picL;
	LCD_SetWindows(x,y,x+40-1,y+40-1);//��������
    for(i=0;i<40*40;i++)
	{
	 	picL=*(p+i*2);	//���ݵ�λ��ǰ
		picH=*(p+i*2+1);
		Lcd_WriteData_16Bit(picH<<8|picL);
	}
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ���ʾ����Ϊȫ��
}

