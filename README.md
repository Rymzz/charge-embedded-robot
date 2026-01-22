# CHARGÉ – Robot Système Embarqué

**Auteurs :** Ranya Anis, Noé Julien, Rym Zidi  
**Date :** 6 decembre 2025  

<p align="center"> <img src="https://github.com/user-attachments/assets/69a628f3-fae0-4f7f-9e06-b5db18dd104e" width="300" style="vertical-align: middle;" alt="IMG_9323" /> <img src="https://github.com/user-attachments/assets/aa074922-08c7-4cc9-a55b-470a02e2d091" width="300" style="vertical-align: middle;" alt="IMG_9326" /> <img src="https://github.com/user-attachments/assets/06ad699d-276e-43e4-b2be-ea9b1182e49a" width="300" style="vertical-align: middle;" alt="IMG_9324" /> </p>

---

## Affiche du projet

<p align="center">
  <img
    src="https://github.com/user-attachments/assets/4ba4498d-bde9-4c59-be40-0908adbca241"
    width="700"
    alt="Affiche du projet CHARGÉ"
  />
</p>

---

## Présentation générale

CHARGÉ est un **robot autonome** conçu dans le cadre d’un projet de **système embarqué**.  
Il repose sur une **machine à états finis (FSM) modulaire** assurant :

- Le suivi de ligne
- L’identification de symboles physiques
- L’exécution de comportements complexes en fonction de l’environnement ainsi que des symboles detectés
- La génération d’un rapport de parcours transmis via **USART**

Le robot est capable de parcourir **trois chemins distincts**, chacun comportant plusieurs symboles déclenchant des actions spécifiques.

---

## Fonctionnement du robot

Le robot peut suivre **trois parcours différents**, chacun pouvant commencer à partir de plusieurs centres.  
Lors de sa navigation, il détecte des **symboles disposés sur la table**.  
Lorsqu’un **poteau est placé à la suite d’un symbole**, le robot identifie le symbole correspondant et adapte automatiquement son comportement.

### Détection des symboles et comportements


| Symbole          | Comportement |
|-----------------|--------------|
| **Diode △**      | Arrêt à 10", DEL verte, pause de 2 s, puis avance jusqu’à 4" avec changement de couleur. DEL éteinte après retrait du poteau. |
| **Moteur ○**     | Contournement du poteau sans contact, puis retour sur la ligne avant le symbole suivant. |
| **Piézoélectrique □** | Arrêt à 4", activation du klaxon en boucle, reprise du trajet avec mélodie après retrait du poteau. |
| **Interrupteur _/** | Arrêt à 4", exécution d’une séquence de rotations, vérification du retrait du poteau avant de repartir. |
| **Fin de chemin** | Clignotement DEL rouge/vert, puis DEL verte après appui sur le bouton **Interrupt**, déclenchant l’envoi du rapport final via USART. |

---

## Traversée du parcours

- Le robot démarre sur des **points jaunes ou rouges**, orienté vers l’intérieur
- Les poteaux sont positionnés sur les points jaunes
- Détection du poteau à **10 pouces** et **4 pouces**
- Une réaction est exécutée selon la forme détectée
- Les résultats sont enregistrés en **EEPROM**, puis transmis en fin de parcours

### Photo du parcours

<p align="center">
  <img src="https://github.com/user-attachments/assets/eed8ad4d-7658-4daf-a333-3038293300c8" width="700" alt="Parcours du robot" />
</p>

---

## Composants matériels

- **Microcontrôleur :** ATmega324PA  
- **Actionneurs :**
  - Moteurs à courant continu
  - DELs (rouge, verte, ambre)
  - Klaxon piézoélectrique  
- **Capteurs :**
  - Capteurs infrarouges (suivi de ligne)
  - Capteur de distance (détection jusqu’à 80 cm)
  - Interrupteurs pour détection de poteaux  
- **Mémoire :** EEPROM externe I²C (24CXXX)  
- **Communication :** RS-232 via USART  

---

## Affichage et diagnostic

Affichage du parcours détecté sur la console via USART :

<p align="center">
  <img src="https://github.com/user-attachments/assets/eb00b78a-527b-4ef2-ac6e-87df8b298d1b" width="550" alt="Message USART" />
</p>

---

## Atténuation de la DEL

<div style="display: flex; justify-content: center; gap: 20px; align-items: center;">
  <img src="https://github.com/user-attachments/assets/6dee2b36-bfd1-4fef-9ad6-73c14a8a77b4" alt="led verte" width="200">
  <img src="https://github.com/user-attachments/assets/b82e69ee-93f6-48fc-8897-845d03ef9ec3" alt="led ambre" width="200">
  <img src="https://github.com/user-attachments/assets/93b977c3-30ea-4f9c-a11b-09776d13c269" alt="led rouge" width="200">
</div>



---

## Organisation logicielle

Le projet est structuré en **modules orientés objet**, facilitant la maintenance et la réutilisation du code.

### Classes principales

| Classe | Rôle |
|------|------|
| `DEL` | Gestion des DELs (couleurs, clignotement) |
| `Interruptions` | Gestion des interruptions externes et Timer1 |
| `Timer` | Configuration des minuteries et interruptions |
| `USART` | Communication série RS-232 |
| `Memoire24CXXX` | Lecture/écriture EEPROM externe |
| `CAN` | Conversion analogique-numérique |
| `PWM` | Contrôle vitesse moteurs et intensité DEL |
| `RobotMotricite` | Avancer, reculer, tourner, arrêter |
| `Debug` | Outils de diagnostic et suivi d’état |

### Structure des fichiers

- `.hpp` : déclarations des classes
- `.cpp` : implémentations
- **Makefiles :**
  - Compilation automatique
  - Lien avec `librairie1900.a`
  - Mode debug (`-DDEBUG -g`)
  - Nettoyage automatique

---

## Conclusion

Le projet CHARGÉ démontre une **intégration complète matériel-logiciel**, combinant :
- Navigation autonome
- Détection d’obstacles
- Reconnaissance de symboles
- Communication série pour diagnostic et rapport final

---

## Remerciements

Nous remercions pour leur soutien continu :

- M. Jérôme Collin  
- M. Laurent Tremblay  
- M. Romain Lebbadi-Breteau  
- M. Paul Petibon  

---

## Références

- Jérôme Collin, *Cours INF1900 – Projet initial de système embarqué*  
