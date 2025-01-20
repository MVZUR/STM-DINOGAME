#ifndef _SYS_H
#define _SYS_H
#include "stm32f3xx_hal.h"



// 0 - does not support os
// 1 - support os
#define SYSTEM_SUPPORT_OS		0		//Define whether the system folder supports OS
///////////////////////////////////////////////////////////////////////////////////

//Define some common data type short keywords
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  
typedef const int16_t sc16;  
typedef const int8_t sc8;  

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  
typedef __I int16_t vsc16; 
typedef __I int8_t vsc8;   

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  
typedef const uint16_t uc16;  
typedef const uint8_t uc8; 

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  
typedef __I uint16_t vuc16; 
typedef __I uint8_t vuc8;  


// WARNING: BITBANDING does not work with STM32F303RET6 and any with GPIO on AHB2 (0x48000000) addresses
//Bit band operation to achieve 51 similar GPIO control functions
//For specific implementation ideas, refer to Chapter 5 of <<CM3 Authoritative Guide>> (pages 87~92). M4 is similar to M3, except that the register address has changed.
//IO port operation macro definition

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

//Io port address mapping
#define GPIOA_ODR_Addr    (GPIOA_BASE+20)
#define GPIOB_ODR_Addr    (GPIOB_BASE+20)
#define GPIOC_ODR_Addr    (GPIOC_BASE+20)
#define GPIOD_ODR_Addr    (GPIOD_BASE+20)
#define GPIOE_ODR_Addr    (GPIOE_BASE+20)
#define GPIOF_ODR_Addr    (GPIOF_BASE+20)
#define GPIOG_ODR_Addr    (GPIOG_BASE+20)
#define GPIOH_ODR_Addr    (GPIOH_BASE+20)
#define GPIOI_ODR_Addr    (GPIOI_BASE+20)
#define GPIOJ_ODR_ADDr    (GPIOJ_BASE+20)
#define GPIOK_ODR_ADDr    (GPIOK_BASE+20)

#define GPIOA_IDR_Addr    (GPIOA_BASE+16)
#define GPIOB_IDR_Addr    (GPIOB_BASE+16)
#define GPIOC_IDR_Addr    (GPIOC_BASE+16)
#define GPIOD_IDR_Addr    (GPIOD_BASE+16)
#define GPIOE_IDR_Addr    (GPIOE_BASE+16)
#define GPIOF_IDR_Addr    (GPIOF_BASE+16)
#define GPIOG_IDR_Addr    (GPIOG_BASE+16)
#define GPIOH_IDR_Addr    (GPIOH_BASE+16)
#define GPIOI_IDR_Addr    (GPIOI_BASE+16)
#define GPIOJ_IDR_Addr    (GPIOJ_BASE+16)
#define GPIOK_IDR_Addr    (GPIOK_BASE+16)

//IO port operation only works on a single IO port!
//Make sure the value of n is less than 16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  // output
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  // input

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  // output
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  // input

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  // output
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  // input

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  // output
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  // input

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  // output
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  // input

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  // output
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  // input

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  // output
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  // input

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  // output
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  // input

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  // output
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  // input

#define PJout(n)   BIT_ADDR(GPIOJ_ODR_Addr,n)  // output
#define PJin(n)    BIT_ADDR(GPIOJ_IDR_Addr,n)  // input

#define PKout(n)   BIT_ADDR(GPIOK_ODR_Addr,n)  // output
#define PKin(n)    BIT_ADDR(GPIOK_IDR_Addr,n)  // input

//void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq);//Clock system configuration
//The following is the assembly function
//void WFI_SET(void);			// Execute wfi command
//void INTX_DISABLE(void);	//Turn off all interrupts
//void INTX_ENABLE(void);		// Enable all interrupts
//void MSR_MSP(u32 addr);		// Set stack address

#endif

