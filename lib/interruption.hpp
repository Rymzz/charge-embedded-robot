/*
 * Auteurs : Mohamed-Adam Ezzarouali, Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Définition de la classe Interruption, utilisée pour configurer et
 * gérer les interruptions externes (INT0, INT1) ainsi que la minuterie du
 * microcontrôleur. Il déclare les types d'interruptions, les modes de déclenchement
 * et les méthodes permettant leur initialisation et leur utilisation.
 *
 * Identification matérielle :
 * INT0 est reliée à la broche PORTD2. INT1 est reliée à
 * la broche PORTD3. Le Timer1 est utilisé pour les minuteries (registre OCR1A).
 */

#pragma once

#include <avr/interrupt.h>
#include <avr/io.h>

class Interruption
{
public:
    enum class TypeInterruption
    {
        Int0,
        Int1
    };

    enum class Mode
    {
        LowLevel,
        AnyEdge,
        FallingEdge,
        RisingEdge
    };

    Interruption(TypeInterruption type);

    void definirMode(Mode mode);
    void initialiser();
    void partirMinuterie(uint16_t duree);

private:
    TypeInterruption type_;
};