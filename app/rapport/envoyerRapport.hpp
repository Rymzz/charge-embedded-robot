/*
 * Auteurs : Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Définition de la classe EnvoyerRapport, qui permet de générer et
 * d'envoyer via USART une représentation textuelle du trajet du robot. La classe gère
 * l'appui du bouton de déclenchement, la construction d'une carte composée de lettres,
 * de poteaux et de positions potentielles du robot, ainsi que la configuration des
 * placements selon les formes détectées (PIEZO, DIODE, MOTEUR, INTERRUPTEUR). Elle
 * fournit aussi les outils pour afficher chaque ligne, placer dynamiquement les objets
 * et réinitialiser les zones du tableau.
 *
 * Identification matérielle :
 * Le bouton physique est connecté à la broche PA5 du port A.
 * La communication série est assurée par la classe Usart. Une DEL (via la classe Del)
 * est utilisée pour indiquer le début et la fin de l'envoi. Le tableau interne compte
 * trois lignes et sept colonnes, utilisé pour représenter graphiquement le rapport final.
 */

#pragma once

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "lib/del.hpp"
#include "lib/interruption.hpp"
#include "lib/usart.hpp"
#include "compFormes/formes.hpp"
#include "compFormes/gestionInterruption.hpp"
#include "composants/capteurDistance.hpp"
#include "composants/capteurLigne.hpp"
#include "rapport/envoyerRapport.hpp"

class EnvoyerRapport
{
public:
    EnvoyerRapport(Usart &usart);

    void initialiserBouton();
    void loop();
    void genererEtEnvoyerRapport();
    void configurerPlacements(Forme forme1, Forme forme2, Forme poteau1, Forme poteau2);
    void resetDepartLigne(uint8_t ligne);
    void resetPoteau(uint8_t ligne);
    void initialiserTrajetFixe();
    void afficherLigne(uint8_t ligne);
    void setRobot(uint8_t ligne, uint8_t colonne, bool versDroite);
    void setPoteau(uint8_t ligne, uint8_t colonne);
    void envoyerTest();
    bool estAppuye();

private:
    static const uint8_t NB_LIGNES = 3;
    static const uint8_t NB_COLONNES = 7;

    Usart &usart_;
    Del del_;
    Case carte_[NB_LIGNES][NB_COLONNES];
    uint8_t nbPoteauxParLigne_[NB_LIGNES] = {0, 0, 0};

    static const int gLigne[4][4];
    static const int gColRobot[4][4];
    static const Case gTypeRobot[4][4];
    static const int gColPoteau1[4][4];
    static const int gColPoteau2[4][4];
};