/**********************************************************************
* © 2008 Microchip Technology Inc.
*
* FileName:        node.h
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

#ifndef _NODE_H_
#define _NODE_H_

/* Set the number of total nodes in the system */
#define NUM_OF_NODES 2
#define NUM_OF_TX_NODES (NUM_OF_NODES - 1)

/* Define a set of node addresses. to be used by the node. */
#define NODE1
/* Change this #define to NODE1 for node 1, NODE2 for node 2 and so on */

/* The SIDx and EIDx values are the  IDs of the local node. The TXSIDx and 
 * TXEIDx value are the ID of nodes to which messages will be sent */
#ifdef NODE1
#define SID1 0x91			/* This is node 1 address */
#define EID1 0x1C490
#define SID2 0x90			/* This is node 1 address */
#define EID2 0x1C490
#define TXSID1 0x92			/* This is node 2 address */
#define TXEID1 0x1C490
#define RANDOM_SEED	20
#endif

#ifdef NODE2
#define SID1 0x92			/* This is node 2 address */
#define EID1 0x1C490
#define SID2 0x90			/* This is node 2 address */
#define EID2 0x1C490
#define TXSID1 0x91			/* This is node 1 address */
#define TXEID1 0x1C490
#define RANDOM_SEED 30
#endif

#endif

