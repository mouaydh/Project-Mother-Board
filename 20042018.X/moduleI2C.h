/*
Club Robot - Polytech Marseille

Fonctions de gestions des capteur ultra son

Révision:     	$Rev: $
Auteur:       	$Author: $
Date révision: 	$Date:  $
projet:      	$URL: $


*/


#ifndef _MODULEI2C_H_
#define _MODULEI2C_H_

extern struct Struct_Boussole Boussole;
extern struct Struct_UltraSon UltraSon;

struct Struct_Boussole{
	unsigned char  Val;
	int ValMean;
	//unsigned char  newVal_F:1;
	unsigned char  newVal_E;
};

struct Struct_UltraSon{
	unsigned char FlagU1_ERR;
    unsigned char FlagU2_ERR;
    unsigned char FlagU3_ERR;
    unsigned char FlagU1_VAL;
    unsigned char FlagU2_VAL;
    unsigned char FlagU3_VAL;
	unsigned int ValU1;
    unsigned int ValU2;
    unsigned int ValU3;
};

char mesure_boussole(void);

//void init_ultrason(void);
void init_ultrason(unsigned char);
/*
init_ultrason permet d'initialiser les module OEM "MSU10".
Il regle simlement la porte des module a 1m50
*/

void mesure_ultrason_1(unsigned char);
void mesure_ultrason_2(unsigned char);
void mesure_ultrason_3(unsigned char);
/*
mesure_ultrason_1 permet d'effectuer la mesure de distance
grace au module OEM "MSU10".

 * adresse : adresse du module
*/
//char mesure_ultrason_2(char, char *);
#endif

