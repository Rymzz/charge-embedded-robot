/*
 * Auteurs : Mohamed-Adam Ezzarouali, Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Définition de la classe CapteurLigne, responsable de la lecture et de
 * l'interprétation des cinq capteurs de suivi de ligne du robot. La classe fournit des
 * méthodes permettant de détecter l'état individuel des capteurs, de déterminer des
 * formes au sol (poteaux, intersections, etc.) et d'ajuster en temps réel la trajectoire
 * du robot grâce à des corrections motrices. Elle permet également la recherche de
 * ligne, la correction instantanée, et la prise de décisions selon le nombre et la
 * position des capteurs actifs.
 *
 * Identification matérielle :
 * Les capteurs de ligne sont reliés à cinq broches
 * numériques du microcontrôleur, regroupés sous un masque binaire (0b00011111). Les
 * valeurs lues permettent d'ajuster la motricité du robot via la classe RobotMotricite
 * et d'utiliser la DEL intégrée pour l'indication visuelle.
 */

#pragma once

#include <avr/io.h>

#include "lib/del.hpp"
#include "lib/robotMotricite.hpp"
#include "compFormes/formes.hpp"
#include "capteurDistance.hpp"

enum Capteurs : int8_t
{
    PERDU = 0b00000,
    D1 = 0b00001,
    D2 = 0b00010,
    D3 = 0b00100,
    D4 = 0b01000,
    D5 = 0b10000
};

class CapteurLigne
{
public:
    CapteurLigne(RobotMotricite robot);

    bool lireD1();
    bool lireD2();
    bool lireD3();
    bool lireD4();
    bool lireD5();
    void initialiserPorts();
    uint8_t lireCapteurs();
    void suivreLigne();
    void rechercherLigne();
    void devinerForme();
    void avancerTemporairement();
    void avancerAvecCorrection(uint8_t vitesse, uint16_t dureeMs);
    void avancerAvecCorrectionInstantane(uint8_t vitesse);
    void avancerPetitPas(uint8_t vitesse, uint16_t pasMs, uint8_t max);
    void ajusterLigneDevant();
    void determinerFormeActuelle(Forme forme);
    void determinerVitesseAvancer(uint8_t vitesse);
    void réinitialiserFormeActuelle();
    void executerFin();
    bool tournerGaucheAvecCheck(uint16_t duree, uint16_t stepMs);
    bool tournerDroiteAvecCheck(uint16_t duree, uint16_t stepMs);
    bool capteursActifs(uint8_t masque);
    bool avoirStationement();
    bool avancerEtTester();
    uint8_t compterCapteursActifs();
    Forme avoirForme() const
    {
        return formeActuel_;
    }
    Forme avoirFormeActuel();

private:
    static constexpr uint8_t MASQUE_TOUTS = 0b00011111;
    static constexpr uint8_t MASQUE_4_GAUCHE = 0b00001111;
    static constexpr uint8_t MASQUE_4_DROITE = 0b00011110;
    static constexpr uint8_t MASQUE_3 = 0b00000100;

    RobotMotricite robot_;
    Del led;
    Forme formeActuel_ = Forme::POTEAU_VIDE;
    uint8_t capteurs_;
    uint8_t vitesseAvancer_ = 40;
    uint8_t vitesseTourner_ = 40;
    uint8_t vitesseCorrectionFaible_ = 50;
    uint8_t vitesseCorrectionForte_ = 60;
    bool surLigne_ = false;
    bool stationnement_ = false;
};