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

#pragma once

#include <util/delay.h>

#include "composants/capteurDistance.hpp"
#include "composants/capteurLigne.hpp"
#include "lib/del.hpp"
#include "lib/robotMotricite.hpp"

class Diode
{
public:
    Diode(
        CapteurDistance &capteurDistance,
        Del &del,
        RobotMotricite &robot,
        CapteurLigne &capteurligne);

    void executer();
    bool avoirDelAttenue() const;
    void determinerDelAttenue(bool valeur);
    bool avoirAExecute();
    void fadeVertRougeNonBloquant();
    void delay_ms_runtime(uint16_t ms);

private:
    enum class EtatDiode
    {
        AVANT_10,
        ARRET_10,
        COULEUR_10_4,
        AVANT_4,
        ARRET_4
    };

    CapteurDistance &capteurDistance_;
    CapteurLigne &capteurligne_;
    Del &del_;
    RobotMotricite &robot_;
    EtatDiode etat_;
    bool delAttenue_ = false;
    bool fadeTermine_ = false;
    bool aExecute = false;
    uint16_t dPrecedent_ = 0;
    int compteurStabilisation_ = 4;
};