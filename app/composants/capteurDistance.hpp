/*
 * Auteurs : Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description : Définition de la classe CapteurDistance, qui encapsule la lecture d'un
 * capteur analogique de distance à l'aide du convertisseur analogique-numérique (CAN).
 * La classe déclare les méthodes permettant d'obtenir une lecture brute ainsi qu'une
 * distance moyenne basée sur plusieurs mesures successives. Elle stocke également les
 * registres nécessaires à la configuration de la broche en entrée.
 *
 * Identification matérielle : Le capteur est relié à une broche analogique du
 * microcontrôleur, configurée en entrée dans le constructeur. Les lectures sont
 * effectuées à l'aide du CAN interne.
 */

#pragma once

#include <avr/io.h>
#include <util/delay.h>

#include "lib/can.hpp"
#include "lib/del.hpp"

class CapteurDistance
{
public:
    CapteurDistance(
        volatile uint8_t &ddr,
        volatile uint8_t &port,
        uint8_t brocheAnalogique);

    uint16_t lire();
    uint16_t lireDistanceMoyenne();

private:
    volatile uint8_t &port_;
    volatile uint8_t &ddr_;
    uint8_t brocheAnalogique_;
    can convertisseur_;
};