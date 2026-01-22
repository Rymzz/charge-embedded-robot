/*
 * Auteurs : Mohamed-Adam Ezzarouali, Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Ce fichier contient l’implémentation de la classe Usart, qui permet
 * de gérer la communication série (UART) du microcontrôleur. Il configure la vitesse
 * de transmission, le format des données et fournit des fonctions pour envoyer des
 * octets ou des chaînes de caractères.
 *
 * Identification matérielle :
 * Le module USART0 est utilisé avec une vitesse de
 * transmission de 2400 bauds. L’horloge du microcontrôleur est réglée à 8 MHz. Les
 * lignes TX (transmission) et RX (réception) sont reliées au périphérique de
 * communication série.
 */

#include "usart.hpp"

constexpr uint16_t BAUD_RATE_2400 = 0xCF;

Usart::Usart()
{
    UBRR0H = 0;
    UBRR0L = BAUD_RATE_2400;

    UCSR0A = 0;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void Usart::transmettre(uint8_t donnee)
{
    while (!(UCSR0A & (1 << UDRE0)))
    {
    }
    UDR0 = donnee;
}

void Usart::transmettre(const char *texte)
{
    while (*texte)
    {
        transmettre(*texte++);
    }
}

uint8_t Usart::recevoir()
{
    while (!(UCSR0A & (1 << RXC0)))
    {
    }
    return UDR0;
}

bool Usart::disponible()
{
    return (UCSR0A & (1 << RXC0));
}

uint8_t Usart::recevoirSansBlocage()
{
    return UDR0;
}
