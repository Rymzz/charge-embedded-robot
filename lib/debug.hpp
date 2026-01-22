/*
 * Auteurs : Mohamed-Adam Ezzarouali, Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Définitions conditionnelles liées au mode de débogage. Lorsque la
 * constante DEBUG est définie, les fonctions afficherDebug() permettent l'envoi
 * d'informations de diagnostic via la liaison série (USART). Dans le cas contraire,
 * les appels à DEBUG_PRINT() sont ignorés.
 *
 * Identification matérielle :
 * La communication de débogage utilise le module USART0 (RS-232).
 * La vitesse de communication est fixée à 2400 bauds avec une horloge de 8 MHz.
 */

#pragma once

#ifdef DEBUG

void afficherDebug(uint8_t valeur);
void afficherDebug(const char *texte);

#define DEBUG_PRINT(x) afficherDebug(x)

#else

#define DEBUG_PRINT(x) \
    do                 \
    {                  \
    } while (0)

#endif