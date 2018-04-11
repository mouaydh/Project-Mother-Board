/*
Club Robot - Polytech Marseille

Fonctions de gestions des capteur ultra son

Révision:     	$Rev: $
Auteur:       	$Author: $
Date révision: 	$Date:  $
projet:      	$URL: $


*/

#include "init_pic.h"

#include "i2c.h"
#include "moduleI2C.h"

struct Struct_Boussole Boussole;
struct Struct_UltraSon UltraSon;

char mesure_boussole(void){
    char data;
    if(Boussole.newVal_E){
        if(lecture_reg_I2C(0XC0,1,&data) != 1){
            Boussole.Val = data;
            Boussole.ValMean = Boussole.ValMean*0.9 + ((int) data)*0.1;
            Boussole.newVal_E = 0;
            return 1;
        }
    }
    return 0;
}



/*void init_ultrason(void){
	//configuration portee = 150cm
	while(write_reg_i2c(ADDRESS_US_av,0X02,0x22));
//	while(write_reg_i2c(ADDRESS_US_ar,0X02,0x22));
}
 * */

void init_ultrason(unsigned char adresse){
	while(
            write_reg_i2c(adresse,2,0xFF)
            );
    while(
            write_reg_i2c(adresse,1,16)
            );
}

/******************************************************************************/
void mesure_ultrason_1(unsigned char adresse){
	static int etat=0;
	char err=0;
	char test=0;
	static char LSB_data;
	static char MSB_data;
	unsigned int data;
	IdleI2C1();
    
	switch(etat){
		case 0: //debut de la mesure en cm
            UltraSon.FlagU1_VAL = 0;
			err=write_reg_i2c(adresse,0X00,0x51);
			if(err!=1) etat++; // Si pas d'erreur : on continu
			break;
		case 1: //attente de fin de mesure
            UltraSon.FlagU1_VAL = 0;
			err=lecture_reg_I2C(adresse,0x00,&test);
			if(test!=0xFF && err!=1)// Si pas d'erreur et l'uson repond : on continu
				etat++;
			break;
		case 2: //LSB de la mesure 
            UltraSon.FlagU1_VAL = 0;
			err=lecture_reg_I2C(adresse,0x03,&LSB_data);
			if(err!=1) etat++; // Si pas d'erreur : on continu
			break;
		case 3: //MSB de la mesure 
			err=lecture_reg_I2C(adresse,0x02,&MSB_data);
			if(err!=1){// Si pas d'erreur :
				etat=0;  //on recommence
				data=(int) MSB_data;
				data=(data<<8)+ ((int) LSB_data);
                UltraSon.ValU1=data;
                UltraSon.FlagU1_VAL = 1;
			}
			break;
	}//end switch
	UltraSon.FlagU1_ERR = err;
}
/******************************************************************************/
void mesure_ultrason_2(unsigned char adresse){
	static int etat=0;
	char err=0;
	char test=0;
	static char LSB_data;
	static char MSB_data;
	unsigned int data;
	IdleI2C1();
	switch(etat){
		case 0: //debut de la mesure en cm
            UltraSon.FlagU2_VAL = 0;
			err=write_reg_i2c(adresse,0X00,0x51);
			if(err!=1) etat++; // Si pas d'erreur : on continu
			break;
		case 1: //attente de fin de mesure
			err=lecture_reg_I2C(adresse,0x00,&test);
			if(test!=0xFF && err!=1)// Si pas d'erreur et l'uson repond : on continu
				etat++;
			break;
		case 2: //LSB de la mesure 
            UltraSon.FlagU2_VAL = 0;
			err=lecture_reg_I2C(adresse,0x03,&LSB_data);
			if(err!=1) etat++; // Si pas d'erreur : on continu
			break;
		case 3: //MSB de la mesure 
			err=lecture_reg_I2C(adresse,0x02,&MSB_data);
			if(err!=1){// Si pas d'erreur :
				etat=0;  //on recommence
				data=(int) MSB_data;
				data=(data<<8)+ ((int) LSB_data);
                UltraSon.ValU2=data;
                UltraSon.FlagU2_VAL = 1;
			}
			break;
	}//end switch
	UltraSon.FlagU2_ERR = err;
}
/******************************************************************************/
void mesure_ultrason_3(unsigned char adresse){
	static int etat=0;
	char err=0;
	char test=0;
	static char LSB_data;
	static char MSB_data;
	unsigned int data;
	IdleI2C1();
	switch(etat){
		case 0: //debut de la mesure en cm
            UltraSon.FlagU3_VAL = 0;
			err=write_reg_i2c(adresse,0X00,0x51);
			if(err!=1) etat++; // Si pas d'erreur : on continu
			break;
		case 1: //attente de fin de mesure
            UltraSon.FlagU3_VAL = 0;
			err=lecture_reg_I2C(adresse,0x00,&test);
			if(test!=0xFF && err!=1)// Si pas d'erreur et l'uson repond : on continu
				etat++;
			break;
		case 2: //LSB de la mesure 
            UltraSon.FlagU3_VAL = 0;
			err=lecture_reg_I2C(adresse,0x03,&LSB_data);
			if(err!=1) etat++; // Si pas d'erreur : on continu
			break;
		case 3: //MSB de la mesure 
			err=lecture_reg_I2C(adresse,0x02,&MSB_data);
			if(err!=1){// Si pas d'erreur :
				etat=0;  //on recommence
				data=(int) MSB_data;
				data=(data<<8)+ ((int) LSB_data);
                UltraSon.ValU3=data;
                UltraSon.FlagU3_VAL = 1;
			}
			break;
	}//end switch
    UltraSon.FlagU3_ERR = err;
}

