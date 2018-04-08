/*********************************************************************/
/*              Header File for I2C module Library routines          */
/*********************************************************************/
/*
R�vision:     	$Rev: $
Auteur:       	$Author: $
Date r�vision: 	$Date:  $
projet:      	$URL: $
*/
#if defined(__dsPIC30F__)
#include <p30fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__PIC24H__)
#include <p24Hxxxx.h>
#endif



#ifndef I2C_H
#define I2C_H

/* I2C1 module definitions for dsPIC33F and PIC24H devices only */
#ifdef _MI2C1IF

/* List of SFRs for I2C1 module */
/* This list contains the SFRs with default (POR) values to be used for configuring I2C1 module */
/* The user can modify this based on the requirement */

#define I2C1RCV_VALUE               0x0000
#define I2C1TRN_VALUE               0x00FF
#define I2C1BRG_VALUE               0x0000
#define I2C1CON_VALUE               0x0000
#define I2C1STAT_VALUE              0x0000
#define I2C1ADD_VALUE               0x0000

/* I2CCON register Configuration bit definitions */

#define I2C1_ON                     0xFFFF    /*I2C module enabled */
#define I2C1_OFF                    0x7FFF    /*I2C module disabled */

#define I2C1_IDLE_STOP              0xFFFF    /*stop I2C module in Idle mode */
#define I2C1_IDLE_CON               0xDFFF    /*continue I2C module in Idle mode */

#define I2C1_CLK_REL                0xFFFF    /*release clock */
#define I2C1_CLK_HLD                0xEFFF    /*hold clock  */

#define I2C1_IPMI_EN                0xFFFF    /*IPMI mode enabled */
#define I2C1_IPMI_DIS               0xF7FF    /*IPMI mode not enabled */

#define I2C1_10BIT_ADD              0xFFFF    /*I2CADD is 10-bit address */
#define I2C1_7BIT_ADD               0xFBFF    /*I2CADD is 7-bit address */

#define I2C1_SLW_DIS                0xFFFF    /*Disable Slew Rate Control for 100KHz */
#define I2C1_SLW_EN                 0xFDFF    /*Enable Slew Rate Control for 400KHz */

#define I2C1_SM_EN                  0xFFFF    /*Enable SM bus specification */
#define I2C1_SM_DIS                 0xFEFF    /*Disable SM bus specification */

#define I2C1_GCALL_EN               0xFFFF    /*Enable Interrupt when General call address is received. */
#define I2C1_GCALL_DIS              0xFF7F    /*Disable General call address. */

#define I2C1_STR_EN                 0xFFFF    /*Enable clock stretching */
#define I2C1_STR_DIS                0xFFBF    /*disable clock stretching */

#define I2C1_ACK                    0xFFDF    /*Transmit 0 to send ACK as acknowledge */
#define I2C1_NACK                   0xFFFF    /*Transmit 1 to send NACK as acknowledge*/

#define I2C1_ACK_EN                 0xFFFF    /*Initiate Acknowledge sequence */
#define I2C1_ACK_DIS                0xFFEF    /*Acknowledge condition Idle */

#define I2C1_RCV_EN                 0xFFFF    /*Enable receive mode */
#define I2C1_RCV_DIS                0xFFF7    /*Receive sequence not in progress */

#define I2C1_STOP_EN                0xFFFF    /*Initiate Stop sequence */
#define I2C1_STOP_DIS               0xFFFB    /*Stop condition Idle */

#define I2C1_RESTART_EN             0xFFFF    /*Initiate Restart sequence */
#define I2C1_RESTART_DIS            0xFFFD    /*Start condition Idle */

#define I2C1_START_EN               0xFFFF    /*Initiate Start sequence */
#define I2C1_START_DIS              0xFFFE    /*Start condition Idle */

/* Priority for Slave I2C1 Interrupt */

#define SI2C1_INT_PRI_7             0xFFFF
#define SI2C1_INT_PRI_6             0xFFFE
#define SI2C1_INT_PRI_5             0xFFFD
#define SI2C1_INT_PRI_4             0xFFFC
#define SI2C1_INT_PRI_3             0xFFFB
#define SI2C1_INT_PRI_2             0xFFFA
#define SI2C1_INT_PRI_1             0xFFF9
#define SI2C1_INT_PRI_0             0xFFF8

/* Slave I2C1 Interrupt Enable/Disable */

#define SI2C1_INT_ON                0xFFFF
#define SI2C1_INT_OFF               0xFFF7

/* Priority for Master I2C1 Interrupt */

#define MI2C1_INT_PRI_7             0xFFFF
#define MI2C1_INT_PRI_6             0xFFEF
#define MI2C1_INT_PRI_5             0xFFDF
#define MI2C1_INT_PRI_4             0xFFCF
#define MI2C1_INT_PRI_3             0xFFBF
#define MI2C1_INT_PRI_2             0xFFAF
#define MI2C1_INT_PRI_1             0xFF9F
#define MI2C1_INT_PRI_0             0xFF8F

/* Master I2C1 Interrupt Enable/Disable */

#define MI2C1_INT_ON                0xFFFF
#define MI2C1_INT_OFF               0xFF7F

/* Macros to  Enable/Disable interrupts and set Interrupt priority of SI2C1 module*/
#define EnableIntSI2C1                    IEC1bits.SI2C1IE = 1
#define DisableIntSI2C1                   IEC1bits.SI2C1IE = 0
#define SetPriorityIntSI2C1(priority)     IPC4bits.SI2C1IP = (priority)

/* Macros to  Enable/Disable interrupts and set Interrupt priority of MI2C1 module*/
#define EnableIntMI2C1                    IEC1bits.MI2C1IE = 1
#define DisableIntMI2C1                   IEC1bits.MI2C1IE = 0
#define SetPriorityIntMI2C1(priority)     IPC4bits.MI2C1IP = (priority)

/* I2C1 module Converter Function Prototypes */

void AckI2C1(void) __attribute__ ((section (".libperi")));

void CloseI2C1(void) __attribute__ ((section (".libperi")));

void ConfigIntI2C1(unsigned int) __attribute__ ((section (".libperi")));

char DataRdyI2C1(void) __attribute__ ((section (".libperi")));

#define SlavegetcI2C1 SlaveReadI2C1

#define MastergetcI2C1 MasterReadI2C1

unsigned int SlavegetsI2C1(unsigned char *, unsigned int) __attribute__ ((section (".libperi")));

unsigned int MastergetsI2C1(unsigned int length, unsigned char * rdptr, unsigned int i2c_data_wait) __attribute__ ((section (".libperi")));

void IdleI2C1(void) __attribute__ ((section (".libperi")));

void NotAckI2C1(void) __attribute__ ((section (".libperi")));

void OpenI2C1(unsigned int,unsigned int) __attribute__ ((section (".libperi")));

unsigned int MasterputsI2C1(unsigned char *) __attribute__ ((section (".libperi")));

unsigned int SlaveputsI2C1(unsigned char *) __attribute__ ((section (".libperi")));

#define SlaveputcI2C1 SlaveWriteI2C1

#define MasterputcI2C1 MasterWriteI2C1

unsigned char SlaveReadI2C1(void) __attribute__ ((section (".libperi")));

unsigned char MasterReadI2C1(void) __attribute__ ((section (".libperi")));

void SlaveWriteI2C1(unsigned char) __attribute__ ((section (".libperi")));

char MasterWriteI2C1(unsigned char) __attribute__ ((section (".libperi")));

void RestartI2C1(void) __attribute__ ((section (".libperi")));

void StartI2C1(void) __attribute__ ((section (".libperi")));

void StopI2C1(void) __attribute__ ((section (".libperi")));


/************************************************************
Fonctions r�alis�es par le club robotique
*/

//void write_i2c(int, int);
char write_i2c(char);
char write_i2c_EEPROM(char, char, char);
char lecture_I2C_EEPROM(char, char, char *);
/*fonction d'�criture d'un double en m�moire EEPROM */
void save_double(double, int *);
/*fonction de lecture d'un double en m�moire EEPROM */
double load_double(int *);
/* fonction d'�criture d'un int en m�moire EEPROM */
void save_int(int data, int *base);
/*fonction de lecture d'un int en m�moire EEPROM  */
int load_int(int *base);



char write_reg_i2c(char, char, char);
char lecture_reg_I2C(char, char, char *);
//char read_multireg_I2C(char, char, char, char *);
void config_I2C (void);

#endif

/* I2C2 module definitions for dsPIC33F and PIC24H devices only */

#endif    /*I2C_H  */
