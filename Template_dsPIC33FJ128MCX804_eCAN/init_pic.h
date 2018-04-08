#ifndef _HEADER_H_
#define _HEADER_H_

#ifdef __dsPIC33FJ128MC804__
#include <p33fj128mc804.h>
#endif


#include <libpic30.h>

#define LED_ON 1
#define LED_OFF 0
#define FQUARTZ     4000000	// 4 MHz
#define PLL_N1      0	// 2 	(0.8M<FQuartz/N1<8M)
#define PLL_M       78	// 80 	(100M<FQuartz/N1*M<200M)		//  todo voir si MUL_40 définit dans .h
                        // PLL_M=38, soit FCY=20MHz
                        // PLL_M=78, soit FCY=40MHz
#define PLL_N2      0	// 2	(12.5M<FQuartz*M/(N1*N2)<80M)
				// FOSC=80MHz
				// FCY=40MHz
#define FTYPE       0b011 // Primary oscillator (XT, HS, EC) with PLL (NOSC=0b011)
#define FOSC        (FQUARTZ/(2*2))*80	//(FQUARTZ/(N1*N2))*M
#define FCY         FOSC/2

/* The LEDS and their ports	*/
#define LED1_TRIS		_TRISB15
#define LED2_TRIS		_TRISB14
#define LED3_TRIS		_TRISB13
#define LED4_TRIS		_TRISB12
#define LED5_TRIS		_TRISB11

#define LED8_TRIS		_TRISC8


#define LED1            _LATB15//_RB15
#define LED2            _LATB14//_RB14
#define LED3            _LATB13//_RB13
#define LED4            _LATB12//_RB12
#define LED5            _LATB11//_RB11

#define LED8            _LATC8//_RC8

#define LED1_LAT		_LATB15
#define LED2_LAT		_LATB14
#define LED3_LAT		_LATB13
#define LED4_LAT		_LATB12
#define LED5_LAT		_LATB11

#define LED8_LAT		_LATC8

void init_pic(void);


#endif
