/*
 * Auteurs : Mohamed-Adam Ezzarouali, Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Implémentation de la classe RobotMotricite, qui gère le déplacement
 * du robot en contrôlant la direction et la vitesse des deux roues à l'aide du module
 * PWM et des broches de contrôle de direction. Les méthodes permettent de faire
 * avancer, reculer, tourner ou arrêter le robot.
 *
 * Identification matérielle :
 * Deux broches du port (ex. PORTB ou PORTD) sont utilisées
 * pour définir la direction des roues. Le Timer2 est utilisé pour générer le signal
 * PWM qui contrôle la vitesse des moteurs. Les valeurs PWM sont calculées à partir
 * d'un pourcentage de vitesse (0 à 100 %).
 */

#define F_CPU 8000000UL

#include "robotMotricite.hpp"

const uint8_t POURCENTAGE_MAX = 100;
const uint8_t PWM_MAX = 255;
const uint16_t DELAI_AVANCE_MS = 500;
const uint16_t DELAI_CORRECTION_MS = 30;

RobotMotricite::RobotMotricite(
    volatile uint8_t &ddr,
    volatile uint8_t &port,
    uint8_t brocheDroite,
    uint8_t brocheGauche) : ddr_(ddr),
                            port_(port),
                            brocheDroite_(brocheDroite),
                            brocheGauche_(brocheGauche),
                            pwm_(Timer::Type::TIMER1)
{
    ddr_ |= (1 << brocheDroite_) | (1 << brocheGauche_);
}

uint8_t RobotMotricite::convertirPourcentage(uint8_t pourcentage)
{
    if (pourcentage > POURCENTAGE_MAX)
    {
        pourcentage = POURCENTAGE_MAX;
    }

    return (static_cast<uint16_t>(pourcentage) * PWM_MAX) / POURCENTAGE_MAX;
}

void RobotMotricite::setDirection(Direction gauche, Direction droite)
{
    if (gauche == Direction::AVANT)
    {
        port_ &= ~(1 << brocheGauche_);
    }
    else
    {
        port_ |= (1 << brocheGauche_);
    }

    if (droite == Direction::AVANT)
    {
        port_ &= ~(1 << brocheDroite_);
    }
    else
    {
        port_ |= (1 << brocheDroite_);
    }
}

void RobotMotricite::arreter()
{
    pwm_.eteindre();
}

void RobotMotricite::avancer(uint8_t vitessePourcentage)
{
    uint8_t pwmValue = convertirPourcentage(vitessePourcentage);
    setDirection(Direction::AVANT, Direction::AVANT);
    pwm_.ajustementPwm(pwmValue, pwmValue);
}

void RobotMotricite::reculer(uint8_t vitessePourcentage)
{
    uint8_t pwmValue = convertirPourcentage(vitessePourcentage);
    setDirection(Direction::ARRIERE, Direction::ARRIERE);
    pwm_.ajustementPwm(pwmValue, pwmValue);
}

void RobotMotricite::tournerGauche(uint8_t vitessePourcentage)
{
    uint8_t pwmValue = convertirPourcentage(vitessePourcentage);
    setDirection(Direction::ARRIERE, Direction::AVANT);
    pwm_.ajustementPwm(pwmValue, pwmValue);
}

void RobotMotricite::tournerDroite(uint8_t vitessePourcentage)
{
    uint8_t pwmValue = convertirPourcentage(vitessePourcentage);
    setDirection(Direction::AVANT, Direction::ARRIERE);
    pwm_.ajustementPwm(pwmValue, pwmValue);
}

void RobotMotricite::avancerEnTournant(uint8_t vitesseGauche, uint8_t vitesseDroite)
{
    setDirection(Direction::AVANT, Direction::AVANT);

    uint8_t pwmGauche = convertirPourcentage(vitesseGauche);
    uint8_t pwmDroite = convertirPourcentage(vitesseDroite);
    pwm_.ajustementPwm(pwmGauche, pwmDroite);
}

void RobotMotricite::avancerAvecCorrection(uint8_t vitesse, uint16_t dureeMs)
{
    uint16_t temps = 0;
    while (temps < dureeMs)
    {
        this->avancer(vitesse);
        _delay_ms(DELAI_AVANCE_MS);
        temps += DELAI_AVANCE_MS;

        this->tournerGauche(vitesse);
        _delay_ms(DELAI_CORRECTION_MS);
        temps += DELAI_CORRECTION_MS;
    }
    this->arreter();
}

void RobotMotricite::avancerAvecCorrectionInstantane(uint8_t vitesse)
{
    this->avancer(vitesse);
    _delay_ms(DELAI_AVANCE_MS);

    this->tournerGauche(vitesse);
    _delay_ms(DELAI_CORRECTION_MS);

    this->arreter();
}