/**********************************************************************
* © 2008 Microchip Technology Inc.
*
* FileName:        ECAN1Drv.c
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


#include "ECAN1Drv.h"
#include "node.h"

unsigned int ECANFilterSID(int sid, unsigned long eid, int exide){
	/* This function returns the value to be stored in CxFnSID register. 
	 * SID - The Standard ID
	 * EID - The Extended ID. For Standard messages EID should be zero.
	 * EXIDE - 0 for Standard messages. 1 otherwise
	 */
	unsigned int returnVal;
	
	returnVal = sid << 5;
	if(exide == 1)
		returnVal |= 0x8;
	else
		returnVal &= 0xFFFE;
	returnVal |= (int)(eid >> 16);
	return (returnVal);
}	

unsigned int ECANFilterEID( unsigned long eid){
	/* This function will return value to be stored on CxFnEID register.
	 * EID - this is the extended ID */
	unsigned int returnVal;
	
	returnVal = (int)(eid & 0xFFFF);
	
	return(returnVal);
}	

void ECAN1DMAConfig(int txBufferOffset, int rxBufferOffset, int numOfRxBuffers){
	/* Set up DMA Channel 0 to copy data from DMA RAM to ECAN module 1. The DMA 
     * mode is Register Indirect with Post Increment. The length of each ECAN
     * message is 7 words. Additionally the Continuous mode with ping pong 
     * disabled is used. For ease of error* handling while using register 
     * indirect mode, only one TX buffer is used. */ 
	DMA0CON = 0x2000;		
	DMA0PAD = (int)(&C1TXD);	
 	DMA0CNT = 6;			
	DMA0REQ = 0x0046;		
	DMA0STA = txBufferOffset; 
   	_DMA0IE	= 0;	
   	_DMA0IF = 0;
   	DMA0CONbits.CHEN = 1;		
	
	/* Set up DMA Channel 1 to copy data from ECAN module 1 to DMA RAM. The 
     * Receive memory is treated like a FIFO. The ECAN module cannot 
     * differentiate between buffers when the DMA is in register indirect mode.
	 * Note the size of each recevied message is eight words. Continuous with 
     * pin pong disabled is used.*/
	DMA1CON = 0x0000;		
	DMA1PAD = (int)(&C1RXD);	
 	DMA1CNT = (numOfRxBuffers * 8) - 1;				
	DMA1REQ = 0x0022;	
	DMA1STA = rxBufferOffset; 
    _DMA1IE	= 0;	
   	_DMA1IF = 0;
   	DMA1CONbits.CHEN = 1;		
}    	



void ECAN1ClockConfig(void){
	/* ECAN_FCY and ECAN_BITRATE are defined in ECAN1Drv.h. The total time time
     * quanta per bit is 8. Refer to ECAN FRM section for more details on 
     * setting the CAN bit rate */
	C1CTRL1bits.CANCKS = 1; 
	C1CFG1 = ((ECAN_FCY/16)/ECAN_BITRATE)-1;  
 	C1CFG2 = 0x0290; 	
} 
 

void ECAN1InterruptConfig(void){
	/* Only the C1 Event Interrupt is used in this code example. All the status 
	 * flags are cleared before the module is enabled */
	C1INTF = 0;
	_C1IF = 0;
	_C1TXIF = 0;
	_C1RXIF = 0;
	_C1IE = 1;
	_C1TXIE = 0;
	_C1RXIE = 0;
	C1INTE = 0x00FF;
	C1RXFUL1 = 0;
	C1RXFUL2 = 0;
	C1RXOVF1 = 0;
	C1RXOVF2 = 0;
}


void ECAN1TxRxBuffersConfig(void){
	/* Configure only one TX buffer and enable four DMA buffers. No need to 
     * configure  RX buffers. */
	C1CTRL1bits.WIN = 0;
	C1TR01CONbits.TXEN0 = 1;
}


void ECAN1RxFiltersConfig(void ){
	/* Enable 2 filters. The ID are defined in node.h Note that using Register 
     * Indirect mode does not  affect the filter and masking capability. You may 
	 * not want to set up the CxBUFPNTn bits to point to FIFO. This way you can 
     * avoid the FIFO interrupts. Just point to an arbitrary RX buffer */
	
	/* In this case filters 0 and 4 are used */
	 C1CTRL1bits.WIN = 1;
	 C1FEN1 = 0x11;
	
	 C1RXF0SID = ECANFilterSID(SID1,EID1,1);		
	 C1RXF0EID = ECANFilterEID(EID1);
	 	 
	 C1RXF4SID = ECANFilterSID(SID2,EID2,1);		
	 C1RXF4EID =ECANFilterEID(EID2);
	 	 
	 C1RXM0SID = 0xFFEB;		/* Configure MASK 0 - All bits used in comparison*/
	 C1RXM0EID = 0xFFFF;
	 
	 C1FMSKSEL1bits.F0MSK = 0x0;	/* User MASK 0 for all filters */
	 C1FMSKSEL1bits.F4MSK = 0x0;	/* User MASK 0 for all filters */
	 
	 C1BUFPNT1bits.F0BP = 0x1;		/* Set the destination buffers to be any thing but */
	 C1BUFPNT2bits.F4BP = 0x1;		/* configured transmit buffers */
}

	

int ECAN1SendPacket(void){
	/* The TXREQ bit is set. This will make the ECAN module generate request to 
     * the DMA. Note that in Register Indirect Mode the ECAN module is not aware
     * of buffers. So setting the TXREQ bit will cause data be transferred from 
     * the location pointed by the DMA */
	 
	/* The DMA Pointer will have to be reset in case arbitration is lost. This 
     * can be done by checking the TXLARB bit in the CxTRmnCON register. The DMA
	 * Pointer is reset by disabling and enabling the DMA channel.*/
	 
	/* If the message transmission is aborted the function will return a zero.*/
	 
	C1TR01CONbits.TXREQ0 = 1;
	while(C1TR01CONbits.TXREQ0 == 1){
		if(C1TR01CONbits.TXLARB0 == 1){
			/* Arbitration lost. Abort the message and reset the transmit buffer
             * DMA */
	
			C1TR01CONbits.TXREQ0 = 0;
			DMA0CONbits.CHEN = 0;
			DMA0CONbits.CHEN = 1;
			C1TR01CONbits.TXREQ0 = 1;
		}
	}
	
	if(C1TR01CONbits.TXABT0 == 1){
		return(0);
	}
	
	return(1);			
}	


void ECAN1SetOPMode(void){
	/* Request the normal operational mode and wait  till it is set */
	C1CTRL1bits.REQOP = 0;
	while(C1CTRL1bits.OPMODE != 0);
}


void ECANCreateSIDPacket(int * data, int sid,int * output){
	/* This function will create a standard ID packet DLC is set to 8. 
	 * data - points to the data payload
	 * SID - Standard ID value to be used. 
	 * output - points to where the packet should be stored which typically is 
     * an ECAN TX buffer */
	 
	 //output[0] = (sid << 2) & 0x1FFC;		/* SID, SRR = 0 and IDE = 0 */
	 //output[1] = 0x8;
	 //output[2] = data[0];
	 //output[3] = data[1];
	 //output[4] = data[2];
	 //output[5] = data[3];
	 //output[6] = 0;
     
     output[0] = (sid << 2) & 0x1FFC;		/* SID, SRR = 0 and IDE = 0 */
     output[1] = 0x0;//(int)(eid >> 6) & 0x0FFF;
	 output[2] = 0x8;
	 output[3] = data[0];
	 output[4] = data[1];
	 output[5] = data[2];
	 output[6] = data[3];
}  

void ECANCreateEIDPacket(int * data,int sid,long eid,int * output){
	/* This function will create a extended ID packet DLC is set to 8. 
	 * data - points to the data payload
	 * sid - standard id
	 * eid - extended id
	 * output - points to where the packet should be stored which typically is 
     * an ECAN TX buffer */
	 
	 output[0] = (sid << 2) | 0x3;		
	 output[1] = (int)(eid >> 6) & 0x0FFF;
	 output[2] = ((int)(eid & 0x3F) << 10) | 0x8;
	 output[3] = data[0];
	 output[4] = data[1];
	 output[5] = data[2];
	 output[6] = data[3];	
}
                         
