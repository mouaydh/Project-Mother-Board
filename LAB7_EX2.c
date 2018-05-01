/******************************************************************************
TITLE: RECEPTION ASCII to Binary on LED
AUTHOR:
DESCRIPTION: 
******************************************************************************/
#include <p18f2331.h>
//-- PIC Configuration --------------------------------------------------------
#pragma config OSC = HSPLL	// HS-PLL Enabled => 40MHz
#pragma config WDTEN = OFF	// Watchdog Time Disabled
#pragma config DEBUG=ON		// Background Debugger Enable
//#pragma config MCLRE = ON	// MCLR Enable
#pragma config STVREN = ON	// Stack Overflow Reset Enable
#pragma config LVP = OFF
//----------

char i,bas,haut;  

//-- Low-priority Interrupt ---------------------------------------------------
void low_priority interrupt low_isr(void){
}
//----------

//-- High-priority Interrupt --------------------------------------------------
void interrupt high_isr(void){
    if (PIR1bits.RCIF==1)
    {   PIR1bits.RCIF=0;
        i=RCREG>>2;
        LATB=i;
        LATC=RCREG;
        bas=RCREG&0b00000001;
        haut=RCREG&0b00000010;
        if (bas)
            PORTCbits.RC1=1;
        else PORTCbits.RC1=0;
        if (haut)
            PORTCbits.RC0=1;
        else PORTCbits.RC0=0;
    }
        
        
}
//----------

  
//-- Programme Principal ------------------------------------------------------
void main(void){
    //Paramètres de réglages de l'EUSART
    //Les 3 paramètres suivants sont obligatoires pour l'utilisation de l'EUSART
    RCSTAbits.SPEN=1;
    TRISCbits.TRISC7=1;
    TRISCbits.TRISC6=1;
    //Activation de la réception asynchrone
    TXSTAbits.SYNC=0;
    RCSTAbits.CREN=1;
    //définition du baud rate
    BAUDCONbits.BRG16=1;
    TXSTAbits.BRGH=0;
    SPBRG=42;
    //Autorisation des interruptions
    INTCONbits.GIE=1;
    INTCONbits.GIEL=1;
    RCONbits.IPEN=0;
    //définition de l'interruption en réception
    PIE1bits.RCIE=1;
    IPR1bits.RCIP=1; 
    //définition des LEDs de sortie
    TRISBbits.RB5=0;
    TRISBbits.RB4=0;
    TRISBbits.RB3=0;
    TRISBbits.RB2=0;
    TRISBbits.RB1=0;
    TRISBbits.RB0=0;
    TRISCbits.RC1=0;
    TRISCbits.RC0=0;
    
	while(1){
	}
}
//----------



