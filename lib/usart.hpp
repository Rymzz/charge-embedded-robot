/*
 * Auteurs : Mohamed-Adam Ezzarouali, Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Ce fichier contient la définition de la classe Usart, utilisée pour
 * gérer la communication série (UART) du microcontrôleur. Elle déclare les méthodes
 * permettant d’initialiser la communication et de transmettre des données, soit sous
 * forme d’octets, soit sous forme de chaînes de caractères.
 *
 * Identification matérielle :
 * Le module matériel utilisé est USART0. La communication
 * est configurée à 2400 bauds avec une horloge de 8 MHz. Les broches TX (transmission)
 * et RX (réception) sont utilisées pour l’échange de données série.
 */

#ifndef USART_H
#define USART_H

#include <avr/io.h>

class Usart
{
public:
    Usart();

    void transmettre(uint8_t donnee);
    void transmettre(const char *texte);

    uint8_t recevoir();
    uint8_t recevoirSansBlocage();
    bool disponible();
};

#endif
