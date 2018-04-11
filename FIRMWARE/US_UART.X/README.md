# dsPICeCAN
Firmware des cartes d'interface dsPIC_I2C (carte\interfaceI2C_eCAN\dsPIC_I2C.brd)



## Description

Réalise l'interface entre :
* [US - srf10] Capteurs Ultrason (I2C) 
* [GP2Y2Y0A02] Capteurs IR SHARP (sortie analogique) 20cm->150cm
* [Le bus eCAN]
* [Une sortie UART] @ 57600 baud


## ToDo
* Définir les adresses eCAN des messages envoyés
* Etudier la mise en place de filtrage sur les données des capteurs
* Mettre en place une compilation conditionnelle en fonction des nodes (Node uniquemen pour US, Node US + SHARP, etc.)

## Note importante
La carte dsPIC_I2C.brd utilise le I/O standart pour I2C.
