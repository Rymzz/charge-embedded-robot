/*
 * Auteurs : Mohamed-Adam Ezzarouali, Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Implémentation de la classe DelPWM, qui permet de contrôler une DEL
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

#define F_CPU 8000000

#include "delPWM.hpp"

#include <math.h>
#include <util/delay.h>

const uint16_t DISTANCE_10CM = 75;
const uint16_t DISTANCE_4CM = 190;
const uint16_t CYCLE_TOTAL_MS = 35;
const uint16_t NB_ETAPES_TRANSITION = 255;
const uint8_t INTENSITE_MAX = 255;
const uint8_t INTENSITE_MIN = 0;
const uint16_t DELAI_RUNTIME_SLICE_MS = 10;
const uint16_t DELAI_TRANSITION_MS = 1;

DelPWM::DelPWM(PWM &pwm) : pwm_(pwm)
{
}

void DelPWM::regler(uint8_t rouge, uint8_t vert)
{
    pwm_.ajustementPwm(rouge, vert);
}

void DelPWM::eteindre()
{
    pwm_.ajustementPwm(INTENSITE_MIN, INTENSITE_MIN);
}

static inline void delay_ms_runtime(uint16_t ms)
{
    uint16_t iterations = ms / DELAI_RUNTIME_SLICE_MS;
    while (iterations--)
    {
        _delay_ms(DELAI_RUNTIME_SLICE_MS);
    }
}

void DelPWM::reglerSelonDistanceAmbre(uint16_t distance)
{
    float ratio = float(DISTANCE_4CM - distance) / float(DISTANCE_4CM - DISTANCE_10CM);

    uint16_t dureeRouge = uint16_t(CYCLE_TOTAL_MS * ratio);
    uint16_t dureeVert = CYCLE_TOTAL_MS - dureeRouge;

    pwm_.ajustementPwm(INTENSITE_MIN, INTENSITE_MAX);
    delay_ms_runtime(dureeRouge);

    pwm_.ajustementPwm(INTENSITE_MAX, INTENSITE_MIN);
    delay_ms_runtime(dureeVert);
}

void DelPWM::transition(uint16_t dureeMs)
{
    uint16_t delaiParEtape = dureeMs / NB_ETAPES_TRANSITION;

    for (uint16_t i = 0; i <= NB_ETAPES_TRANSITION; i++)
    {
        uint8_t rouge = i;
        uint8_t vert = NB_ETAPES_TRANSITION - i;

        pwm_.ajustementPwm(rouge, vert);

        for (uint16_t j = 0; j < delaiParEtape; j++)
        {
            _delay_ms(DELAI_TRANSITION_MS);
        }
    }
}