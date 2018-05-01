/******************************************************************************
TITLE: 
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

char greeting[8] = {'S', 'a', 'l', 'u', 't','!','\n', '\0'};
  int i=0;
  
//-- Low-priority Interrupt ---------------------------------------------------
void low_priority interrupt low_isr(void){
}
//----------

//-- High-priority Interrupt --------------------------------------------------
void interrupt high_isr(void){
    
      if (INTCONbits.INT0IF==1)
    { 
         
        {INTCONbits.INT0IF=0;
        TXREG= greeting[i];
        i++;
        if (i==7)
        i=0;
        }
    }
}
//----------

  
//-- Programme Principal ------------------------------------------------------
void main(void){

    RCSTAbits.SPEN=1;
    TRISCbits.TRISC7=1;
    TRISCbits.TRISC6=1;
    TXSTAbits.SYNC=0;
    TXSTAbits.TXEN=1;
    BAUDCONbits.BRG16=1;
    TXSTAbits.BRGH=0;
    SPBRG=42;
    TRISCbits.RC3=1;
    INTCONbits.GIE=1;
    INTCONbits.INT0IE=1;
    RCONbits.IPEN=1;
   
            
	while(1){
	}
}
//----------



