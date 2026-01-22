/*
 * Auteurs : Mohamed-Adam Ezzarouali, Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Implémentation de la classe PWM, qui permet de configurer et de
 * contrôler le signal PWM (Pulse Width Modulation) sur le microcontrôleur. Elle
 * gère la configuration du Timer2 en mode Phase Correct PWM ainsi que l'ajustement
 * du rapport cyclique pour les sorties OC2A et OC2B.
 *
 * Identification matérielle :
 * Les broches PD6 (OC2A) et PD7 (OC2B) sont configurées
 * en sortie. Le Timer2 est utilisé avec un prescaler de 8 en mode Phase Correct PWM
 * (8 bits).
 */

#include "pwm.hpp"

const uint8_t BROCHE_OC0A = DDB3;
const uint8_t BROCHE_OC0B = DDB4;
const uint8_t BROCHE_OC1A = DDD4;
const uint8_t BROCHE_OC1B = DDD5;
const uint8_t BROCHE_OC2A = DDD6;
const uint8_t BROCHE_OC2B = DDD7;
const uint8_t RAPPORT_CYCLIQUE_MIN = 0;

PWM::PWM(Timer::Type timerChoisi) : timer_(timerChoisi)
{
    switch (timer_)
    {
    case Timer::Type::TIMER0:
        DDRB |= (1 << BROCHE_OC0A) | (1 << BROCHE_OC0B);

        TCCR0A = (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1);
        TCCR0B = (1 << CS01);
        break;

    case Timer::Type::TIMER1:
        DDRD |= (1 << BROCHE_OC1A) | (1 << BROCHE_OC1B);

        TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
        TCCR1B = (1 << CS11);
        break;

    case Timer::Type::TIMER2:
        DDRD |= (1 << BROCHE_OC2A) | (1 << BROCHE_OC2B);
        OCR2A = RAPPORT_CYCLIQUE_MIN;
        OCR2B = RAPPORT_CYCLIQUE_MIN;
        TCCR2A = (1 << COM2A1) | (1 << COM2B1) | (1 << WGM20);
        TCCR2B = (1 << CS21);
        break;

    default:
        break;
    }
}

void PWM::ajustementPwm(uint8_t ratioA, uint8_t ratioB)
{
    switch (timer_)
    {
    case Timer::Type::TIMER0:
        OCR0A = ratioA;
        OCR0B = ratioB;
        break;

    case Timer::Type::TIMER1:
        OCR1A = ratioA;
        OCR1B = ratioB;
        break;

    case Timer::Type::TIMER2:
        OCR2A = ratioA;
        OCR2B = ratioB;
        break;

    default:
        break;
    }
}

void PWM::eteindre()
{
    ajustementPwm(RAPPORT_CYCLIQUE_MIN, RAPPORT_CYCLIQUE_MIN);
}