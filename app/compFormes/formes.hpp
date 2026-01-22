/*
 * Auteurs : Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Ce fichier regroupe les différentes énumérations utilisées dans le
 * projet pour décrire les états du robot, les formes détectées sur le parcours ainsi
 * que les éléments de la carte générée pour le rapport. Il centralise ainsi les types
 * symboliques permettant une meilleure lisibilité et une gestion uniforme des
 * comportements dans les autres modules.
 *
 * Identification matérielle :
 * Sans objet, ce fichier sert uniquement à définir des
 * constantes symboliques utilisées par les modules de détection, d'affichage, de
 * motricité et d'envoi de rapport.
 */

#pragma once

enum class Etat
{
    INIT,
    MOTEUR_DIODE1_1,
};

enum class Forme
{
    ND,
    VIDE,
    PIEZO,
    DIODE,
    MOTEUR,
    INTERRUPTEUR,
    POTEAU_PRESENT,
    POTEAU_VIDE
};

enum class Case
{
    VIDE,
    LETTRE_P,
    LETTRE_M,
    LETTRE_I,
    LETTRE_D,
    POTEAU,
    ROBOT_GAUCHE,
    ROBOT_DROITE
};
