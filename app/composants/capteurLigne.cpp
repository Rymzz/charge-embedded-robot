/*
 * Auteurs : Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Implémentation de la classe CapteurLigne, qui gère la lecture des
 * capteurs de ligne et le suivi de trajectoire du robot. La classe implémente une
 * logique de détection de formes (piezo, diode, moteur, interrupteur, poteau) basée
 * sur les patterns détectés par les cinq capteurs de ligne. Elle coordonne les
 * mouvements du robot pour maintenir la trajectoire, rechercher la ligne en cas de
 * perte, et identifier les différentes sections du parcours.
 *
 * Identification matérielle :
 * Les cinq capteurs de ligne sont connectés aux broches
 * PA0 à PA4 du port A. Une DEL de débogage est connectée au port B (PB0, PB1). Les
 * lectures sont effectuées via le registre PINA. Le système tourne sur un
 * microcontrôleur ATmega324PA à 8 MHz.
 */

#define F_CPU 8000000UL

#include "capteurLigne.hpp"
#include "robotMotricite.hpp"

#include <avr/io.h>
#include <util/delay.h>

const uint8_t BROCHE_D1 = PA0;
const uint8_t BROCHE_D2 = PA1;
const uint8_t BROCHE_D3 = PA2;
const uint8_t BROCHE_D4 = PA3;
const uint8_t BROCHE_D5 = PA4;
const uint8_t NB_CAPTEURS = 5;
const uint8_t NB_ESSAIS_MAX = 2;
const uint8_t NB_ITERATIONS_DETECTION = 5;
const uint8_t NB_STEPS_MAX = 8;
const uint8_t VITESSE_ROTATION = 45;
const uint8_t VITESSE_AVANCE_BASE = 50;
const uint8_t VITESSE_AVANCE_DROITE = 58;
const uint8_t VITESSE_AVANCE_RAPIDE = 80;
const uint8_t AJUSTEMENT_VITESSE_DROITE = 8;
const uint8_t AJUSTEMENT_CORRECTION_FAIBLE = 7;
const uint16_t DELAI_AVANCE_TEMPORAIRE_MS = 320;
const uint16_t DELAI_ARRET_TEMPORAIRE_MS = 300;
const uint16_t DELAI_RECHERCHE_COURT_MS = 1600;
const uint16_t DELAI_RECHERCHE_AVANCE_MS = 80;
const uint16_t DELAI_RECHERCHE_STEP_MS = 50;
const uint16_t DELAI_AVANCE_LONG_MS = 1100;
const uint16_t DELAI_RECUL_FIN_MS = 1600;
const uint16_t DELAI_INTERRUPTEUR_MS = 500;
const uint16_t DELAI_STEP_COURT_MS = 100;
const uint16_t DELAI_RECULER_REALIGNEMENT_MS = 80;
const uint16_t DELAI_ATTENTE_FORME_MS = 200;
const uint16_t DELAI_STEP_LONG_MS = 500;
const uint16_t DELAI_PAS_MS = 600;
const uint16_t DELAI_INITIAL_DETECTION_MS = 1000;
const uint16_t DELAI_MICRO_US = 1000;
const uint8_t SEUIL_MIN_PIEZO = 3;
const uint8_t SEUIL_MIN_DIODE = 1;
const uint8_t SEUIL_MIN_MOTEUR = 4;
const uint8_t SEUIL_DETECTION_FORME = 4;
const uint8_t SEUIL_ITERATION_DIODE = 1;
const uint8_t NB_CAPTEURS_ACTIFS_DEUX = 2;

CapteurLigne::CapteurLigne(RobotMotricite robot)
    : capteurs_(0),
      robot_(robot),
      stationnement_(false),
      led(DDRB, PORTB, PB0, PB1)
{
    initialiserPorts();
}

bool CapteurLigne::lireD1()
{
    return (PINA & (1 << BROCHE_D1));
}

bool CapteurLigne::lireD2()
{
    return (PINA & (1 << BROCHE_D2));
}

bool CapteurLigne::lireD3()
{
    return (PINA & (1 << BROCHE_D3));
}

bool CapteurLigne::lireD4()
{
    return (PINA & (1 << BROCHE_D4));
}

bool CapteurLigne::lireD5()
{
    return (PINA & (1 << BROCHE_D5));
}

void CapteurLigne::initialiserPorts()
{
    DDRA &= ~((1 << BROCHE_D1) | (1 << BROCHE_D2) | (1 << BROCHE_D3) |
              (1 << BROCHE_D4) | (1 << BROCHE_D5));
}

uint8_t CapteurLigne::lireCapteurs()
{
    capteurs_ = PINA & MASQUE_TOUTS;
    return capteurs_;
}

bool CapteurLigne::capteursActifs(uint8_t masque)
{
    return (capteurs_ & masque) == masque;
}

bool CapteurLigne::avoirStationement()
{
    return stationnement_;
}

Forme CapteurLigne::avoirFormeActuel()
{
    return formeActuel_;
}

void CapteurLigne::determinerFormeActuelle(Forme forme)
{
    formeActuel_ = forme;
}

uint8_t CapteurLigne::compterCapteursActifs()
{
    uint8_t nbActifs = 0;
    for (uint8_t i = 0; i < NB_CAPTEURS; i++)
    {
        if (capteurs_ & (1 << i))
        {
            nbActifs++;
        }
    }
    return nbActifs;
}

bool CapteurLigne::tournerGaucheAvecCheck(uint16_t duree, uint16_t stepMs)
{
    for (uint16_t t = 0; t < duree; t += stepMs)
    {
        robot_.tournerGauche(vitesseTourner_);
        for (uint16_t i = 0; i < stepMs; i++)
        {
            _delay_us(DELAI_MICRO_US);
        }
        robot_.arreter();

        if (lireCapteurs() != PERDU)
        {
            return true;
        }
    }
    return false;
}

bool CapteurLigne::avancerEtTester()
{
    robot_.avancerEnTournant(VITESSE_AVANCE_BASE, VITESSE_AVANCE_DROITE);
    _delay_ms(DELAI_AVANCE_LONG_MS);

    lireCapteurs();
    if (compterCapteursActifs() == 0)
    {
        return false;
    }
    return true;
}

void CapteurLigne::executerFin()
{
    robot_.reculer(vitesseAvancer_);
    _delay_ms(DELAI_RECUL_FIN_MS);
    robot_.arreter();
    stationnement_ = true;
    robot_.arreter();
}

void CapteurLigne::ajusterLigneDevant()
{
    bool positionDroite = false;
    while (!positionDroite)
    {
        uint8_t capteurs = lireCapteurs();

        if (capteurs & (D1 | D2))
        {
            robot_.tournerGauche(VITESSE_ROTATION);
        }
        else if (capteurs & D3 && !(capteurs & (D1 | D5)))
        {
            robot_.arreter();
            positionDroite = true;
        }
        else if (capteurs & (D4 | D5))
        {
            robot_.tournerDroite(VITESSE_ROTATION);
        }
        else
        {
            rechercherLigne();
        }
    }
}

void CapteurLigne::avancerPetitPas(uint8_t vitesse, uint16_t pasMs, uint8_t min)
{
    uint8_t stepCount = 0;

    while (stepCount < NB_STEPS_MAX)
    {
        robot_.avancerEnTournant(VITESSE_AVANCE_BASE, VITESSE_AVANCE_BASE);
        _delay_ms(DELAI_PAS_MS);
        robot_.arreter();
        lireCapteurs();

        if (compterCapteursActifs() == NB_CAPTEURS_ACTIFS_DEUX ||
            capteursActifs(MASQUE_3))
        {
            break;
        }
        else if (stepCount >= min && compterCapteursActifs() == 0)
        {
            break;
        }

        _delay_ms(DELAI_STEP_LONG_MS);
        stepCount++;
    }
}

void CapteurLigne::avancerTemporairement()
{
    _delay_ms(DELAI_INITIAL_DETECTION_MS);
    Forme forme = Forme::ND;

    robot_.avancerEnTournant(VITESSE_AVANCE_BASE, VITESSE_AVANCE_DROITE);
    _delay_ms(DELAI_AVANCE_TEMPORAIRE_MS);
    robot_.arreter();
    _delay_ms(DELAI_ARRET_TEMPORAIRE_MS);
    lireCapteurs();

    if (capteursActifs(MASQUE_TOUTS))
    {
        forme = Forme::PIEZO;
    }

    for (uint8_t i = 0; i < NB_ITERATIONS_DETECTION; i++)
    {
        robot_.avancerEnTournant(VITESSE_AVANCE_BASE, VITESSE_AVANCE_DROITE);
        _delay_ms(DELAI_AVANCE_TEMPORAIRE_MS);
        robot_.arreter();
        _delay_ms(DELAI_ARRET_TEMPORAIRE_MS);
        lireCapteurs();

        if ((capteursActifs(MASQUE_TOUTS) || capteursActifs(MASQUE_4_GAUCHE) ||
             capteursActifs(MASQUE_4_DROITE)) &&
            i < SEUIL_DETECTION_FORME)
        {
            forme = Forme::PIEZO;
        }
        else if (i > SEUIL_ITERATION_DIODE && compterCapteursActifs() > 0)
        {
            forme = Forme::DIODE;
        }
    }
    lireCapteurs();

    if (forme != Forme::PIEZO && forme != Forme::DIODE)
    {
        forme = Forme::MOTEUR;
    }

    formeActuel_ = forme;
}

void CapteurLigne::devinerForme()
{
    avancerTemporairement();
    Forme forme = formeActuel_;
    _delay_ms(DELAI_ATTENTE_FORME_MS);

    switch (forme)
    {
    case Forme::ND:
        break;

    case Forme::PIEZO:
        formeActuel_ = forme;
        avancerPetitPas(VITESSE_AVANCE_BASE, 130, SEUIL_MIN_PIEZO);
        break;

    case Forme::DIODE:
        formeActuel_ = forme;
        avancerPetitPas(VITESSE_AVANCE_BASE, 130, SEUIL_MIN_DIODE);
        break;

    case Forme::MOTEUR:
        formeActuel_ = forme;
        avancerPetitPas(VITESSE_AVANCE_BASE, 130, SEUIL_MIN_MOTEUR);
        break;

    default:
        break;
    }
}

void CapteurLigne::rechercherLigne()
{
    if (stationnement_)
    {
        return;
    }

    if (surLigne_ &&
        (formeActuel_ == Forme::PIEZO ||
         formeActuel_ == Forme::DIODE ||
         formeActuel_ == Forme::MOTEUR ||
         formeActuel_ == Forme::INTERRUPTEUR))
    {
        formeActuel_ = Forme::POTEAU_VIDE;
    }
    else if (surLigne_ && (formeActuel_ == Forme::POTEAU_PRESENT ||
                           formeActuel_ == Forme::POTEAU_VIDE ||
                           formeActuel_ == Forme::ND))
    {
        formeActuel_ = Forme::INTERRUPTEUR;
        surLigne_ = false;
        robot_.avancerEnTournant(vitesseAvancer_,
                                 vitesseAvancer_ + AJUSTEMENT_VITESSE_DROITE);
        _delay_ms(DELAI_INTERRUPTEUR_MS);
        robot_.arreter();
    }

    if (surLigne_ && !avancerEtTester())
    {
        executerFin();
        return;
    }

    surLigne_ = false;

    for (uint8_t essai = 0; essai < NB_ESSAIS_MAX; essai++)
    {
        if (lireCapteurs() != PERDU)
        {
            return;
        }

        if (tournerGaucheAvecCheck(DELAI_RECHERCHE_COURT_MS,
                                   DELAI_RECHERCHE_STEP_MS))
        {
            return;
        }

        for (uint16_t t = 0; t < DELAI_RECHERCHE_COURT_MS * 2;
             t += DELAI_RECHERCHE_STEP_MS)
        {
            robot_.tournerDroite(vitesseTourner_);
            _delay_ms(DELAI_RECHERCHE_STEP_MS);
            robot_.arreter();
            if (lireCapteurs() != PERDU)
            {
                return;
            }
        }

        if (tournerGaucheAvecCheck(DELAI_RECHERCHE_COURT_MS,
                                   DELAI_RECHERCHE_STEP_MS))
        {
            return;
        }

        _delay_ms(DELAI_ATTENTE_FORME_MS);

        for (uint16_t t = 0; t < DELAI_RECHERCHE_AVANCE_MS;
             t += DELAI_RECHERCHE_STEP_MS)
        {
            robot_.avancer(VITESSE_AVANCE_RAPIDE);
            _delay_ms(DELAI_RECHERCHE_AVANCE_MS);
            robot_.arreter();
            if (lireCapteurs() != PERDU)
            {
                return;
            }
        }
    }
    executerFin();
}

void CapteurLigne::suivreLigne()
{
    if (stationnement_)
    {
        robot_.arreter();
        return;
    }

    uint8_t capteurs = lireCapteurs();

    switch (capteurs)
    {
    case (D1 | D2 | D3 | D4 | D5):
    case (D2 | D3 | D4 | D5):
    case (D1 | D2 | D3 | D4):
        if (surLigne_ && (formeActuel_ == Forme::POTEAU_PRESENT ||
                          formeActuel_ == Forme::POTEAU_VIDE))
        {
            robot_.arreter();
            surLigne_ = false;
            devinerForme();
        }
        else
        {
            robot_.avancerEnTournant(vitesseAvancer_,
                                     vitesseAvancer_ + AJUSTEMENT_VITESSE_DROITE);
            _delay_ms(DELAI_STEP_COURT_MS);
            rechercherLigne();
        }
        break;

    case D1:
    case (D1 | D2):
    case (D1 | D2 | D3):
        robot_.avancerEnTournant(vitesseCorrectionForte_, vitesseAvancer_);
        break;

    case D2:
        robot_.avancerEnTournant(vitesseCorrectionFaible_ +
                                     AJUSTEMENT_CORRECTION_FAIBLE,
                                 vitesseAvancer_);
        break;

    case D3:
        surLigne_ = true;
        robot_.avancerEnTournant(vitesseAvancer_, vitesseAvancer_);
        break;

    case (D2 | D3):
    case (D3 | D4):
        surLigne_ = true;
        robot_.avancerEnTournant(vitesseAvancer_,
                                 vitesseAvancer_ + AJUSTEMENT_VITESSE_DROITE);
        break;

    case D4:
    case (D4 | D5):
        robot_.avancerEnTournant(vitesseAvancer_,
                                 vitesseCorrectionForte_ + AJUSTEMENT_CORRECTION_FAIBLE);
        break;

    case D5:
    case (D3 | D4 | D5):
        robot_.avancerEnTournant(vitesseAvancer_, vitesseCorrectionForte_);
        break;

    case (D1 | D5):
        robot_.reculer(vitesseAvancer_);
        _delay_ms(DELAI_RECULER_REALIGNEMENT_MS);
        break;

    case PERDU:
        if (!lireCapteurs())
        {
            if (surLigne_)
            {
                rechercherLigne();
            }
            else if (formeActuel_ == Forme::PIEZO ||
                     formeActuel_ == Forme::DIODE ||
                     formeActuel_ == Forme::MOTEUR ||
                     formeActuel_ == Forme::INTERRUPTEUR)
            {
                surLigne_ = false;
                rechercherLigne();
            }
        }
        break;

    default:
        break;
    }
}