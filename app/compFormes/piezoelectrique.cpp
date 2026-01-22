/*
 * Auteurs : Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Implémentation de la classe Piezoelectrique, responsable de la gestion
 * des comportements sonores associés au capteur piezo. La classe permet de détecter
 * la présence d'un poteau à l'aide du capteur de distance, de jouer des bips graves
 * successifs tant que l'obstacle est présent, et d'exécuter une mélodie finale une
 * fois que le poteau a été retiré. Elle met également à jour la trajectoire via le
 * capteur de ligne pendant les séquences sonores.
 *
 * Identification matérielle :
 * Le module utilise un capteur de distance analogique pour
 * détecter la présence du poteau, un piezo contrôlé par la classe Musique, un module
 * de motricité RobotMotricite, et un capteur de ligne pour l'ajustement dynamique
 * pendant la mélodie. Les délais reposent sur un microcontrôleur ATmega324PA cadencé
 * à 8 MHz.
 */

#define F_CPU 8000000UL

#include "piezoelectrique.hpp"

const uint8_t NB_BIP_GRAVE = 3;
const uint8_t NB_VERIFICATIONS = 8;
const uint16_t DUREE_BIP_MS = 200;
const uint16_t DUREE_SILENCE_MS = 200;
const uint16_t DUREE_VERIF_MS = 250;
const uint16_t DUREE_NOTE_FIN_MS = 300;
const uint16_t DISTANCE_4_PO = 170;
const uint16_t DISTANCE_POTEAU_MAX = 230;
const uint8_t NOTE_BIP_GRAVE = 50;

Piezoelectrique::Piezoelectrique(
    CapteurDistance &capteurDistance,
    CapteurLigne &capteurLigne,
    Musique &musique,
    RobotMotricite &robot) : capteur_(capteurDistance),
                             capteurLigne_(capteurLigne),
                             musique_(musique),
                             robot_(robot)
{
}

bool Piezoelectrique::avoirAExecute()
{
    return aExecute;
}

bool Piezoelectrique::detecterPoteau()
{
    uint16_t distance = capteur_.lireDistanceMoyenne();
    poteauPresent_ = (distance >= DISTANCE_4_PO && distance <= DISTANCE_POTEAU_MAX);
    return poteauPresent_;
}

bool Piezoelectrique::sequenceVerification()
{
    for (uint8_t i = 0; i < NB_VERIFICATIONS; i++)
    {
        _delay_ms(DUREE_VERIF_MS);

        detecterPoteau();
        if (!poteauPresent_)
        {
            return true;
        }
    }
    return false;
}

void Piezoelectrique::jouerTroisBipsGraves()
{
    for (uint8_t i = 0; i < NB_BIP_GRAVE; i++)
    {
        musique_.jouerNote(NOTE_BIP_GRAVE);
        _delay_ms(DUREE_BIP_MS);

        musique_.arreter();
        _delay_ms(DUREE_SILENCE_MS);
    }
}

void Piezoelectrique::jouerMelodieFin()
{
    const uint8_t melodie[] = {60, 64, 67, 72, 67, 64};
    const uint8_t nbNotes = sizeof(melodie) / sizeof(melodie[0]);

    for (uint8_t i = 0; i < nbNotes; i++)
    {
        musique_.jouerNote(melodie[i]);
        _delay_ms(DUREE_NOTE_FIN_MS);
        capteurLigne_.suivreLigne();
    }

    musique_.arreter();
}

void Piezoelectrique::executerComportement()
{
    uint16_t distance = capteur_.lireDistanceMoyenne();

    switch (etat_)
    {
    case EtatPiezo::AVANCE:
        if (distance >= DISTANCE_4_PO && distance <= DISTANCE_POTEAU_MAX)
        {
            robot_.arreter();
            etat_ = EtatPiezo::BIPS;
        }
        break;

    case EtatPiezo::BIPS:
    {
        robot_.arreter();
        bool poteauRetire = false;

        do
        {
            jouerTroisBipsGraves();
            poteauRetire = sequenceVerification();
        } while (!poteauRetire);

        etat_ = EtatPiezo::MELODIE;
        break;
    }

    case EtatPiezo::MELODIE:
        if (!sequenceFinie_)
        {
            capteurLigne_.ajusterLigneDevant();
            jouerMelodieFin();

            sequenceFinie_ = true;
            aExecute = true;
        }
        break;
    }
}