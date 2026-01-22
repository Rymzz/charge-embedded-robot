# CHARGÉ – Robot Système Embarqué

**Noms :** Julien Noé, Anis Ranya, Zidi Rym  
**Date :** 28 octobre 2025  

---

## Fonctionnement du robot

Le robot peut suivre **trois parcours différents** ou trois lignes principales avec plusieurs symboles disposés à divers endroits. Chaque parcours peut commencer à plusieurs centres différents.  

### Détection des symboles

Pendant qu’il circule sur la ligne, le robot détecte les symboles (DEL, moteur, piézoélectrique, interrupteur) et exécute les actions correspondantes :  

| Symbole | Comportement |
|--------|--------------|
| DEL | Arrêt à 10" du poteau, DEL verte, pause 2s, puis déplacement de 10" à 4" avec changement de couleur. DEL éteinte après retrait du poteau. |
| Moteur | Contournement du poteau sans contact, retour sur la ligne avant le symbole suivant. |
| Piézoélectrique | Arrêt à 4", klaxon en boucle, reprise du trajet avec mélodie après retrait du poteau. |
| Interrupteur | Arrêt à 4", exécution d’une séquence de rotations, vérification du retrait du poteau avant de repartir. |
| Fin de chemin | Clignotement DEL rouge/vert puis DEL verte après appui sur le bouton **Interrupt**, déclenchant l’envoi du rapport final via USART. |

### Exemple de photo du parcours

![Parcours du robot](./images/parcours.jpg)  
*Le robot suit la ligne, détecte les symboles et exécute les actions correspondantes.

---

## Composants matériels

- **Microcontrôleur :** ATmega324PA  
- **Actionneurs :** moteurs à courant continu, DELs (rouge, verte, ambre), klaxon piézoélectrique  
- **Capteurs :** capteurs de distance, capteurs analogiques pour suivi de ligne, interrupteurs pour détection de poteaux  
- **Mémoire :** EEPROM externe I²C (24CXXX)  
- **Communication :** RS-232 via USART pour debug et diagnostic  

### Exemple de photos

Robot complet :  
![Robot complet](./images/robot_complet.jpg)

Circuit et composants :  
![Circuit et composants](./images/circuit.jpg)

Affichage ou test de la DEL :  
![DEL et test](./images/del_test.jpg)

*(Remplacez les chemins par vos photos réelles)*

---

## Librairies et organisation du code

Le code est structuré en **modules orientés objet** pour faciliter la réutilisation et la modularité.  

### Classes principales

| Classe | Fonction |
|--------|---------|
| DEL | Gestion des DELs (rouge, verte, ambre, éteint), clignotement et changements de couleur. |
| Interruptions | Configuration et gestion des interruptions externes et Timer1. |
| Timer | Interface pour configurer minuteries, PWM et interruptions périodiques. |
| USART | Communication série RS-232 pour debug et diagnostic. |
| Memoire24CXXX | Lecture/écriture EEPROM externe via I²C. |
| CAN | Conversion analogique-numérique pour capteurs. |
| PWM | Contrôle de la vitesse des moteurs et intensité DEL via Timer2 en Phase Correct PWM. |
| RobotMotricite | Gestion complète des roues : avancer, reculer, tourner, arrêter. |
| Debug | Outils de diagnostic et suivi de l’état du robot via communication série et affichage visuel. |

**Structure des fichiers :**  
- `.hpp` : déclarations de classes, attributs et méthodes  
- `.cpp` : implémentation des méthodes et comportements  

**Makefiles :**  
- Compilation automatique de tous les fichiers `.cpp`  
- Liaison avec la librairie statique `librairie1900.a`  
- Version debug avec flags `-DDEBUG -g`  
- Nettoyage automatique des fichiers objets et dépendances  

---
