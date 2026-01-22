/*
 * Auteurs : Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description : Définition de la classe Bouton, qui encapsule les opérations permettant
 * d'initialiser une broche en entrée et de détecter l'appui d'un bouton poussoir. La
 * classe fournit deux méthodes : l'une pour configurer le port du microcontrôleur, et
 * l'autre pour vérifier si le bouton est actuellement pressé.
 *
 * Identification matérielle : Le bouton est connecté à la broche PA5 du port A.
 * L'initialisation configure cette broche en entrée, et l'état du bouton est lu via
 * le registre PINA.
 */

#pragma once

#include <avr/io.h>
#include <util/delay.h>

class Bouton
{
public:
    void initialiserBouton();
    bool estAppuye();
};