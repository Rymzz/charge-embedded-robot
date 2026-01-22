/*
 * Auteurs : Mohamed-Adam Ezzarouali, Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Implémentation des fonctions de la classe Timer, permettant de
 * configurer les timers (TIMER0, TIMER1, TIMER2) du microcontrôleur en différents
 * modes : Normal, CTC ou PWM phase correct. Il permet également de définir le
 * préscaleur, les valeurs de comparaison (OCRxA, OCRxB) et d'activer les interruptions
 * associées.
 *
 * Identification matérielle :
 * TIMER0, TIMER1 et TIMER2 sont utilisés pour la
 * génération de signaux ou la mesure du temps. Les registres OCR0A/B, OCR1A/B et
 * OCR2A/B contrôlent respectivement les sorties PWM. Les interruptions OCIE0A,
 * OCIE1A et OCIE2A sont activées pour déclencher une routine lors de la comparaison.
 */

#include "timer.hpp"

#include <avr/interrupt.h>

const uint8_t VALEUR_INITIALE_REGISTRE = 0;

void Timer::configurerTimer(
    Timer::Type timer,
    Timer::Mode mode,
    Timer::Prescaler prescaler,
    uint16_t topValue,
    uint16_t cycleA,
    uint16_t cycleB)
{
    switch (timer)
    {
    case TIMER0:
        TCCR0A = VALEUR_INITIALE_REGISTRE;
        TCCR0B = VALEUR_INITIALE_REGISTRE;
        TCNT0 = VALEUR_INITIALE_REGISTRE;

        if (cycleA != 0)
        {
            OCR0A = (uint8_t)cycleA;
        }
        else
        {
            OCR0A = (uint8_t)topValue;
        }

        if (cycleB != 0)
        {
            OCR0B = (uint8_t)cycleB;
        }

        switch (mode)
        {
        case NORMAL:
            TCCR0A |= (1 << COM0A0);
            break;
        case CTC:
            TCCR0A |= (1 << WGM01) | (1 << COM0B0);
            break;
        case PHASE_CORRECT_PWM:
            TCCR0A |= (1 << WGM00) | (1 << COM0A1 | (1 << COM0B1));
            break;
        }

        switch (prescaler)
        {
        case PRESCALE_1:
            TCCR0B |= (1 << CS00);
            break;
        case PRESCALE_8:
            TCCR0B |= (1 << CS01);
            break;
        case PRESCALE_64:
            TCCR0B |= (1 << CS01) | (1 << CS00);
            break;
        case PRESCALE_256:
            TCCR0B |= (1 << CS02);
            break;
        case PRESCALE_1024:
            TCCR0B |= (1 << CS02) | (1 << CS00);
            break;
        case NO_CLOCK:
        default:
            break;
        }
        break;

    case TIMER1:
        TCCR1A = VALEUR_INITIALE_REGISTRE;
        TCCR1B = VALEUR_INITIALE_REGISTRE;
        TCCR1C = VALEUR_INITIALE_REGISTRE;
        TCNT1 = VALEUR_INITIALE_REGISTRE;

        if (cycleA != 0)
        {
            OCR1A = cycleA;
        }
        else
        {
            OCR1A = topValue;
        }

        if (cycleB != 0)
        {
            OCR1B = cycleB;
        }

        switch (mode)
        {
        case NORMAL:
            TCCR1A |= (1 << COM1A0) | (1 << COM1B0);
            break;
        case CTC:
            TCCR1A |= (1 << COM1A0) | (1 << COM1B0);
            TCCR1B |= (1 << WGM12);
            break;
        case PHASE_CORRECT_PWM:
            TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
            break;
        }

        switch (prescaler)
        {
        case PRESCALE_1:
            TCCR1B |= (1 << CS10);
            break;
        case PRESCALE_8:
            TCCR1B |= (1 << CS11);
            break;
        case PRESCALE_64:
            TCCR1B |= (1 << CS11) | (1 << CS10);
            break;
        case PRESCALE_256:
            TCCR1B |= (1 << CS12);
            break;
        case PRESCALE_1024:
            TCCR1B |= (1 << CS12) | (1 << CS10);
            break;
        case NO_CLOCK:
        default:
            break;
        }

        TIMSK1 = (1 << OCIE1A);
        break;

    case TIMER2:
        TCCR2A = VALEUR_INITIALE_REGISTRE;
        TCCR2B = VALEUR_INITIALE_REGISTRE;
        TCNT2 = VALEUR_INITIALE_REGISTRE;

        if (cycleA != 0)
        {
            OCR2A = (uint8_t)cycleA;
        }
        else
        {
            OCR2A = (uint8_t)topValue;
        }

        if (cycleB != 0)
        {
            OCR2B = (uint8_t)cycleB;
        }

        switch (mode)
        {
        case NORMAL:
            TCCR2A |= (1 << COM2A0);
            break;
        case CTC:
            TCCR2A |= (1 << WGM21) | (1 << COM2A0);
            break;
        case PHASE_CORRECT_PWM:
            TCCR2A |= (1 << WGM20) | (1 << COM2A1);
            break;
        }

        switch (prescaler)
        {
        case PRESCALE_1:
            TCCR2B |= (1 << CS20);
            break;
        case PRESCALE_8:
            TCCR2B |= (1 << CS21);
            break;
        case PRESCALE_64:
            TCCR2B |= (1 << CS21) | (1 << CS20);
            break;
        case PRESCALE_256:
            TCCR2B |= (1 << CS22);
            break;
        case PRESCALE_1024:
            TCCR2B |= (1 << CS22) | (1 << CS20);
            break;
        case NO_CLOCK:
        default:
            break;
        }

        TIMSK2 = (1 << OCIE2A);
        break;
    }
}