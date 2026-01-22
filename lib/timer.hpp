/*
 * Auteurs : Mohamed-Adam Ezzarouali, Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Définition de la classe Timer, utilisée pour configurer les timers
 * internes du microcontrôleur (TIMER0, TIMER1, TIMER2). Elle déclare les énumérations
 * correspondant aux types de timers, aux modes de fonctionnement (Normal, CTC, PWM)
 * et aux valeurs de préscaleur disponibles, ainsi que la méthode configurerTimer()
 * permettant d'en ajuster les paramètres.
 *
 * Identification matérielle :
 * Les timers 0, 1 et 2 contrôlent diverses fonctions de
 * temporisation et de génération de PWM. Les registres OCRxA et OCRxB sont utilisés
 * pour fixer les valeurs de comparaison.
 */

#pragma once

#include <avr/interrupt.h>
#include <avr/io.h>

class Timer
{
public:
    enum Type
    {
        TIMER0,
        TIMER1,
        TIMER2
    };

    enum Mode
    {
        NORMAL,
        CTC,
        PHASE_CORRECT_PWM
    };

    enum Prescaler
    {
        NO_CLOCK,
        PRESCALE_1,
        PRESCALE_8,
        PRESCALE_64,
        PRESCALE_256,
        PRESCALE_1024
    };

    void configurerTimer(
        Type timer,
        Mode mode,
        Prescaler prescaler,
        uint16_t topValue,
        uint16_t cycleA = 0,
        uint16_t cycleB = 0);
};