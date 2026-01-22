/*
 * Auteurs : Mohamed-Adam Ezzarouali, Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Définition de la classe RobotMotricite, utilisée pour contrôler la
 * motricité d'un robot à deux roues. Elle gère à la fois la direction et la vitesse
 * des moteurs à l'aide d'un signal PWM et de broches de commande.
 *
 * Identification matérielle :
 * Deux broches du microcontrôleur contrôlent la direction
 * (avant/arrière) des moteurs gauche et droit. Le Timer2 génère le signal PWM servant
 * à ajuster la vitesse des roues.
 */

#pragma once

#include <util/delay.h>

#include "pwm.hpp"

class RobotMotricite
{
public:
    enum Direction
    {
        ARRIERE,
        AVANT
    };

    RobotMotricite(
        volatile uint8_t &ddr,
        volatile uint8_t &port,
        uint8_t brocheDroite,
        uint8_t brocheGauche);

    void avancer(uint8_t vitessePourcentage);
    void reculer(uint8_t vitessePourcentage);
    void tournerGauche(uint8_t vitessePourcentage);
    void tournerDroite(uint8_t vitessePourcentage);
    void avancerEnTournant(uint8_t vitesseGauche, uint8_t vitesseDroite);
    void avancerAvecCorrection(uint8_t vitesse, uint16_t dureeMs);
    void avancerAvecCorrectionInstantane(uint8_t vitesse);
    void arreter();

private:
    uint8_t convertirPourcentage(uint8_t pourcentage);
    void setDirection(Direction gauche, Direction droite);

    volatile uint8_t &ddr_;
    volatile uint8_t &port_;
    uint8_t brocheDroite_;
    uint8_t brocheGauche_;
    PWM pwm_;

    static constexpr uint8_t POURCENTAGE_MAX = 100;
    static constexpr uint8_t PWM_MAX = 255;
};