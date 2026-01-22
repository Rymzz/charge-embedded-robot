/*
 * Auteurs : Mohamed-Adam Ezzarouali, Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Implémentation de la classe Interruption, qui permet de configurer et
 * de gérer les interruptions externes (INT0, INT1) ainsi que la minuterie du
 * microcontrôleur. Les méthodes permettent de définir le mode de déclenchement,
 * d'initialiser les interruptions et de démarrer une minuterie avec une durée donnée.
 *
 * Identification matérielle :
 * INT0 est reliée à la broche PORTD2. INT1 est reliée à
 * la broche PORTD3. La minuterie utilisée est Timer1 (registre OCR1A) pour la
 * génération d'interruptions temporelles.
 */

#include "interruption.hpp"

const uint8_t BROCHE_INT0 = PD2;
const uint8_t BROCHE_INT1 = PD3;

Interruption::Interruption(TypeInterruption type) : type_(type)
{
}

void Interruption::definirMode(Mode mode)
{
    switch (mode)
    {
    case Mode::LowLevel:
        switch (type_)
        {
        case TypeInterruption::Int0:
            EICRA &= ~((1 << ISC01) | (1 << ISC00));
            break;
        case TypeInterruption::Int1:
            EICRA &= ~((1 << ISC11) | (1 << ISC10));
            break;
        }
        break;

    case Mode::AnyEdge:
        switch (type_)
        {
        case TypeInterruption::Int0:
            EICRA &= ~(1 << ISC01);
            EICRA |= (1 << ISC00);
            break;
        case TypeInterruption::Int1:
            EICRA &= ~(1 << ISC11);
            EICRA |= (1 << ISC10);
            break;
        }
        break;

    case Mode::FallingEdge:
        switch (type_)
        {
        case TypeInterruption::Int0:
            EICRA |= (1 << ISC01);
            EICRA &= ~(1 << ISC00);
            break;
        case TypeInterruption::Int1:
            EICRA |= (1 << ISC11);
            EICRA &= ~(1 << ISC10);
            break;
        }
        break;

    case Mode::RisingEdge:
        switch (type_)
        {
        case TypeInterruption::Int0:
            EICRA |= (1 << ISC01) | (1 << ISC00);
            break;
        case TypeInterruption::Int1:
            EICRA |= (1 << ISC11) | (1 << ISC10);
            break;
        }
        break;
    }
}

void Interruption::initialiser()
{
    cli();

    switch (type_)
    {
    case TypeInterruption::Int0:
        DDRD &= ~(1 << BROCHE_INT0);
        EIMSK |= (1 << INT0);
        break;

    case TypeInterruption::Int1:
        DDRD &= ~(1 << BROCHE_INT1);
        EIMSK |= (1 << INT1);
        break;
    }

    sei();
}

void Interruption::partirMinuterie(uint16_t duree)
{
    TCNT1 = 0;
    OCR1A = duree;

    TCCR1A = 0;
    TCCR1B = (1 << WGM12);
    TCCR1C = 0;
    TIMSK1 = (1 << OCIE1A);
    TCCR1B |= (1 << CS12) | (1 << CS10);
}