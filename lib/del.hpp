/*
 * Auteurs : Mohamed-Adam Ezzarouali, Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Définition de la classe Del, utilisée pour contrôler une DEL bicolore
 * (rouge et verte) à l'aide de deux broches du microcontrôleur. Elle déclare les
 * méthodes permettant d'allumer la DEL selon la couleur désirée, de l'éteindre ou de
 * produire la couleur ambre par alternance rapide.
 *
 * Identification matérielle :
 * Deux broches d'un même port (ex. PORTA1 et PORTA2) sont
 * configurées en sortie. Chaque broche commande une couleur de la DEL : rouge ou verte.
 */

#pragma once

#include <avr/io.h>

class Del
{
public:
    enum Couleur
    {
        ETEINT,
        ROUGE,
        VERT,
        AMBRE
    };

    Del(
        volatile uint8_t &ddr,
        volatile uint8_t &port,
        uint8_t brocheRouge,
        uint8_t brocheVerte);

    void allumer(Couleur couleur);
    void eteindre();
    void allumerAmbre();
    void ajusterCouleur(uint8_t intensiteRouge, uint8_t intensiteVerte);
    void myDelay(uint8_t cycles);
    void delAttenue();
    void ambreNonBloquant(uint8_t ratioRouge);
    void reglerSelonDistance(uint16_t distance);

private:
    volatile uint8_t &port_;
    volatile uint8_t &ddr_;
    uint8_t brocheRouge_;
    uint8_t brocheVerte_;
};