/*
 * Auteurs : Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Définition de la classe Robot, qui regroupe l'ensemble des modules
 * matériels et logiciels nécessaires au fonctionnement autonome du robot. La classe
 * assure la coordination entre la motricité, les capteurs (ligne et distance), la DEL
 * RGB, le PWM, le module piezo, le module diode, ainsi que la gestion de l'interrupteur.
 * Elle inclut également la logique de mémorisation des formes rencontrées, la détection
 * des poteaux, l'exécution des comportements associés et la génération du rapport final
 * via USART. Robot agit comme point central assurant le déroulement complet du parcours.
 *
 * Identification matérielle :
 * Cette classe intègre un module RobotMotricite (PORTD),
 * un capteur de distance sur PA7, cinq capteurs de ligne (PA0 à PA4), un module de PWM
 * (TIMER2), une DEL RGB via Del et DelPWM, un piezo (classe Musique), un bouton physique,
 * un module USART pour le rapport final. Le robot est contrôlé par un ATmega324PA tournant
 * à 8 MHz.
 */

#pragma once

#include <avr/io.h>

#include "lib/del.hpp"
#include "lib/delPWM.hpp"
#include "lib/musique.hpp"
#include "lib/pwm.hpp"
#include "compFormes/diode.hpp"
#include "compFormes/formes.hpp"
#include "compFormes/gestionInterruption.hpp"
#include "compFormes/piezoelectrique.hpp"
#include "composants/bouton.hpp"
#include "composants/capteurDistance.hpp"
#include "composants/capteurLigne.hpp"
#include "rapport/envoyerRapport.hpp"

class Robot
{
public:
    Robot();

    void executer();
    void executerFormes();
    void myDelay(unsigned long iteration);
    void regarderPoteau();
    void regarderForme();
    void envoyerRapport();
    void envoyerPendantDelay(uint16_t totalDelayMs);

private:
    RobotMotricite robotMotricite_;
    Musique musique_;
    CapteurLigne capteurLigne_;
    CapteurDistance capteurDistance_;
    PWM pwm_;
    DelPWM delPWM_;
    Del del_;
    Diode diode_;
    Bouton bouton_;
    Piezoelectrique piezoelectrique_;
    GestionInterruption gestionInterrupteur_;
    EnvoyerRapport rapport_;
    Usart usart_;
    Forme formeActuel_ = Forme::POTEAU_VIDE;
    Forme forme1 = Forme::ND;
    Forme forme2 = Forme::ND;
    Forme poteau1 = Forme::POTEAU_VIDE;
    Forme poteau2 = Forme::POTEAU_VIDE;
    bool comportementSymbole_ = false;
    bool aExecute = false;
    bool delFading_ = false;
    int delFadeStep_ = 0;

    static constexpr uint8_t INTERVAL_MS = 10;
};