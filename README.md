# CHARGÉ – Robot autonome pour systèmes embarqués <br> CHARGÉ – Autonomous Embedded Robot


**Auteurs / Authors :** Ranya Anis, Noé Julien, Rym Zidi  
**Date / Date :** 6 décembre 2025 / December 6, 2025

<p align="center">
  <img src="https://github.com/user-attachments/assets/69a628f3-fae0-4f7f-9e06-b5db18dd104e" alt="IMG_9323" width="30%" />
  <img src="https://github.com/user-attachments/assets/aa074922-08c7-4cc9-a55b-470a02e2d091" alt="IMG_9326" width="30%" />
  <img src="https://github.com/user-attachments/assets/06ad699d-276e-43e4-b2be-ea9b1182e49a" alt="IMG_9324" width="30%" />
</p>

---

## Affiche du projet / Project Poster

<p align="center">
  <img
    src="https://github.com/user-attachments/assets/4ba4498d-bde9-4c59-be40-0908adbca241"
    width="700"
    alt="Affiche du projet CHARGÉ"
  />
</p>

---

## Présentation générale / General Overview

**FR:**  
CHARGÉ est un robot autonome conçu dans le cadre d’un projet de système embarqué.  
Il repose sur une machine à états finis (FSM) modulaire assurant le suivi de ligne, l’identification de symboles physiques, l’exécution de comportements complexes selon l’environnement et la génération d’un rapport via USART.  
Le robot parcourt trois chemins distincts avec différents symboles déclencheurs.

**EN:**  
CHARGÉ is an autonomous robot developed as part of an embedded systems project.  
It is based on a modular Finite State Machine (FSM) handling line following, physical symbol recognition, execution of complex behaviors based on the environment, and generation of a report transmitted via USART.  
The robot can navigate three distinct paths, each with specific trigger symbols.

---

## Fonctionnement du robot / Robot Operation

**FR:**  
Le robot peut suivre trois parcours différents, chacun pouvant commencer à partir de plusieurs points.  
Lors de sa navigation, il détecte des symboles disposés sur la table. Lorsqu’un poteau est placé à la suite d’un symbole, le robot identifie le symbole correspondant et adapte son comportement.

**EN:**  
The robot can navigate three different paths, each starting from multiple possible points.  
During navigation, it detects symbols placed on the table. When a post is placed after a symbol, the robot identifies the corresponding symbol and adapts its behavior.

### Détection des symboles et comportements / Symbol Detection and Behaviors

| Symbole / Symbol | Comportement / Behavior |
|-----------------|------------------------|
| **Diode △** | **FR:** Arrêt à 10", DEL verte, pause de 2 s, puis avance jusqu’à 4" avec changement de couleur. DEL éteinte après retrait du poteau. <br> **EN:** Stops at 10", green LED, 2 s pause, then moves to 4" with color change. LED turns off after post removal. |
| **Moteur ○** | **FR:** Contournement du poteau sans contact, puis retour sur la ligne avant le symbole suivant. <br> **EN:** Circumvents the post without contact, then returns to the line before the next symbol. |
| **Piézoélectrique □** | **FR:** Arrêt à 4", activation du klaxon en boucle, reprise du trajet avec mélodie après retrait du poteau. <br> **EN:** Stops at 4", activates buzzer in loop, resumes path with melody after post removal. |
| **Interrupteur _/** | **FR:** Arrêt à 4", exécution d’une séquence de rotations, vérification du retrait du poteau avant de repartir. <br> **EN:** Stops at 4", executes a rotation sequence, checks post removal before continuing. |
| **Fin de chemin / End of Path** | **FR:** Clignotement DEL rouge/vert, puis DEL verte après appui sur le bouton Interrupt, déclenchant l’envoi du rapport final via USART. <br> **EN:** Red/green LED blinks, then green LED after pressing Interrupt button, triggering final report via USART. |

---

## Traversée du parcours / Path Navigation

**FR:**  
- Démarrage sur points jaunes ou rouges, orienté vers l’intérieur  
- Poteaux positionnés sur points jaunes  
- Détection du poteau à 10" et 4"  
- Exécution d’actions selon symbole détecté  
- Résultats enregistrés dans EEPROM et transmis en fin de parcours

**EN:**  
- Starts on yellow or red points, facing inward  
- Posts positioned on yellow points  
- Post detected at 10" and 4"  
- Actions executed according to detected symbol  
- Results stored in EEPROM and transmitted at the end of the path

### Photo du parcours / Path Photo

<p align="center">
  <img src="https://github.com/user-attachments/assets/eed8ad4d-7658-4daf-a333-3038293300c8" width="700" alt="Parcours du robot" />
</p>

---

## Composants matériels / Hardware Components

- **Microcontrôleur / Microcontroller:** ATmega324PA  
- **Actionneurs / Actuators:**
  - Moteurs à courant continu / DC Motors
  - DELs (rouge, verte, ambre) / LEDs (red, green, amber)
  - Klaxon piézoélectrique / Piezo buzzer  
- **Capteurs / Sensors:**
  - Capteurs infrarouges (suivi de ligne) / IR line sensors
  - Capteur de distance (jusqu’à 80 cm) / Distance sensor (up to 80 cm)
  - Interrupteurs pour détection de poteaux / Switches for post detection  
- **Mémoire / Memory:** EEPROM externe I²C (24CXXX)  
- **Communication / Communication:** RS-232 via USART  

---

## Affichage et diagnostic / Display and Diagnostics

Affichage du parcours détecté sur la console via USART / Path displayed on console via USART:

<p align="center">
  <img src="https://github.com/user-attachments/assets/eb00b78a-527b-4ef2-ac6e-87df8b298d1b" width="550" alt="Message USART" />
</p>

---

## Atténuation de la DEL / LED Control

<p align="center">
  <img src="https://github.com/user-attachments/assets/6dee2b36-bfd1-4fef-9ad6-73c14a8a77b4" alt="led verte" width="30%" />
  <img src="https://github.com/user-attachments/assets/b82e69ee-93f6-48fc-8897-845d03ef9ec3" alt="led ambre" width="30%" />
  <img src="https://github.com/user-attachments/assets/93b977c3-30ea-4f9c-a11b-09776d13c269" alt="led rouge" width="30%" />
</p>

---

## Organisation logicielle / Software Organization

Le projet est structuré en modules orientés objet, facilitant maintenance et réutilisation / The project is structured in object-oriented modules, facilitating maintenance and reuse.

### Classes principales / Main Classes


| Classe / Class | Rôle / Role |
|----------------|------------|
| `DEL` | **FR:** Gestion des DELs (couleurs, clignotement) <br> **EN:** LED management (color, blinking) |
| `Interruptions` | **FR:** Gestion des interruptions externes et Timer1 <br> **EN:** External interrupts and Timer1 management |
| `Timer` | **FR:** Configuration des minuteries et interruptions <br> **EN:** Timer configuration and interrupts |
| `USART` | **FR:** Communication série RS-232 <br> **EN:** RS-232 serial communication |
| `Memoire24CXXX` | **FR:** Lecture/écriture EEPROM externe <br> **EN:** External EEPROM read/write |
| `CAN` | **FR:** Conversion analogique-numérique <br> **EN:** Analog-to-digital conversion |
| `PWM` | **FR:** Contrôle vitesse moteurs et intensité DEL <br> **EN:** Motor speed and LED intensity control |
| `RobotMotricite` | **FR:** Avancer, reculer, tourner, arrêter <br> **EN:** Move forward/backward, turn, stop |
| `Debug` | **FR:** Outils de diagnostic et suivi d’état <br> **EN:** Diagnostics and state monitoring tools |

### Structure des fichiers / File Structure

- `.hpp` : déclarations des classes / class declarations  
- `.cpp` : implémentations / implementations  
- Makefiles : compilation automatique, lien avec librairie1900.a, mode debug (`-DDEBUG -g`), nettoyage automatique / automatic compilation, link with librairie1900.a, debug mode, automatic cleanup  

---

## Conclusion / Conclusion

**FR :** Le projet CHARGÉ démontre une intégration complète matériel-logiciel, combinant :  
Navigation autonome, détection d’obstacles, reconnaissance de symboles, communication série  
**EN :** The CHARGÉ project demonstrates full hardware-software integration, combining autonomous navigation, obstacle detection, symbol recognition, and serial communication.

---

## Remerciements / Acknowledgements

Nous remercions pour leur soutien / We thank for their continuous support:

- M. Jérôme Collin  
- M. Laurent Tremblay  
- M. Romain Lebbadi-Breteau  
- M. Paul Petibon  

---

## Références / References

- Jérôme Collin, *Cours INF1900 – Projet initial de système embarqué*  
- Jérôme Collin, *INF1900 Course – Initial Embedded Systems Project*
