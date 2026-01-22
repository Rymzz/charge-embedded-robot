/*
 * Auteurs : Mohamed-Adam Ezzarouali, Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Implémentation des fonctions de débogage permettant d'envoyer des
 * messages ou des valeurs numériques via la communication série (USART). Ces fonctions
 * facilitent l'affichage d'informations pour le diagnostic et le suivi du comportement
 * du programme pendant l'exécution.
 *
 * Identification matérielle :
 * Le module USART0 est utilisé pour la transmission des
 * données de débogage. La vitesse de communication est fixée à 2400 bauds avec une
 * horloge de 8 MHz.
 */

#include "debug.hpp"
#include "usart.hpp"

static Usart gUsart;

void afficherDebug(uint8_t valeur)
{
    gUsart.transmettre(valeur);
}

void afficherDebug(const char *texte)
{
    gUsart.transmettre(texte);
}