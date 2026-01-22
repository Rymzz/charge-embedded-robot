/*
 * Auteurs : Mohamed-Adam Ezzarouali, Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Implémentation de la classe Musique, qui permet de produire des sons
 * à l'aide d'un piézoélectrique connecté au microcontrôleur. Le programme configure
 * le Timer0 en mode CTC afin de générer des signaux carrés dont la fréquence
 * correspond aux notes musicales définies dans le tableau des fréquences. Les méthodes
 * jouerNote() et arreter() contrôlent la génération et l'arrêt du signal sonore.
 *
 * Identification matérielle :
 * Sortie sonore : PB3 (OC0A). Masse du piézo : PB2 (GND).
 */

#define F_CPU 8000000UL

#include "musique.hpp"

#include <avr/io.h>
#include <util/delay.h>

const uint8_t BROCHE_MASSE_PIEZO = PB2;
const uint8_t BROCHE_SIGNAL_PIEZO = PB3;
const uint8_t NOTE_OFFSET = 45;
const uint8_t PRESCALAGE_MINUTERIE = 256;
const uint8_t DIVISEUR_FREQUENCE_MINUTERIE = 2;

Musique::Musique()
{
    DDRB |= (1 << BROCHE_MASSE_PIEZO) | (1 << BROCHE_SIGNAL_PIEZO);
    PORTB &= ~(1 << BROCHE_MASSE_PIEZO);

    TCCR0A &= ~((1 << WGM02) | (1 << WGM00) | (1 << COM0A1));
    TCCR0A |= (1 << WGM01) | (1 << COM0A0);

    TCNT0 = 0;
}

void Musique::jouerNote(double note)
{
    int index = (note - NOTE_OFFSET);
    double frequence = frequences[index];

    TCCR0B &= ~((1 << CS01) | (1 << CS00));
    TCCR0B |= (1 << CS02);

    OCR0A = (F_CPU / (PRESCALAGE_MINUTERIE * DIVISEUR_FREQUENCE_MINUTERIE * frequence)) - 1;
}

void Musique::arreter()
{
    TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
    OCR0A = 0;
}