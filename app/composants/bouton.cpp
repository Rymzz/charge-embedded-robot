/*
 * Auteurs : Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Implémentation de la classe Bouton, qui permet d'initialiser et de
 * lire l'état d'un bouton poussoir connecté au microcontrôleur. La méthode estAppuye()
 * inclut un léger délai pour gérer l'antirebond et assurer que la détection de l'appui
 * est fiable. Le bouton est configuré en entrée via le registre DDRA.
 *
 * Identification matérielle :
 * Le bouton est connecté à la broche PA5 du port A. La
 * lecture s'effectue via le registre PINA, et l'activation de l'antirebond repose sur
 * la fonction _delay_ms().
 */

#define F_CPU 8000000UL

#include "bouton.hpp"

const uint8_t BOUTON_PIN = PA5;
const uint8_t DELAI_ANTIREBOND_MS = 50;

void Bouton::initialiserBouton()
{
    DDRA &= ~(1 << BOUTON_PIN);
}

bool Bouton::estAppuye()
{
    if (PINA & (1 << BOUTON_PIN))
    {
        _delay_ms(DELAI_ANTIREBOND_MS);
        if (PINA & (1 << BOUTON_PIN))
        {
            return true;
        }
    }
    return false;
}