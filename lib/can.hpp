/*
 * Classe permettant l'acces au convertisseur analogique/numerique
 * du microcontroleur ATMega16 de Atmel.
 *
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Cours inf1995
 *
 * Matthew Khouzam et Jerome Collin
 * 2005-2006
 *
 * Code qui n'est sous aucune license.
 *
 */
/*
 
Travail : TP7 et TP8, fichier can.hpp*

Section # : 03

Equipe # 5456

Auteurs: Mohamed-Adam Ezzarouali, Noé Julien, Ranya Anis et Rym Zidi.

Description du fichier :
Ce fichier contient la définition de la classe can, qui permet d’accéder
au convertisseur analogique-numérique (CAN) du microcontrôleur ATmega16.
Elle offre les méthodes nécessaires pour initialiser le CAN et effectuer
des lectures analogiques sur les broches du port A.*

Identifications matérielles :
Le convertisseur analogique-numérique utilise les broches du PORTA (A0 à A7).
La conversion renvoie une valeur numérique sur 10 bits correspondant au signal analogique mesuré.

Correcteur: Paul Petibon

*/

#ifndef CAN_H
#define CAN_H

#include <avr/io.h> 

/*
 * Classe can:
 *   Le constructeur initialise le convertisseur.
 *   Une lecture enclanche une conversion et le resultat
 *   est retourne sur 16 bits.
 *
 */

class can
{
public:

   can();
   ~can();

   // retourne la valeur numerique correspondant a la valeur
   // analogique sur le port A.  pos doit etre entre 0 et 7
   // inclusivement.  Seulement les 10 bits de poids faible
   // sont significatifs.
   uint16_t lecture(uint8_t pos);

private:
   // Donnees membres - aucun

};

#endif /* CAN_H */
