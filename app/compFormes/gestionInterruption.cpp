/*
 * Auteurs : Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Implémentation de la classe GestionInterruption, responsable de la
 * logique de traitement des interruptions liées à l'interrupteur détecté sur le
 * parcours. La classe coordonne la lecture du capteur de distance, l'ajustement de
 * la ligne, les mouvements du robot et l'activation de la DEL selon une machine à
 * états interne (approche, arrêt, rotation, validation et reprise du trajet). Elle
 * gère également la détermination de la présence d'un poteau ainsi que la séquence
 * visuelle et motrice associée.
 *
 * Identification matérielle :
 * La DEL est pilotée via la classe Del. Le capteur de
 * distance et les capteurs de ligne sont utilisés pour déterminer les transitions
 * d'état. Un bouton externe, utilisé pour valider une étape, est lu dans la boucle
 * d'attente. Le microcontrôleur utilisé est l'ATmega324PA cadencé à 8 MHz.
 */

#define F_CPU 8000000UL

#include "gestionInterruption.hpp"

const uint16_t DISTANCE_POTEAU_MIN = 170;
const uint16_t DISTANCE_POTEAU_MAX = 250;
const uint16_t DELAI_ARRET_MS = 400;
const uint16_t DELAI_ROTATION_GAUCHE_COURTE_MS = 300;
const uint16_t DELAI_ROTATION_LONGUE_MS = 600;
const uint16_t DELAI_ATTENTE_BOUTON_MS = 2000;
const uint16_t COMPTEUR_MAX_AJUSTEMENT = 300;
const uint8_t VITESSE_ROTATION_GAUCHE = 50;
const uint8_t VITESSE_ROTATION_DROITE = 45;

volatile bool estAppuyer = false;

GestionInterruption::GestionInterruption(
    RobotMotricite &robot,
    CapteurDistance &capteurD,
    CapteurLigne &capteurL,
    Del &del) : robot_(robot),
                capteurDistance_(capteurD),
                capteurLigne_(capteurL),
                del_(del),
                etat_(EtatInterrupteur::AVANT_4)
{
}

bool GestionInterruption::avoirAExecute()
{
    return aExecute;
}

bool GestionInterruption::poteauPresent()
{
    uint16_t distance = capteurDistance_.lireDistanceMoyenne();

    if (distance >= DISTANCE_POTEAU_MIN && distance <= DISTANCE_POTEAU_MAX)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void GestionInterruption::sequenceRotations()
{
    robot_.tournerGauche(VITESSE_ROTATION_GAUCHE);
    _delay_ms(DELAI_ROTATION_GAUCHE_COURTE_MS);
    robot_.tournerDroite(VITESSE_ROTATION_DROITE);
    _delay_ms(DELAI_ROTATION_LONGUE_MS);
    robot_.tournerGauche(VITESSE_ROTATION_DROITE);
    _delay_ms(DELAI_ROTATION_LONGUE_MS);
    robot_.tournerDroite(VITESSE_ROTATION_DROITE);
    _delay_ms(DELAI_ROTATION_LONGUE_MS);
    robot_.tournerGauche(VITESSE_ROTATION_GAUCHE);
    _delay_ms(DELAI_ROTATION_GAUCHE_COURTE_MS);
    robot_.arreter();
    _delay_ms(DELAI_ARRET_MS);
}

void GestionInterruption::executerInterrupteur()
{
    uint16_t distance = capteurDistance_.lireDistanceMoyenne();

    switch (etat_)
    {
    case EtatInterrupteur::AVANT_4:
        if (distance >= DISTANCE_POTEAU_MIN && distance <= DISTANCE_POTEAU_MAX)
        {
            etat_ = EtatInterrupteur::ARRET_4;
        }
        break;

    case EtatInterrupteur::ARRET_4:
        robot_.arreter();
        _delay_ms(DELAI_ARRET_MS);

        sequenceRotations();

        capteurLigne_.ajusterLigneDevant();

        while (!poteauPresent())
        {
            if (compt >= COMPTEUR_MAX_AJUSTEMENT)
            {
                break;
            }
            compt++;
        }
        compt = 0;

        positionDroite_ = false;

        while (!bouton_.estAppuye())
        {
        }

        _delay_ms(DELAI_ATTENTE_BOUTON_MS);

        if (!poteauPresent())
        {
            etat_ = EtatInterrupteur::CONTINUER_CHEMIN;
        }
        else
        {
            etat_ = EtatInterrupteur::ARRET_4;
        }
        break;

    case EtatInterrupteur::CONTINUER_CHEMIN:
        aExecute = true;
        break;
    }
}