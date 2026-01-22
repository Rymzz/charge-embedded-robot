/*
 * Auteurs : Mohamed-Adam Ezzarouali, Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Implémentation de la classe Robot, qui regroupe et orchestre tous les
 * sous-systèmes du projet : motricité, capteurs de ligne et de distance, modules Piezo
 * et Diode, gestion d'interrupteur, DEL RGB, PWM, musique ainsi que l'envoi du rapport
 * via USART. La classe agit comme contrôleur principal et assure la coordination
 * globale des comportements selon les formes détectées sur le parcours. Elle gère la
 * mémorisation des formes rencontrées, la détection des poteaux, ainsi que l'exécution
 * dynamique des modules spécialisés (Piezo, Diode, Interrupteur, etc.).
 *
 * Identification matérielle :
 * Le robot utilise un module de motricité RobotMotricite
 * (PORTD), un capteur de distance sur PA7, cinq capteurs de ligne (PORTA0–A4), une
 * DEL RGB contrôlée par Del et DelPWM, un module piezo (Musique), un module USART pour
 * la transmission du rapport, ainsi qu'un bouton pour déclencher l'envoi final. Le
 * système fonctionne sur un ATmega324PA cadencé à 8 MHz.
 */

#define F_CPU 8000000UL

#include "robot.hpp"

const uint16_t INTERVAL_MS = 10;
const uint16_t DELAI_CLIGNOTEMENT_MS = 500;

Robot::Robot()
    : robotMotricite_(DDRD, PORTD, PD3, PD2),
      capteurLigne_(robotMotricite_),
      capteurDistance_(DDRA, PORTA, PA7),
      pwm_(Timer::Type::TIMER2),
      delPWM_(pwm_),
      del_(DDRB, PORTB, PB0, PB1),
      diode_(capteurDistance_, del_, robotMotricite_, capteurLigne_),
      piezoelectrique_(capteurDistance_, capteurLigne_, musique_, robotMotricite_),
      gestionInterrupteur_(robotMotricite_, capteurDistance_, capteurLigne_, del_),
      rapport_(usart_)
{
}

void Robot::envoyerRapport()
{
    if (bouton_.estAppuye())
    {
        rapport_.configurerPlacements(forme1, forme2, poteau1, poteau2);
    }
}

void Robot::envoyerPendantDelay(uint16_t totalDelayMs)
{
    uint16_t iterations = totalDelayMs / INTERVAL_MS;
    for (uint16_t i = 0; i < iterations; i++)
    {
        this->envoyerRapport();
        _delay_ms(INTERVAL_MS);
    }
}

void Robot::regarderForme()
{
    if (formeActuel_ == Forme::ND ||
        formeActuel_ == Forme::POTEAU_PRESENT ||
        formeActuel_ == Forme::POTEAU_VIDE)
    {
        return;
    }

    if (forme1 == Forme::ND)
    {
        forme1 = formeActuel_;
        return;
    }

    if (forme2 == Forme::ND && formeActuel_ != forme1)
    {
        forme2 = formeActuel_;
    }
}

void Robot::regarderPoteau()
{
    if (forme1 == formeActuel_ && aExecute)
    {
        poteau1 = Forme::POTEAU_PRESENT;
        aExecute = false;
    }
    else if (forme2 == formeActuel_ && aExecute)
    {
        poteau2 = Forme::POTEAU_PRESENT;
        aExecute = false;
    }
}

void Robot::executerFormes()
{
    switch (formeActuel_)
    {
    case Forme::ND:
        break;

    case Forme::PIEZO:
        piezoelectrique_.executerComportement();
        if (piezoelectrique_.avoirAExecute())
        {
            aExecute = true;
        }
        break;

    case Forme::DIODE:
        diode_.executer();
        if (diode_.avoirAExecute())
        {
            aExecute = true;
        }
        break;

    case Forme::MOTEUR:
        break;

    case Forme::INTERRUPTEUR:
        gestionInterrupteur_.executerInterrupteur();
        if (gestionInterrupteur_.avoirAExecute())
        {
            aExecute = true;
        }
        break;

    default:
        break;
    }
}

void Robot::executer()
{
    rapport_.loop();

    while (true)
    {
        capteurLigne_.suivreLigne();

        formeActuel_ = capteurLigne_.avoirFormeActuel();
        executerFormes();

        regarderForme();
        regarderPoteau();

        if (capteurLigne_.avoirStationement())
        {
            while (true)
            {
                del_.allumer(Del::ROUGE);
                envoyerPendantDelay(DELAI_CLIGNOTEMENT_MS);
                del_.allumer(Del::VERT);
                envoyerPendantDelay(DELAI_CLIGNOTEMENT_MS);
            }
        }
    }
}