/*
 * Auteurs : Mohamed-Adam Ezzarouali, Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Définition de la classe DelPWM, qui permet de contrôler une DEL
 * bicolore en utilisant la modulation de largeur d'impulsion (PWM). La classe offre
 * des méthodes pour régler l'intensité des couleurs rouge et verte, effectuer des
 * transitions progressives entre les couleurs, et ajuster la couleur ambre en fonction
 * de la distance mesurée.
 *
 * Identification matérielle :
 * La DEL est contrôlée via un module PWM (typiquement
 * TIMER2) du microcontrôleur ATmega324PA. Deux canaux PWM distincts sont utilisés
 * pour piloter les composantes rouge et verte de la DEL. Le système fonctionne à 8 MHz.
 */

#pragma once

#include <avr/io.h>
#include <stdint.h>

#include "pwm.hpp"

class DelPWM
{
public:
    DelPWM(PWM &pwm);

    void regler(uint8_t rouge, uint8_t vert);
    void transition(uint16_t dureeMs);
    void eteindre();
    void reglerSelonDistance(uint16_t distance);
    void reglerSelonDistanceNonBloquant(uint16_t distance);
    void reglerSelonDistanceAmbre(uint16_t distance);

private:
    PWM &pwm_;
    uint8_t rougePWM_ = 0;
    uint8_t vertPWM_ = 255;
    bool etatRouge_ = false;
    uint16_t counterFade_ = 0;
};