#include "init_pic.h"

void init_pic(void) {
/***INITIALISATION DE LA CLOCK  ************************************************/
    //Initialisation de la clock
    PLLFBD = PLL_M;
    CLKDIVbits.PLLPOST = PLL_N1;    // N1 = 2
    CLKDIVbits.PLLPRE = PLL_N2;     // N2 = 2
    OSCTUN = 0;                     // Tune FRC oscillator, if FRC is used
    RCONbits.SWDTEN = 0;            // Disable WatchDog Timer
    RCON = 0; // aucune erreur ne gener de RESET
// Clock switch to incorporate PLL
    __builtin_write_OSCCONH(FTYPE); // Initiate Clock Switch to
                                    // Primary oscillator (XT, HS, EC) with PLL (NOSC=0b011)
    __builtin_write_OSCCONL(0x01);  // Start clock switching
    while (OSCCONbits.COSC != FTYPE);	// Wait for Clock switch to occur
    while(OSCCONbits.LOCK != 1);
/******************************************************************************/

/***ASSIGNATION DES PIN *******************************************************/    
    RPINR18bits.U1RXR 	= 	21;  	// assigne RX de UART1 sur RP21
	RPOR10bits.RP20R	= 0b00011 ;	// assigne TX de UART1 sur RP20 (0b00011<=> U1TX)
    RPOR2bits.RP4R = 0b10000;       // assigne TX de ECAN1 sur RP4  (0b10000<=> CAN_TX)
    RPINR26bits.C1RXR	= 	19;  	// assigne RX de ECAN1 sur RP19
/******************************************************************************/
    
/***INITIALISATION PORT I/O ***************************************************/
	LED1_TRIS = 0;
	LED2_TRIS = 0;
	LED3_TRIS = 0;
	LED4_TRIS = 0;
    LED5_TRIS = 0;
    LED8_TRIS = 0;
    LED8 = LED_ON;
	LED1 = LED_OFF; 	
	LED2 = LED_OFF;	
	LED3 = LED_OFF;	
	LED4 = LED_OFF;	
    
    LED5 = LED_ON;	
/******************************************************************************/
	INTCON1bits.NSTDIS=0;          // Autorisation des nested interruptions
	INTCON2bits.ALTIVT=0;          // standard vector table
/******************************************************************************/
}
