/*
 * Auteurs : Mohamed-Adam Ezzarouali, Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Ce fichier contient l’implémentation de la classe Del, qui permet de
 * contrôler une DEL bicolore (rouge et verte) connectée à deux broches du
 * microcontrôleur. Les méthodes permettent d’allumer la DEL en rouge, vert, ambre
 * (alternance rapide) ou de l’éteindre complètement.
 *
 * Identification matérielle :
 * Les deux broches associées à la DEL sont configurées en
 * sortie sur le même port. La couleur ambre est produite par un clignotement rapide
 * entre le rouge et le vert. Une fréquence d’horloge de 8 MHz (F_CPU = 8000000) est
 * utilisée pour le délai.
 */

#define F_CPU 8000000UL
#include <util/delay.h>
#include "del.hpp"

constexpr uint16_t DUREE_LED_VERT_MS = 12;
constexpr uint16_t DUREE_LED_ROUGE_MS = 3;

Del::Del(volatile uint8_t &ddr, volatile uint8_t &port, uint8_t brocheRouge, uint8_t brocheVerte)
    : port_(port), ddr_(ddr), brocheRouge_(brocheRouge), brocheVerte_(brocheVerte)
{
    ddr_ |= (1 << brocheRouge_) | (1 << brocheVerte_);
}

void Del::allumer(Couleur couleur)
{
    switch (couleur)
    {
    case ROUGE:
        port_ |= (1 << brocheRouge_);
        port_ &= ~(1 << brocheVerte_);
        break;

    case VERT:
        port_ &= ~(1 << brocheRouge_);
        port_ |= (1 << brocheVerte_);
        break;

    case ETEINT:
        port_ &= ~(1 << brocheRouge_);
        port_ &= ~(1 << brocheVerte_);
        break;

    case AMBRE:
        allumerAmbre();
        break;
    }
}

void Del::allumerAmbre()
{
    while (true)
    {
        allumer(ROUGE);
        _delay_ms(DUREE_LED_ROUGE_MS);

        allumer(VERT);
        _delay_ms(DUREE_LED_VERT_MS);
    }
}

void Del::eteindre()
{
    allumer(ETEINT);
}

void Del::ambreNonBloquant(uint8_t ratioRouge)
{

    constexpr uint8_t CYCLE_US = 100;

    uint8_t tRouge = (ratioRouge * CYCLE_US) / 255;
    uint8_t tVert = CYCLE_US - tRouge;

    port_ &= ~(1 << brocheRouge_);
    port_ |= (1 << brocheVerte_);
    for (uint8_t i = 0; i < tVert; i++)
    {
        _delay_us(10);
    }

    port_ |= (1 << brocheRouge_);
    port_ &= ~(1 << brocheVerte_);
    for (uint8_t i = 0; i < tRouge; i++)
    {
        _delay_us(10);
    }
}
