/*
Club Robot - Polytech Marseille

Fonctions du bus I2C

Révision:     	$Rev: $
Auteur:       	$Author: $
Date révision: 	$Date:  $
projet:      	$URL: $

*/
#include "init_pic.h"
#include "i2c.h"
#include <string.h>

#define TEMPO_I2C 4000

void I2C_Init();

/********************************************************************
* Initialise le Bus I2C
*
* entrées: -
* sortie: -
*/
void I2C_Init()
{
// todo
	IPC4bits.MI2C1IP = 5;	// interrupt priority
}


/************************************************************************************************
fonction de configuration I2C
*/
void config_I2C (void)
{
	unsigned int config2, config1;
	config2 = 0x18B;                                           // Baud rate is set for 100 Khz
	config1 = (I2C1_ON &                                       // Enable I2C
  			 I2C1_IDLE_CON & I2C1_CLK_HLD &   
             I2C1_IPMI_DIS & I2C1_7BIT_ADD &
             I2C1_SLW_DIS & I2C1_SM_DIS &
             I2C1_GCALL_DIS & I2C1_STR_DIS &
             I2C1_NACK & I2C1_ACK_DIS & I2C1_RCV_DIS &
             I2C1_STOP_DIS & I2C1_RESTART_DIS &
             I2C1_START_DIS);
	OpenI2C1(config1,config2);
}


char write_i2c(char val){
    int i=0;
	MasterWriteI2C1(val);
    // Verification 8 clock cycles + 9th clock cycle + reception de ack  
/*    while(i<TEMPO_I2C && (I2C1STATbits.TBF || !IFS1bits.MI2C1IF || I2C1STATbits.ACKSTAT)){
        i++;
    }
	if(i>=TEMPO_I2C)*/
    if(I2C1STATbits.TBF || !IFS1bits.MI2C1IF || I2C1STATbits.ACKSTAT)
	{
		StopI2C1();
		return 1;
	}
// ?? ligne présente dans la fonction lecture des étudiants ???
//IFS1bits.MI2C1IF = 0;                  // Clear interrupt flag avant l'aquitement de l'esclave ...
	return 0;
}



/**************************************************************************************************
fonction d'écriture ultrason 
*/
char write_reg_i2c(char addresse, char reg, char data){
	int i;
  StartI2C1();
//Commenter pour test !!!!!
  while(I2C1CONbits.SEN); 
					     
  IFS1bits.MI2C1IF = 0;               // Clear interrupt flag
  
//envoie/ecriture de l'adresse
	if(write_i2c(addresse))	return 1; 		// si erreur return 1
//envoie/ecriture du registre
	if(write_i2c(reg))	return 1; 		// si erreur return 1
//envoie/ecriture de la data
	if(write_i2c(data))	return 1; 		// si erreur return 1

  	StopI2C1();
    //while(I2C1CONbits.PEN);             // Wait till stop sequence is completed
	for(i=0; i<TEMPO_I2C; i++){}; 				// Attende de 10*10µs avec FCY=40MHz
	if(I2C1CONbits.PEN)	return 1;
	else				return 0;
}	
 
 	
 	

/*****************************************************************************************************
fonction de lecture ultrason 
*/

char lecture_reg_I2C(char address, char reg, char *data){
    int i;
    StartI2C1();
    while(I2C1CONbits.SEN);                // Wait till Start sequence is completed
    IFS1bits.MI2C1IF = 0;                  // Clear interrupt flag
 
//envoie/ecriture de l'adresse	
    if(write_i2c(address)==1)	return 1;
//envoie/ecriture du MSB du registre
    if(write_i2c(reg)==1)	return 1;

  //restart
    RestartI2C1();
    while(I2C1CONbits.RSEN);
  
//ecriture de l'adresse lecture							      
    MasterWriteI2C1(address|0x01);         // Write Slave address and set master for reception
    while(I2C1STATbits.TBF);               // Wait till address is transmitted      // 8 clock cycles
    while(!IFS1bits.MI2C1IF);              // Wait for 9th clock cycle
    IFS1bits.MI2C1IF = 0;                  // Clear interrupt flag
    while(I2C1STATbits.ACKSTAT);           //reception de ack de l'esclave 
	
//lecture de la donnée	
    *data=MasterReadI2C1();                // Write string of data	
    while(I2C1STATbits.RBF);			     //wait till data is received
    NotAckI2C1();		                     //ack=1 acquittement du maitre et fin de reception
    
    StopI2C1();
	//while(I2C1CONbits.PEN);             // Wait till stop sequence is completed
	for(i=0; i<TEMPO_I2C; i++){}; 				// Attende de 10*10µs avec FCY=40MHz
	if(I2C1CONbits.PEN)	return 1;
	else				return 0;
 }

/*
char read_multireg_I2C(char address, char reg, char nbReg, char *data){
    int i;
    StartI2C1();
    while(I2C1CONbits.SEN);                // Wait till Start sequence is completed
    IFS1bits.MI2C1IF = 0;                  // Clear interrupt flag
 
//envoie/ecriture de l'adresse	
    if(write_i2c(address)==1)	return 1;
//envoie/ecriture du MSB du registre
    if(write_i2c(reg)==1)	return 1;

  //restart
    RestartI2C1();
    while(I2C1CONbits.RSEN);
  
//ecriture de l'adresse lecture							      
    MasterWriteI2C1(address|0x01);         // Write Slave address and set master for reception
    while(I2C1STATbits.TBF);               // Wait till address is transmitted      // 8 clock cycles
    while(!IFS1bits.MI2C1IF);              // Wait for 9th clock cycle
    IFS1bits.MI2C1IF = 0;                  // Clear interrupt flag
    while(I2C1STATbits.ACKSTAT);           //reception de ack de l'esclave 
	
//lecture de la donnée	
    for(i=0; i<nbReg; i++){
        *(data+1)=MasterReadI2C1();                // Write string of data	
        while(I2C1STATbits.RBF);			     //wait till data is received
        if(i<nbReg-1)
            AckI2C1();
        else
            NotAckI2C1();
    }
    
    StopI2C1();
	//while(I2C1CONbits.PEN);             // Wait till stop sequence is completed
	for(i=0; i<TEMPO_I2C; i++){}; 				// Attende de 10*10µs avec FCY=40MHz
	if(I2C1CONbits.PEN)	return 1;
	else				return 0;
 }
*/


#ifdef ADDRESS_EEPROM
/**********************************************************************************************
fonction écriture I2C dans EEPROM
*/
char write_i2c_EEPROM(char MSB_ad, char LSB_ad, char data)
{
	int i;
	StartI2C1();
	while(I2C1CONbits.SEN); 				// Attente d'une condition de start				     
	IFS1bits.MI2C1IF = 0;               	// Clear interrupt flag
  
//envoie/ecriture de l'adresse	
	if(write_i2c(ADDRESS_EEPROM))	return 1;
//envoie/ecriture du MSB de l'adresse mémoire	
	if(write_i2c(MSB_ad))	return 1;
//envoie/ecriture du LSB de l'adresse mémoire	
	if(write_i2c(LSB_ad))	return 1;
//envoie/ecriture du data	
	if(write_i2c(data))	return 1;

  	StopI2C1();
    //while(I2C1CONbits.PEN);             // Wait till stop sequence is completed
	for(i=0; i<TEMPO_I2C; i++){}; 				// Attende de 10*10µs avec FCY=40MHz
	if(I2C1CONbits.PEN)	return 1;
	else				return 0;
}
	





/**************************************************************************************************
fonction test de lecture
*/
char lecture_I2C_EEPROM(char MSB_ad, char LSB_ad, char *data){
	int i;
	StartI2C1();
	while(I2C1CONbits.SEN);			// Wait till Start sequence is completed
	IFS1bits.MI2C1IF = 0;				// Clear interrupt flag
 


//envoie/ecriture de l'adresse	
	if(write_i2c(ADDRESS_EEPROM))	return 1;
//envoie/ecriture du MSB de l'adresse mémoire
	if(write_i2c(MSB_ad))	return 1;
//envoie/ecriture du LSB de l'adresse mémoire
	if(write_i2c(LSB_ad))	return 1;

/*	IFS1bits.MI2C1IF = 0;
	StartI2C1();
	while(I2C1CONbits.SEN);			// Wait till Start sequence is completed
	IFS1bits.MI2C1IF = 0;			// Clear interrupt flag
*/
  //restart
  //RestartI2C1();
	StartI2C1();
	while(I2C1CONbits.SEN);			// Wait till Start sequence is completed
  while(I2C1CONbits.RSEN);

//envoie/ecriture de l'adresse	
	if(write_i2c(ADDRESS_EEPROM|0x01))	return 1;
	IFS1bits.MI2C1IF = 0;			// Clear interrupt flag
//lecture de la donnée	
	*data=MasterReadI2C1();			// Write string of data	
	while(I2C1STATbits.RBF);		// Wait till data is received
	NotAckI2C1();					// ack=1 acquittement du maitre et fin de reception


	StopI2C1();
	//while(I2C1CONbits.PEN);			// Wait till stop sequence is completed	
	for(i=0; i<TEMPO_I2C; i++){}; 				// Attende de 10*10µs avec FCY=40MHz
	if(I2C1CONbits.PEN)	return 1;
	else				return 0;
	return 0;
 }

 
 	


/**************************************************************************************************
fonction d'écriture d'un double en mémoire EEPROM 
*/
void save_double(double data, int *base){
	char i=0;
	//char arr[sizeof(data)];
	char arr[4];
	char base_arr[2];

	//memcpy(&arr,&data,sizeof(data));
	memcpy(&arr,&data,4);
	
//	for(i=0; i<sizeof(data); i++){
	for(i=0; i<4; i++){
		memcpy(&base_arr,base,2);
		while(write_i2c_EEPROM(base_arr[1],base_arr[0], arr[i]));
		*base=*base+1;
	}
}


/**************************************************************************************************
fonction de lecture d'un double en mémoire EEPROM 
*/
double load_double(int *base){
	char i=0;
	double data;
	//char arr[sizeof(data)];
	char arr[4];
	char base_arr[2];

	for(i=0; i<sizeof(data); i++){
		memcpy(&base_arr,base,2);
		while(lecture_I2C_EEPROM(base_arr[1], base_arr[0], &arr[i]));
		*base=*base+1;
	}

	memcpy(&data,&arr,4);
	


	return data;
}

/**************************************************************************************************
fonction d'écriture d'un int en mémoire EEPROM 
*/
void save_int(int data, int *base){
	char i=0;
	char arr[2];
	char base_arr[2];

	memcpy(&arr,&data,2);
	
	for(i=0; i<2; i++){
		memcpy(&base_arr,base,2);
		while(write_i2c_EEPROM(base_arr[1],base_arr[0],arr[i]));
		*base=*base+1;
	}
}


/**************************************************************************************************
fonction de lecture d'un int en mémoire EEPROM 
*/
int load_int(int *base){
	char i=0;
	int data;
	char arr[2];
	char base_arr[2];

	for(i=0; i<2; i++){
		memcpy(&base_arr,base,2);
		while(lecture_I2C_EEPROM(base_arr[1], base_arr[0], &arr[i]));
		*base=*base+1;
	}

	memcpy(&data,&arr,2);
	


	return data;
}
#endif

