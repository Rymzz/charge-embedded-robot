/*
 * Auteurs : Mohamed-Adam Ezzarouali, Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Définition de la classe Musique, utilisée pour produire des sons à
 * l'aide d'un piézoélectrique connecté au microcontrôleur. La classe configure la
 * minuterie interne (Timer0) pour générer des fréquences correspondant à des notes
 * musicales et offre des méthodes pour démarrer et arrêter la production sonore.
 *
 * Identification matérielle :
 * Sortie sonore : PB3 (OC0A). Masse du piézo : PB2 (GND).
 */

#pragma once

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

class Musique
{
public:
    Musique();

    void jouerNote(double note);
    void arreter();

    float frequences[37] = {
        110.00, 116.54, 123.47, 130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185.00,
        196.00, 207.65, 220.00, 233.08, 246.94, 261.63, 277.18, 293.66, 311.13, 329.63,
        349.23, 369.99, 392.00, 415.30, 440.00, 466.16, 493.88, 523.25, 554.37, 587.33,
        622.25, 659.26, 698.46, 739.99, 783.99, 830.61, 880.00};

private:
    static constexpr uint16_t PRESCALAGE_MINUTERIE = 256;
    static constexpr uint8_t DIVISEUR_FREQUENCE_MINUTERIE = 2;
};