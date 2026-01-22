/*
 * Auteurs : Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Point d'entrée principal du programme. Il initialise l'ensemble du
 * robot via la classe Robot et lance la boucle d'exécution principale. Le module main
 * sert uniquement à instancier le robot et à déléguer toutes les responsabilités aux
 * différents sous-systèmes : motricité, capteurs, modules Piezo et Diode, gestion
 * de l'interrupteur, DEL RGB, PWM, USART et envoi du rapport. Toute la logique
 * fonctionnelle est donc centralisée dans la classe Robot.
 *
 * Identification matérielle :
 * Le système est exécuté sur un microcontrôleur ATmega324PA
 * cadencé à 8 MHz. L'ensemble du matériel utilisé (capteurs, piezo, DEL, PWM, USART,
 * moteurs, bouton) est initialisé et géré à travers la classe Robot.
 */

#define F_CPU 8000000UL

#include "robot.hpp"

int main()
{
    Robot robot;
    robot.executer();
}