/*
 * Auteurs : Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Implémentation de la classe Diode, responsable de la gestion
 * des transitions lumineuses et des comportements associés à la DEL du robot.
 * La classe coordonne l'état de la diode en fonction de la distance mesurée
 * par le capteur, déclenche des transitions de couleur (vert → rouge) et
 * effectue un suivi de ligne pendant les transitions. Elle orchestre également
 * une petite machine à états permettant de gérer l'approche, l'arrêt et les
 * changements de couleur à différentes distances.
 *
 * Identification matérielle :
 * La DEL est contrôlée via la classe Del et pilotée en synchronisation avec
 * le robot et les capteurs. Le capteur de distance fournit les seuils
 * déclenchant les états, et le capteur de ligne est utilisé pour maintenir
 * la trajectoire pendant les transitions de couleur. Les délais reposent sur
 * un F_CPU de 8 MHz et la librairie <util/delay.h>.
 */

#define F_CPU 8000000UL

#include "diode.hpp"

const uint16_t DUREE_TRANSITION_MS = 300;
const uint8_t CYCLE_US = 100;
const uint8_t DELAI_MICRO_US = 10;
const uint16_t DELAI_ARRET_MS = 2000;
const uint16_t DELAI_ATTENTE_MS = 100;
const uint16_t DISTANCE_10CM_MIN = 75;
const uint16_t DISTANCE_10CM_MAX = 90;
const uint16_t DISTANCE_4CM = 190;
const uint16_t DISTANCE_ATTENTE_MIN = 150;
const uint16_t DELAI_RUNTIME_SLICE_MS = 10;
const uint8_t RATIO_PERCENT = 100;

Diode::Diode(
    CapteurDistance &capteurDistance,
    Del &del,
    RobotMotricite &robot,
    CapteurLigne &capteurligne) : capteurDistance_(capteurDistance),
                                  capteurligne_(capteurligne),
                                  del_(del),
                                  robot_(robot),
                                  etat_(EtatDiode::AVANT_10)
{
}

bool Diode::avoirDelAttenue() const
{
    return delAttenue_;
}

void Diode::determinerDelAttenue(bool valeur)
{
    delAttenue_ = valeur;
}

bool Diode::avoirAExecute()
{
    return aExecute;
}

void Diode::delay_ms_runtime(uint16_t ms)
{
    uint16_t iterations = ms / DELAI_RUNTIME_SLICE_MS;
    while (iterations--)
    {
        _delay_ms(DELAI_RUNTIME_SLICE_MS);
    }
}

void Diode::fadeVertRougeNonBloquant()
{
    for (uint16_t i = 0; i < DUREE_TRANSITION_MS; i++)
    {
        uint8_t ratioRouge = (i * RATIO_PERCENT) / DUREE_TRANSITION_MS;

        uint8_t tempsRouge = (ratioRouge * CYCLE_US) / RATIO_PERCENT;
        uint8_t tempsVert = CYCLE_US - tempsRouge;

        del_.allumer(Del::VERT);
        for (uint8_t t = 0; t < tempsVert; t++)
        {
            _delay_us(DELAI_MICRO_US);
            capteurligne_.suivreLigne();
        }

        del_.allumer(Del::ROUGE);
        for (uint8_t t = 0; t < tempsRouge; t++)
        {
            _delay_us(DELAI_MICRO_US);
            capteurligne_.suivreLigne();
        }
    }
}

void Diode::executer()
{
    uint16_t distance = capteurDistance_.lireDistanceMoyenne();

    switch (etat_)
    {
    case EtatDiode::AVANT_10:
        if (distance >= DISTANCE_10CM_MIN && distance <= DISTANCE_10CM_MAX)
        {
            etat_ = EtatDiode::ARRET_10;
        }
        break;

    case EtatDiode::ARRET_10:
        robot_.arreter();
        del_.allumer(Del::VERT);
        _delay_ms(DELAI_ARRET_MS);
        etat_ = EtatDiode::COULEUR_10_4;
        break;

    case EtatDiode::COULEUR_10_4:
    {
        distance = capteurDistance_.lireDistanceMoyenne();

        if (distance > DISTANCE_4CM)
        {
            etat_ = EtatDiode::AVANT_4;
            break;
        }

        if (distance >= DISTANCE_10CM_MIN && distance <= DISTANCE_4CM)
        {
            dPrecedent_ = distance;
        }

        fadeVertRougeNonBloquant();
        etat_ = EtatDiode::AVANT_4;
        break;
    }

    case EtatDiode::AVANT_4:
        if (distance >= DISTANCE_4CM)
        {
            etat_ = EtatDiode::ARRET_4;
        }
        break;

    case EtatDiode::ARRET_4:
        robot_.arreter();
        _delay_ms(DELAI_ARRET_MS);
        distance = capteurDistance_.lireDistanceMoyenne();

        while (distance >= DISTANCE_ATTENTE_MIN)
        {
            distance = capteurDistance_.lireDistanceMoyenne();
            _delay_ms(DELAI_ATTENTE_MS);
        }

        _delay_ms(DELAI_ARRET_MS);
        etat_ = EtatDiode::AVANT_10;
        aExecute = true;
        del_.eteindre();
        break;
    }
}