/**********************************************************************
* © 2008 Microchip Technology Inc.
*
* FileName:        main.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       dsPIC33Fxxxx
* Compiler:        MPLAB® C30 v3.00 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
************************************************************************/

/* The code example uses LEDs to indicate status
 * LED1 - Message Transmitted
 * LED2 - Message Received
 * LED3 - CAN BUS Error
 * LED4 - Data Payload CRC check error */
 
#include <stdio.h>
#include <stdlib.h> // stdlib.h is included for the rand() and srand() function call
#ifdef __dsPIC33FJ128MC804__
#include <p33fj128mc804.h>
#endif

#include "ECAN1Drv.h"
//#include "crc16.h"
#include "init_pic.h"
#include "node.h"

#include "uart.h"

_FOSCSEL(FNOSC_PRIPLL); // Primary Oscillator (XT, HS, EC) w/ PLL
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF  & POSCMD_XT);
    //FCKSM_CSECMD  Clock switching is enabled, Fail-Safe Clock Monitor is disabled
    //OSCIOFNC_OFF  OSC2 pin has clock out function
    //POSCMD_XT     XT Oscillator Mode
_FWDT(FWDTEN_OFF);
    // FWDTEN_OFF   Watchdog timer enabled/disabled by user software
//Configuration pin I2C
_FPOR(ALTI2C_ON);

/* Define the number of ECAN RX buffers. Only one TX buffer is used. This is 
 * explained in the readme file along with this code example. */
#define NUM_OF_RX_BUFFERS 8

/* Create the message arrays. Note the size of the TX Message. It is 7 (not 8)*/
int ecanTxMsgBuffer[7] __attribute__((space(dma)));
int ecanRxMsgBuffer[NUM_OF_RX_BUFFERS][8] __attribute__((space(dma)));

/* The received flag is checked in the main loop to compute CRC on the received
 * message and flags errors. The transmitNext flag  transmits the next packet.
 * The sentPacket  variable tracks the number of packets sent.*/
volatile int received = 0;
volatile int transmitNext = 1;
int sentPacket = 0;

/* In Register Indirect mode, the ECAN/DMA cannot differentiate between buffers.
 * The application must keep track of where the received message was stored */
volatile int rxBufferIndex = -1;
int rxIndex;
int txBufferIndex = 0;


void RandomWait(void){
	/* This function will wait for a randon period
	 * of time before exiting. The rand() and srand() functions
	 * used for generating the random numbers */
	
	unsigned long wait;
	wait = rand();
	srand(wait);
    wait *=16;
	while(wait > 0){
		wait--;
	}
}

int main(void){
    char sizeU1Tx;
    int data[4];
//	int crcval;
    char strU1Tx[UxTx_length];

    
	/* Configure the dsPIC */
	init_pic();
    initUART1(57600);//9600, 19200, 57600
    
	ECAN1DMAConfig(__builtin_dmaoffset(ecanTxMsgBuffer),
					__builtin_dmaoffset(ecanRxMsgBuffer), 
					NUM_OF_RX_BUFFERS);
	ECAN1ClockConfig();
	ECAN1InterruptConfig();
	ECAN1RxFiltersConfig();
	ECAN1TxRxBuffersConfig();
	ECAN1SetOPMode();

	while(1){
        if(transmitNext == 1){
			/* Send a new packet. Create the packet using
             * data structure and the SID and EID.*/ 
			data[0] = 1;
			data[1] = 2;
			data[2] = 3;
			data[3] = 4;
			ECANCreateEIDPacket(data,TXSID1,TXEID1,ecanTxMsgBuffer);
			transmitNext = 0;
		}	

		/* Wait for a random period of time and then send the message. This code
         * example does not check if the message was aborted but that could be 
         * checked here. Note that if the message was aborted the TBIF flag will
         * not be set and you will not get the associated event interrupt. */
		RandomWait();
		ECAN1SendPacket();
				
		if(received == 1){
			received = 0;
			rxIndex = rxBufferIndex;
            sizeU1Tx=sprintf(strU1Tx, "%d\t%d\t%d\t%d\t%d%d%d%d\n", rxIndex,
                    ecanRxMsgBuffer[rxIndex][0],ecanRxMsgBuffer[rxIndex][1],
                    ecanRxMsgBuffer[rxIndex][2],ecanRxMsgBuffer[rxIndex][3],
                    ecanRxMsgBuffer[rxIndex][4],ecanRxMsgBuffer[rxIndex][5],ecanRxMsgBuffer[rxIndex][6]);
            fctU1Tx_string(strU1Tx,sizeU1Tx);
		}
	}
}

void __attribute__((__interrupt__,no_auto_psv)) _C1Interrupt(void){
	/* Only the Transmit Message, Receive Message Events and Error flags are 
     * processed. You can check for the other events as well.*/
	
	_C1IF = 0;
	
	if(C1INTFbits.TBIF == 1){
		/* Transmit done. Set the flag so that the main application loop knows
         * that the message was transmitted */
		C1INTFbits.TBIF = 0;
		transmitNext = 1;
		LED1 != LED1;
		T1CONbits.TON = 1;	
	}
	
	if(C1INTFbits.RBIF == 1){
		/* Received a packet. Check if the rxBufferIndex is at the boundary
		 * and reset to initial condition if needed. 
		 * You can check the RXFUL flags if needed. */
		if(rxBufferIndex == (NUM_OF_RX_BUFFERS - 1))
			rxBufferIndex = -1;	
		
		C1INTFbits.RBIF = 0;
		C1RXFUL1 = 0;
		received = 1;
		rxBufferIndex ++;
		LED2 != LED2;
		T1CONbits.TON = 1;	
	}
		
	LED3 = LED_OFF;
	if(C1INTFbits.ERRIF == 1){
		C1INTFbits.ERRIF = 0;
		LED3 = LED_ON;
	}
}	