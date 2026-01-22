/*
 * Auteurs : Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description : Implémentation de la classe CapteurDistance, qui permet de lire la
 * valeur analogique provenant d'un capteur de distance et d'en calculer une moyenne
 * afin d'obtenir une mesure plus stable. La classe configure la broche du capteur
 * en entrée, utilise le convertisseur analogique-numérique (CAN) du
 * microcontrôleur, et fournit des méthodes pour lire une valeur simple ou une distance
 * moyennée sur plusieurs lectures.
 *
 * Identification matérielle : Le capteur de distance est connecté à une broche
 * analogique du microcontrôleur. La broche utilisée est configurée en entrée dans le
 * constructeur grâce au DDR correspondant. La lecture est réalisée via le CAN interne
 * (méthode convertisseur_.lecture()).
 */

#define F_CPU 8000000UL

#include "capteurDistance.hpp"

const uint8_t NB_LECTURES_MOYENNE = 8;
const uint8_t DELAI_ENTRE_LECTURES_MS = 2;
const uint8_t DECALAGE_DIVISION_PAR_8 = 3;

CapteurDistance::CapteurDistance(
    volatile uint8_t &ddr,
    volatile uint8_t &port,
    uint8_t brocheAnalogique) : port_(port),
                                ddr_(ddr),
                                brocheAnalogique_(brocheAnalogique)
{
    ddr_ &= ~(1 << brocheAnalogique_);
}

uint16_t CapteurDistance::lire()
{
    return convertisseur_.lecture(brocheAnalogique_);
}

uint16_t CapteurDistance::lireDistanceMoyenne()
{
    uint32_t somme = 0;

    for (uint8_t i = 0; i < NB_LECTURES_MOYENNE; i++)
    {
        somme += lire();
        _delay_ms(DELAI_ENTRE_LECTURES_MS);
    }

    return (somme >> DECALAGE_DIVISION_PAR_8);
}