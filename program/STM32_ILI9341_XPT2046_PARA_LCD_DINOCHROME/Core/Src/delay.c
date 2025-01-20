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
#include "delay.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
// If using uC/OS, include the following header file.
#if SYSTEM_SUPPORT_OS
#include "includes.h"                    // uC/OS usage
#endif

// Modification notes
//////////////////////////////////////////////////////////////////////////////////

static u32 fac_us=0;                            // Multiplier for microsecond delays

#if SYSTEM_SUPPORT_OS
    static u16 fac_ms=0;                        // Multiplier for millisecond delays; under OS, represents ms per tick
#endif

#if SYSTEM_SUPPORT_OS                          // If SYSTEM_SUPPORT_OS is defined, support for OS (not limited to uC/OS) is enabled.
// When delay_us/delay_ms needs OS support, three OS-related macros and functions are required.
// Macros:
// delay_osrunning: Indicates whether the OS is running, to decide if related functions can be used.
// delay_ostickspersec: Indicates the OS clock tick rate; delay_init will initialize SysTick based on this.
// delay_osintnesting: Indicates the OS interrupt nesting level; delay_ms uses this to decide how to operate.
// Functions:
// delay_osschedlock: Locks OS task scheduling, prohibiting scheduling.
// delay_osschedunlock: Unlocks OS task scheduling, allowing scheduling again.
// delay_ostimedly: Delays using the OS, enabling task scheduling.

// This example supports uC/OS-II and uC/OS-III. For other OS, please port it accordingly.
// Support for uC/OS-II
#ifdef  OS_CRITICAL_METHOD                     // If OS_CRITICAL_METHOD is defined, support for uC/OS-II is enabled.
#define delay_osrunning     OSRunning         // OS running status flag, 0: not running; 1: running
#define delay_ostickspersec OS_TICKS_PER_SEC  // OS clock tick rate, i.e., scheduling times per second
#define delay_osintnesting  OSIntNesting      // Interrupt nesting level, i.e., interrupt nesting count
#endif

// Support for uC/OS-III
#ifdef  CPU_CFG_CRITICAL_METHOD                // If CPU_CFG_CRITICAL_METHOD is defined, support for uC/OS-III is enabled.
#define delay_osrunning     OSRunning         // OS running status flag, 0: not running; 1: running
#define delay_ostickspersec OSCfg_TickRate_Hz // OS clock tick rate, i.e., scheduling times per second
#define delay_osintnesting  OSIntNestingCtr   // Interrupt nesting level, i.e., interrupt nesting count
#endif

// For microsecond-level delay, disable task scheduling (to prevent interruption of us-level delays).
void delay_osschedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD                // Using uC/OS-III
    OS_ERR err;
    OSSchedLock(&err);                        // uC/OS-III method to disable scheduling, preventing interruption of us delay
#else                                         // Otherwise, uC/OS-II
    OSSchedLock();                            // uC/OS-II method to disable scheduling, preventing interruption of us delay
#endif
}

// For microsecond-level delay, re-enable task scheduling.
void delay_osschedunlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD                // Using uC/OS-III
    OS_ERR err;
    OSSchedUnlock(&err);                      // uC/OS-III method to re-enable scheduling
#else                                         // Otherwise, uC/OS-II
    OSSchedUnlock();                          // uC/OS-II method to re-enable scheduling
#endif
}

// Delay using OS's built-in delay function
// ticks: Number of ticks to delay
void delay_ostimedly(u32 ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
    OS_ERR err;
    OSTimeDly(ticks,OS_OPT_TIME_PERIODIC,&err); // uC/OS-III delay uses periodic mode
#else
    OSTimeDly(ticks);                          // uC/OS-II delay
#endif 
}
 
// SysTick interrupt service function, used when OS is enabled
void SysTick_Handler(void)
{
    HAL_IncTick();
    if(delay_osrunning==1)                    // Only execute normal scheduling processing if OS is running
    {
        OSIntEnter();                        // Enter interrupt
        OSTimeTick();                        // Call uC/OS clock service program
        OSIntExit();                         // Trigger task switch soft interrupt
    }
}
#endif

// Initialize delay function
// When using uC/OS, this function initializes the uC/OS clock tick.
// The SYSTICK clock is fixed to the AHB clock.
// SYSCLK: System clock frequency
void delay_init(u8 SYSCLK)
{
#if SYSTEM_SUPPORT_OS                         // If OS support is needed
    u32 reload;
#endif
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK); // SysTick frequency is HCLK
    fac_us=SYSCLK;                        // fac_us is used regardless of OS usage
#if SYSTEM_SUPPORT_OS                         // If OS support is needed
    reload=SYSCLK;                        // Number of counts per second, unit: K
    reload*=1000000/delay_ostickspersec;   // Set overflow time based on delay_ostickspersec
                                            // reload is a 24-bit register, max value: 16777216, approx. 0.745s at 180M
    fac_ms=1000/delay_ostickspersec;       // Represents the minimum unit of delay provided by OS
    SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk; // Enable SysTick interrupt
    SysTick->LOAD=reload;                   // Interrupt every 1/OS_TICKS_PER_SEC second
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; // Enable SysTick
#else
#endif
}

#if SYSTEM_SUPPORT_OS                         // If OS support is needed
// Delay for nus microseconds
// nus: Number of microseconds to delay
// nus: 0~190887435 (maximum value is 2^32/fac_us when fac_us=22.5)
void delay_us(u32 nus)
{
    u32 ticks;
    u32 told,tnow,tcnt=0;
    u32 reload=SysTick->LOAD;                // Value of LOAD
    ticks=nus*fac_us;                        // Number of ticks needed
    delay_osschedlock();                    // Prevent OS scheduling, preventing interruption of us delay
    told=SysTick->VAL;                        // Counter value upon entry
    while(1)
    {
        tnow=SysTick->VAL;
        if(tnow!=told)
        {
            if(tnow<told)tcnt+=told-tnow;    // Note that SYSTICK is a decrementing counter.
            else tcnt+=reload-tnow+told;
            told=tnow;
            if(tcnt>=ticks)break;            // Exit if time exceeds or equals the delay time.
        }
    };
    delay_osschedunlock();                    // Restore OS scheduling
}  
// Delay for nms milliseconds
// nms: Number of milliseconds to delay
// nms: 0~65535
void delay_ms(u16 nms)
{
    if(delay_osrunning&&delay_osintnesting==0)// If OS is running and not in interrupt (task scheduling cannot occur in interrupt)
    {
        if(nms>=fac_ms)                        // Delay time is greater than OS minimum cycle
        {
            delay_ostimedly(nms/fac_ms);    // OS delay
        }
        nms%=fac_ms;                        // Use normal delay for remaining small delay
    }
    delay_us((u32)(nms*1000));                // Normal delay
}
#else  // Without uC/OS

// Delay for nus microseconds
// nus: Number of microseconds to delay
// nus: 0~190887435 (maximum value is 2^32/fac_us when fac_us=22.5)
void delay_us(u32 nus)
{
    u32 ticks;
    u32 told,tnow,tcnt=0;
    u32 reload=SysTick->LOAD;                // Value of LOAD
    ticks=nus*fac_us;                        // Number of ticks needed
    told=SysTick->VAL;                        // Counter value upon entry
    while(1)
    {
        tnow=SysTick->VAL;
        if(tnow!=told)
        {
            if(tnow<told)tcnt+=told-tnow;    // Note that SYSTICK is a decrementing counter.
            else tcnt+=reload-tnow+told;
            told=tnow;
            if(tcnt>=ticks)break;            // Exit if time exceeds or equals the delay time.
        }
    };
}

// Delay for nms milliseconds
// nms: Number of milliseconds to delay
void delay_ms(u16 nms)
{
    u32 i;
    for(i=0;i<nms;i++) delay_us(1000);
}
#endif
