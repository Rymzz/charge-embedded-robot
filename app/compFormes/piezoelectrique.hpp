/*
 * Auteurs : Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Définition de la classe Piezoelectrique, responsable de la logique du
 * module sonore lié au piezo. Elle permet de détecter la présence d'un poteau à l'aide
 * du capteur de distance, d'exécuter des séquences de bips graves tant que l'obstacle
 * est présent et de jouer une mélodie finale lorsque le poteau est retiré. La classe
 * pilote également la motricité du robot et utilise le capteur de ligne pour garder
 * la trajectoire pendant les séquences musicales.
 *
 * Identification matérielle :
 * Ce module utilise un capteur de distance (CAN), un piezo
 * contrôlé via la classe Musique, un capteur de ligne pour maintenir l'alignement, et
 * la motricité fournie par RobotMotricite. Les paramètres temporels reposent sur une
 * fréquence d'horloge de 8 MHz.
 */

#pragma once

#include <util/delay.h>

#include "lib/musique.hpp"
#include "lib/robotMotricite.hpp"
#include "composants/capteurDistance.hpp"
#include "composants/capteurLigne.hpp"

class Piezoelectrique
{
public:
    enum class EtatPiezo
    {
        AVANCE,
        BIPS,
        MELODIE
    };

    Piezoelectrique(
        CapteurDistance &capteur,
        CapteurLigne &capteurLigne,
        Musique &musique,
        RobotMotricite &robot);

    void executerComportement();
    void jouerTroisBipsGraves();
    void jouerMelodieFin();
    bool sequenceVerification();
    bool avoirAExecute();

private:
    bool detecterPoteau();

    CapteurDistance &capteur_;
    CapteurLigne &capteurLigne_;
    Musique &musique_;
    RobotMotricite &robot_;
    EtatPiezo etat_ = EtatPiezo::AVANCE;
    bool poteauPresent_ = false;
    bool sequenceFinie_ = false;
    bool aExecute = false;

    static constexpr uint16_t DISTANCE_10_PO = 30;
    static constexpr uint16_t DISTANCE_4_PO = 190;
    static constexpr uint16_t DUREE_BIP_MS = 250;
    static constexpr uint16_t DUREE_SILENCE_MS = 50;
    static constexpr uint16_t DUREE_VERIF_MS = 250;
    static constexpr uint16_t DUREE_NOTE_FIN_MS = 300;
    static constexpr uint16_t SEUIL_POTEAU_MM = 230;
    static constexpr uint8_t NB_VERIFICATIONS = 8;
    static constexpr uint8_t NB_BIP_GRAVE = 3;
    static constexpr uint8_t NOTE_BIP_GRAVE = 45;
    static constexpr uint8_t NOTE_FIN_1 = 60;
    static constexpr uint8_t NOTE_FIN_2 = 64;
    static constexpr uint8_t NOTE_FIN_3 = 67;
    static constexpr uint8_t NOTE_FIN_4 = 72;
    static constexpr uint8_t VITESSE_AVANCE = 40;
    static constexpr uint8_t VITESSE_FIN = 20;
};