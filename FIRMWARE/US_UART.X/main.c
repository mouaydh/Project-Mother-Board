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
#include "init_pic.h"
#include "node.h"

#include "uart.h"
#include "moduleI2C.h"
#include "i2c.h"
_FOSCSEL(FNOSC_PRIPLL); // Primary Oscillator (XT, HS, EC) w/ PLL
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF  & POSCMD_XT);
    //FCKSM_CSECMD  Clock switching is enabled, Fail-Safe Clock Monitor is disabled
    //OSCIOFNC_OFF  OSC2 pin has clock out function
    //POSCMD_XT     XT Oscillator Mode
_FWDT(FWDTEN_OFF);
    // FWDTEN_OFF   Watchdog timer enabled/disabled by user software
/* Configuration pin I2C
 * !! ATTENTION !!
 * Les cartes d'interface eCAN/I2C/UART/An fonctionnent avec les pin standards
 * pour IC2.
 * Le shield I2C pour Ultrason + Boussol + Centrale Inertielle utilise les pin
 * alternative pour I2C
 */
//_FPOR(ALTI2C_ON); 

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

unsigned int dataa = 0;
unsigned char ADC_Flag_VAL=0;

unsigned int waitUltraSon = 0;

int main(void){
    char sizeU1Tx;
    int data[4];
//	int crcval;
    char strU1Tx[UxTx_length];

    
	/* Configure the dsPIC */
	init_pic();
    initUART1(57600);//9600, 19200, 57600
    #ifdef NODEUS
    config_I2C();
    init_ultrason(0xE0);
    #endif
	ECAN1DMAConfig(__builtin_dmaoffset(ecanTxMsgBuffer),
					__builtin_dmaoffset(ecanRxMsgBuffer), 
					NUM_OF_RX_BUFFERS);
	ECAN1ClockConfig();
	ECAN1InterruptConfig();
	ECAN1RxFiltersConfig();
	ECAN1TxRxBuffersConfig();
	ECAN1SetOPMode();

    
    
    AD1PCFGL = 0b111111110; //port an1 en mode analogue *****************************
    //timler config
    T2CONbits.T32 = 0b0;//16bits
    T4CONbits.T32 = 0b0;//16 bits
    T3CONbits.TCKPS = 0b00; //prescale à 1
    T3CONbits.TCS = 0b0; //internal clock
    T3CONbits.TGATE = 0b0; //gate acccumulation desactiver
    //PR3 = 103;  // 104 pour compter à 192k 0b000000001101000 (50ns)
    PR3 = 19999;  // 20000            à   1k 

    //adc config
    //bits.CH0SB0 = 0b00100; //AN4 input chan 0***************************************
    AD1CON2bits.VCFG = 0b111; //alim avdd et avss (vref plus atrd))
    AD1CON3bits.ADCS = 0b00000000; //adc clock convertion
    AD1CON2bits.CHPS = 0b00; //choisis channel CH0***************************************
    AD1CON1bits.SSRC = 0b010; //stimer 3 trigger adc
    AD1CON3bits.SAMC = 0b0000; //0b1110 = 14 TAD
    AD1CON1bits.FORM = 0b00; //format unsign integer
    AD1CON1bits.AD12B = 0b1; //converyion 12 bits
    AD1CON1bits.ASAM = 0b1; // adc sample auto
    AD1CON2bits.SMPI = 0b0000; //incremente adresse apr&s chasue convertion
    AD1CON1bits.ADDMABM = 0b1;    
    
    //enable interupt
    //IEC0bits.T3IE = 0b1; //timer3 interupt enable
    IEC0bits.AD1IE = 0b1; // Enable Analog-to-Digital interrupt

    //clear flag interupt
    //IFS0bits.T3IF = 0b0; 
    IFS0bits.AD1IF = 0b0;
    
    //allumage
    T3CONbits.TON = 0b1;//T3 ON
    AD1CON1bits.ADON = 0b1; //ADC ON

    TRISBbits.TRISB15 = 0b0; //rb15 en sortit
    
    
    
    
	while(1){
        #ifdef NODEUS
        if(waitUltraSon >1000){
            waitUltraSon=0;
        if(UltraSon.FlagU1_VAL == 0){
            mesure_ultrason_1(0xE0);
        }
        }
        //if(UltraSon.FlagU2_VAL == 0)
        //    mesure_ultrason_2(0xE0);
        //if(UltraSon.FlagU3_VAL == 0)
        //    mesure_ultrason_3(0xE0);
        
        //if(ADC_Flag_VAL==1){
        if(UltraSon.FlagU1_VAL==1){
            sizeU1Tx=sprintf(strU1Tx, "Tx:%d:%d\n", UltraSon.ValU1, dataa);
            fctU1Tx_string(strU1Tx,sizeU1Tx);
            
            
            if(transmitNext == 1){
			/* Send a new packet. Create the packet using
             * data structure and the SID and EID.*/ 
                data[0] = UltraSon.ValU1;
                data[1] = dataa;
                data[2] = 3;
                data[3] = 4;
                //ECANCreateEIDPacket(data,TXSID1,TXEID1,ecanTxMsgBuffer);
                transmitNext = 0;
                //RandomWait();
                //ECAN1SendPacket();
            }
            UltraSon.FlagU1_VAL = 0;
            ADC_Flag_VAL=0;
		}
        #endif

        #ifdef NODE1
		if(received == 1){
			received = 0;
			rxIndex = rxBufferIndex;
            sizeU1Tx=sprintf(strU1Tx, "%d\t%d\t%d\t%d\t%d%d%d%d\n", rxIndex,
                    ecanRxMsgBuffer[rxIndex][0],ecanRxMsgBuffer[rxIndex][1],
                    ecanRxMsgBuffer[rxIndex][2],ecanRxMsgBuffer[rxIndex][3],
                    ecanRxMsgBuffer[rxIndex][4],ecanRxMsgBuffer[rxIndex][5],ecanRxMsgBuffer[rxIndex][6]);
            fctU1Tx_string(strU1Tx,sizeU1Tx);
		}
        #endif
        
        
        #ifdef NODE2
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
        #endif
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



void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt (void) {
    long int disValues = 205050;
    int tempADC;
    /* Routine d'interruption de ADC
     * Permet la convertion sur AN0 de la donnée de distance provenant du 
     * capteur SHARP2Y0A02 pouvant effectuer des mesure de 15cm à 150cm
     * Les donnes constructeurs donnent:
     * Distance(mm) Tension(V) ADC1BUF0 205050/ADC1BUF0-38.713 (mm)
     * 155          2.7506702  853          202
     * 207          2.5227883  782          223
     * 299          2          620          292
     * 398          1.538874   477          390
     * 500          1.2466488  386          491
     * 600          1.0455765  324          594
     * 700          0.8981233  278          696
     * 800          0.8150134  252          772
     * 901          0.71313673 221          889
     * 1001         0.64343166 199          987
     * 1100         0.58981234 183          1082
     * 1198         0.536193   166          1197
     * 1302         0.49597856 153          1293
     * 1401         0.45040214 139          1426
     * 1501         0.42091152 130          1527
     * 
     * Dans la pratique des testes ont monte que la conversion entre la valeur
     * provenant de l'ADC et la distance n'allait pas ... une correction a ete
     * apporte 716196/ADC1BUF0 valeur en mm
     * 
     * Il est important d'avoir en tete que la mesure peut etre perturbe par le 
     * déplacement du robot (cf datasheet)
     */
    waitUltraSon ++;
  	IFS0bits.AD1IF = 0;	// Clear interrupt flag 
    tempADC = ADC1BUF0; // Read POT value to set Reference Speed 
    disValues = 716196/((long int) tempADC);
  	dataa = (int) disValues; // Read POT value to set Reference Speed 
    ADC_Flag_VAL = 1;
    PORTBbits.RB15 = LATBbits.LATB15 ^ 0b1;
    }  