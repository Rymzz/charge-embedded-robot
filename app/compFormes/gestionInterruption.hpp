/*
 * Auteurs : Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Définition de la classe GestionInterruption, qui implémente la machine
 * à états associée à l'interrupteur détecté sur le parcours. La classe coordonne les
 * actions du robot lors de l'approche d'un poteau, l'arrêt, l'exécution d'une séquence
 * de rotations, la détection via le capteur de distance, ainsi que la validation par
 * l'appui d'un bouton. Elle regroupe également les outils permettant de vérifier la
 * présence d'un poteau, d'ajuster la trajectoire et de déterminer si la séquence
 * complète a été exécutée.
 *
 * Identification matérielle :
 * Cette classe utilise un capteur de distance (CAN), un
 * ensemble de capteurs de ligne, une DEL contrôlée via la classe Del, un bouton
 * (classe Bouton) pour confirmer l'étape, et un module de motricité RobotMotricite.
 * Le système tourne sur un microcontrôleur ATmega324PA à 8 MHz.
 */

#pragma once

#include <avr/io.h>
#include <util/delay.h>

#include "lib/del.hpp"
#include "lib/interruption.hpp"
#include "lib/robotMotricite.hpp"
#include "composants/bouton.hpp"
#include "composants/capteurDistance.hpp"
#include "composants/capteurLigne.hpp"
#include "gestionInterruption.hpp"

class GestionInterruption
{
public:
    enum class EtatInterrupteur
    {
        AVANT_4,
        ARRET_4,
        CONTINUER_CHEMIN
    };

    GestionInterruption(
        RobotMotricite &robot,
        CapteurDistance &capteurD,
        CapteurLigne &capteurL,
        Del &del);

    void executerInterrupteur();
    void sequenceRotations();
    void attendreBouton();
    void initialiserBouton();
    void ajusterLigne();
    bool poteauPresent();
    bool scanner(bool droite);
    bool estAppuye();
    bool avoirAExecute();

private:
    RobotMotricite &robot_;
    CapteurDistance &capteurDistance_;
    CapteurLigne &capteurLigne_;
    Del &del_;
    Bouton bouton_;
    EtatInterrupteur etat_;
    bool positionDroite_ = false;
    bool aExecute = false;
    uint8_t vitesseTourner_ = 45;
    uint8_t compt = 0;
    const uint16_t duree_ms = 600;
    const uint8_t pas_ms = 10;
};