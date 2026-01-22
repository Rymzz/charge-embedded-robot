/*
 * Auteurs : Mohamed-Adam Ezzarouali, Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Définition de la classe PWM, utilisée pour contrôler la modulation de
 * largeur d'impulsion (PWM) à l'aide des timers du microcontrôleur. Elle déclare les
 * méthodes permettant d'ajuster le rapport cyclique et d'éteindre le signal PWM.
 *
 * Identification matérielle :
 * Le Timer2 est principalement utilisé pour générer le
 * signal PWM. Les sorties associées sont PD6 (OC2A) et PD7 (OC2B).
 */

#pragma once

#include <avr/io.h>

#include "timer.hpp"

class PWM
{
public:
    PWM() = default;
    PWM(Timer::Type timerChoisi);

    void ajustementPwm(uint8_t ratioA, uint8_t ratioB = 0);
    void eteindre();

private:
    Timer::Type timer_;
};