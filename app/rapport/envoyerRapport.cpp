/*
 * Auteurs : Noé Julien, Ranya Anis et Rym Zidi
 *
 * Description :
 * Implémentation de la classe EnvoyerRapport, responsable de la génération
 * et de la transmission d'un rapport textuel via l'interface USART. Le module permet de
 * détecter l'appui d'un bouton, de préparer une carte représentant le trajet du robot
 * (position, poteaux, lettres) et d'envoyer une visualisation structurée du parcours
 * sur trois lignes. Il gère également l'initialisation des éléments fixes du trajet,
 * la mise à jour des positions selon les formes détectées et l'envoi final via USART.
 *
 * Identification matérielle :
 * Le bouton utilisé pour déclencher l'envoi du rapport est
 * connecté à la broche PA5. La transmission série est assurée par la classe Usart. Une
 * DEL (via la classe Del) est utilisée pour indiquer l'état d'envoi du rapport. Le
 * système repose sur un microcontrôleur ATmega324PA cadencé à 8 MHz.
 */

#define F_CPU 8000000UL

#include "envoyerRapport.hpp"

const uint8_t BROCHE_BOUTON = PA5;
const uint8_t DELAI_ANTIREBOND_MS = 50;
const uint16_t DELAI_DEMARRAGE_MS = 2000;
const uint16_t DELAI_LED_ROUGE_MS = 1000;
const uint16_t DELAI_BOUCLE_MS = 10;
const uint8_t NB_LIGNES = 3;
const uint8_t NB_COLONNES = 7;
const uint8_t OFFSET_FORME_ENUM = 2;
const uint8_t COLONNE_POTEAU_1 = 2;
const uint8_t COLONNE_POTEAU_2 = 4;
const uint8_t NB_MAX_POTEAUX_PAR_LIGNE = 2;
const uint8_t INDEX_FORME_MAX = 3;
const int VALEUR_INVALIDE = -1;

EnvoyerRapport::EnvoyerRapport(Usart &usart)
    : usart_(usart), del_(DDRB, PORTB, PB0, PB1) {}

void EnvoyerRapport::initialiserBouton()
{
    DDRA &= ~(1 << BROCHE_BOUTON);
}

bool EnvoyerRapport::estAppuye()
{
    if (PINA & (1 << BROCHE_BOUTON))
    {
        _delay_ms(DELAI_ANTIREBOND_MS);
        if (PINA & (1 << BROCHE_BOUTON))
        {
            return true;
        }
    }
    return false;
}

void EnvoyerRapport::resetDepartLigne(uint8_t ligne)
{
    for (uint8_t col = 0; col < NB_COLONNES; col++)
    {
        if (carte_[ligne][col] == Case::ROBOT_DROITE ||
            carte_[ligne][col] == Case::ROBOT_GAUCHE)
        {
            carte_[ligne][col] = Case::VIDE;
        }
    }
}

void EnvoyerRapport::resetPoteau(uint8_t ligne)
{
    for (uint8_t col = 0; col < NB_COLONNES; col++)
    {
        if (carte_[ligne][col] == Case::POTEAU)
        {
            carte_[ligne][col] = Case::VIDE;
        }
    }
}

void EnvoyerRapport::initialiserTrajetFixe()
{
    carte_[0][1] = Case::LETTRE_P;
    carte_[0][3] = Case::LETTRE_I;
    carte_[0][5] = Case::LETTRE_D;

    carte_[1][1] = Case::LETTRE_M;
    carte_[1][3] = Case::LETTRE_D;
    carte_[1][5] = Case::LETTRE_P;

    carte_[2][1] = Case::LETTRE_P;
    carte_[2][3] = Case::LETTRE_M;
    carte_[2][5] = Case::LETTRE_I;
}

void EnvoyerRapport::setPoteau(uint8_t ligne, uint8_t colonneIgnore)
{
    if (ligne >= NB_LIGNES)
    {
        return;
    }

    uint8_t index = nbPoteauxParLigne_[ligne];

    uint8_t colonne = COLONNE_POTEAU_1;
    if (index == 0)
    {
        colonne = COLONNE_POTEAU_1;
    }
    else if (index == 1)
    {
        colonne = COLONNE_POTEAU_2;
    }
    else
    {
        return;
    }

    nbPoteauxParLigne_[ligne]++;

    if (colonne < NB_COLONNES)
    {
        carte_[ligne][colonne] = Case::POTEAU;
    }
}

void EnvoyerRapport::setRobot(uint8_t ligne, uint8_t colonne, bool versDroite)
{
    if (ligne < NB_LIGNES && colonne < NB_COLONNES)
    {
        carte_[ligne][colonne] = versDroite ? Case::ROBOT_DROITE : Case::ROBOT_GAUCHE;
    }
}

void EnvoyerRapport::afficherLigne(uint8_t ligne)
{
    for (uint8_t c = 0; c < NB_COLONNES; c++)
    {
        switch (carte_[ligne][c])
        {
        case Case::VIDE:
            usart_.transmettre("───");
            break;
        case Case::LETTRE_P:
            usart_.transmettre("─P─");
            break;
        case Case::LETTRE_M:
            usart_.transmettre("─M─");
            break;
        case Case::LETTRE_I:
            usart_.transmettre("─I─");
            break;
        case Case::LETTRE_D:
            usart_.transmettre("─D─");
            break;
        case Case::POTEAU:
            usart_.transmettre("(O)");
            break;
        case Case::ROBOT_DROITE:
            usart_.transmettre(">>>");
            break;
        case Case::ROBOT_GAUCHE:
            usart_.transmettre("<<<");
            break;
        default:
            usart_.transmettre("───");
            break;
        }

        if (c < NB_COLONNES - 1)
        {
            usart_.transmettre("────");
        }
    }
    usart_.transmettre("\n\n");
}

void EnvoyerRapport::genererEtEnvoyerRapport()
{
    del_.allumer(Del::ROUGE);
    _delay_ms(DELAI_LED_ROUGE_MS);

    initialiserTrajetFixe();

    usart_.transmettre("\nrobot  : CHARGE\n");
    usart_.transmettre("equipe : 5456\n\n\n");

    for (uint8_t l = 0; l < NB_LIGNES; l++)
    {
        afficherLigne(l);
    }

    usart_.transmettre("\n");

    del_.allumer(Del::VERT);
}

void EnvoyerRapport::configurerPlacements(
    Forme forme1,
    Forme forme2,
    Forme poteau1,
    Forme poteau2)
{
    uint8_t indexForme1 = static_cast<int>(forme1) - OFFSET_FORME_ENUM;
    uint8_t indexForme2 = static_cast<int>(forme2) - OFFSET_FORME_ENUM;

    if (indexForme1 < 0 || indexForme1 > INDEX_FORME_MAX ||
        indexForme2 < 0 || indexForme2 > INDEX_FORME_MAX)
    {
        return;
    }

    uint8_t ligneRobot = gLigne[indexForme1][indexForme2];
    if (ligneRobot == VALEUR_INVALIDE)
    {
        return;
    }

    resetDepartLigne(ligneRobot);
    resetPoteau(ligneRobot);

    carte_[ligneRobot][gColRobot[indexForme1][indexForme2]] =
        gTypeRobot[indexForme1][indexForme2];

    if (poteau1 == Forme::POTEAU_PRESENT &&
        gColPoteau1[indexForme1][indexForme2] != VALEUR_INVALIDE)
    {
        carte_[ligneRobot][gColPoteau1[indexForme1][indexForme2]] = Case::POTEAU;
    }

    if (poteau2 == Forme::POTEAU_PRESENT &&
        gColPoteau2[indexForme1][indexForme2] != VALEUR_INVALIDE)
    {
        carte_[ligneRobot][gColPoteau2[indexForme1][indexForme2]] = Case::POTEAU;
    }

    genererEtEnvoyerRapport();
}

void EnvoyerRapport::loop()
{
    uint16_t temps = 0;

    while (temps < DELAI_DEMARRAGE_MS)
    {
        bool boutonActuel = estAppuye();
        if (boutonActuel)
        {
            genererEtEnvoyerRapport();
        }

        _delay_ms(DELAI_BOUCLE_MS);
        temps += DELAI_BOUCLE_MS;
    }
}

const int EnvoyerRapport::gLigne[4][4] = {
    {-1, 1, 2, 0},
    {1, -1, 1, 0},
    {2, 1, -1, 2},
    {0, 0, 2, -1}};

const int EnvoyerRapport::gColRobot[4][4] = {
    {-1, 6, 0, 0},
    {2, -1, 4, 6},
    {4, 0, -1, 2},
    {4, 2, 6, -1}};

const Case EnvoyerRapport::gTypeRobot[4][4] = {
    {Case::ROBOT_GAUCHE, Case::ROBOT_GAUCHE, Case::ROBOT_DROITE, Case::ROBOT_DROITE},
    {Case::ROBOT_DROITE, Case::ROBOT_GAUCHE, Case::ROBOT_GAUCHE, Case::ROBOT_GAUCHE},
    {Case::ROBOT_GAUCHE, Case::ROBOT_DROITE, Case::ROBOT_GAUCHE, Case::ROBOT_DROITE},
    {Case::ROBOT_GAUCHE, Case::ROBOT_DROITE, Case::ROBOT_GAUCHE, Case::ROBOT_GAUCHE}};

const int EnvoyerRapport::gColPoteau1[4][4] = {
    {-1, 4, 2, 2},
    {4, -1, 2, 4},
    {2, 2, -1, 4},
    {2, 4, 4, -1}};

const int EnvoyerRapport::gColPoteau2[4][4] = {
    {-1, 2, 4, 4},
    {-1, -1, 4, 2},
    {-1, 4, -1, -1},
    {-1, -1, 2, -1}};