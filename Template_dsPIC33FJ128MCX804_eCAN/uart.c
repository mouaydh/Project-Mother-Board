#include "init_pic.h"	// processor, constantes,
#include "uart.h"
#include <ctype.h>
#include <stdio.h>      // for printf
#include <stdlib.h>     // for atoi
#include <string.h>

struct Struct_Uart1 Uart1;

void initUART1(unsigned long baudrate) {
	#ifndef FCY	// test si FCY est definit (cf init_pic.h)
	// FCY pas definit, declenche une erreur de compilation car on ne peut pas
    // calculer le baudrate
	#error "unknow constant FCY"
	#endif

    Uart1.Tx_size = sprintf(Uart1.Tx_string, "Test_Uart1\n");
    Uart1.Flag &= FLAG_Tx_MASKB; // Information en cours d'envoie

	// Configuration de l'UART 
    U1BRG =  ((FCY/baudrate)/4)-1;	// Calcule du baud rate en mode High Speed
    U1MODEbits.BRGH = 1;	// Selection du mode High speed Baud Rate
	// configure U1MODE
	U1MODEbits.STSEL = 0;	// Un bit de Stop
	U1MODEbits.PDSEL = 0;	// 8bit, No Parity
	U1MODEbits.ABAUD = 0;	// No Autobaud (would require sending '55')
	U1MODEbits.RTSMD = 0;	// Simplex Mode
	U1MODEbits.LPBACK = 0;	// No Loop Back
    U1MODEbits.UEN = 0;		// activation de TX,RX ; les signaux CTS,RTS ne sont pas active
	U1MODEbits.WAKE = 0;	// No Wake up (pa de reveil vias Tx)
	U1MODEbits.IREN = 0;	// No IrDA translation
	U1MODEbits.USIDL = 1;	// Arret du mode en mode sandby (disContinue in Idle)
	U1MODEbits.URXINV = 0;	// 0 = Etat de repot de laligne UxRX a 1
	// configure U1STA
	U1STAbits.URXISEL0 = 0;	// Interrupt Reception mode : character recieved
	U1STAbits.URXISEL1 = 0;
	U1STAbits.UTXISEL1 = 1; //Interrupt de transmition quand :
    U1STAbits.UTXISEL0 = 0; // Un caractere est transferre au registre a decalage
                            // et que le buffer de transmission devient vide
	U1STAbits.UTXEN = 1;	// TX pins controlled by uart 0
	U1STAbits.UTXBRK = 0;	// Disabled
	U1STAbits.UTXISEL1 = 0;	//Bit15 Int when Char is transferred (1/2 config!)
	U1STAbits.UTXINV = 0;	//
	U1STAbits.ADDEN = 0;	// Address Detect Disabled

    
	// Interrupts : Reception
	IPC2bits.U1RXIP = 1;	// interrupt priority 3
    IFS0bits.U1RXIF = 0; 	// Clear the Recieve Interrupt Flag
	IEC0bits.U1RXIE = 1; 	// Enable Recieve Interrupts 1
	// Interrupts : Transmission
	IPC3bits.U1TXIP = 1;	// interrupt priority 2
    IFS0bits.U1TXIF = 0; 	// Clear the Transmission Interrupt Flag
	IEC0bits.U1TXIE	= 1;	// // Enable Transmisssion Interrupts 1
	
	U1MODEbits.UARTEN = 1;	// And turn the peripheral on
	U1STAbits.UTXEN = 1;	// UART1 transmitter enabled;
    
}


// routine d'interruption sur reception UART1
void __attribute__ ((interrupt, no_auto_psv)) _U1RXInterrupt(void) {	
	char UxRx_char;
	static int i=0;
	IFS0bits.U1RXIF = 0;	// acquittement interruption
    
	UxRx_char = U1RXREG; // get the data 
    
	if(U1STAbits.FERR == 1){// check for receive errors
		UxRx_char='b';
		U1STAbits.FERR = 0;
	}
	
	if(U1STAbits.OERR == 1){// must clear the overrun error to keep uart receiving 
		UxRx_char='a';
		U1STAbits.OERR = 0;
	}
    
	switch (isspace(UxRx_char)){
		case 1:
			// assig_char(UxRx_string); -> Mot recu => pres a etre analyse
			for(i=0; i<UxRx_length; i++) Uart1.Rx_string[i]=0; // nettoyage de la chaine de caractére
			i=0;
			break;
		default :
			Uart1.Rx_string[i]=UxRx_char;
			i++;
			break;
	} 
}

void startU1TX(void){
	U1TXREG=0;			// Reveille de la ligne pas l'envoie d'un 0. L'envoie des donnés est géré en interruption
}


/*******************************************************************************
* routine d'interruption transmission UART1 (buffer vide)
*/
void __attribute__ ((interrupt, no_auto_psv)) _U1TXInterrupt(void){
	static int i = 0;
	IFS0bits.U1TXIF = 0;            // acquittement IT
    while(U1STAbits.UTXBF==0 && i<Uart1.Tx_size){
        U1TXREG = Uart1.Tx_string[i];   // Transmition d'un caractere a la FIFO
        i++;
    }
	if(i>=Uart1.Tx_size){           // si tous les caracteres ont ete envoye :
        IEC0bits.U1TXIE	= 0;    // Deactivation des interruptions de transmission
		Uart1.Flag |= FLAG_Tx_MASK; // Fin de l'envoie
		i=0;
	}
}

void fctU1Tx_int(int Data){
	char str[12];
	while((Uart1.Flag & FLAG_Tx_MASK) ==0); // attente liberation de l'UART1
    Uart1.Tx_size=sprintf(str, "%d", Data)+1;
    strcat(str,"\n");
    strcpy(Uart1.Tx_string,str);
    Uart1.Flag &= FLAG_Tx_MASKB; // Positionnement du Flag : Information en cours d'envoie
    IEC0bits.U1TXIE	= 1;//Enable Transmisssion Interrupts 1
}

void  fctU1Tx_string(char string[UxTx_length],char size){
		while((Uart1.Flag & FLAG_Tx_MASK) ==0); // attente liberation de l'UART1
        Uart1.Tx_size=size;
		strcpy(Uart1.Tx_string,string);
		Uart1.Flag &= FLAG_Tx_MASKB; // Positionnement du Flag : Information en cours d'envoie
		IEC0bits.U1TXIE	= 1;//Enable Transmisssion Interrupts 1
}





/********************************************************************************
interruption UART1 error 
*/
/*
void __attribute__ ((interrupt, no_auto_psv)) _U1ErrInterrupt(void)
{
	IFS bits.
}
*/
