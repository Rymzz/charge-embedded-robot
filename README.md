# CHARGÉ – Robot Système Embarqué

**Noms :** Julien Noé, Anis Ranya, Zidi Rym  
**Date :** 28 octobre 2025  

<p align="center">
  <img src="https://github.com/user-attachments/assets/69a628f3-fae0-4f7f-9e06-b5db18dd104e" width="300" style="vertical-align: middle;" alt="IMG_9323" />
  <img src="https://github.com/user-attachments/assets/aa074922-08c7-4cc9-a55b-470a02e2d091" width="300" style="vertical-align: middle;" alt="IMG_9326" />
  <img src="https://github.com/user-attachments/assets/06ad699d-276e-43e4-b2be-ea9b1182e49a" width="300" style="vertical-align: middle;" alt="IMG_9324" />
</p>


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

###  Photo du parcours

<p align="center">
  <img 
    src="https://github.com/user-attachments/assets/eed8ad4d-7658-4daf-a333-3038293300c8"
    width="700"
    alt="IMG_0821"
  />
</p>

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

Affichage du parcour detecté sur la console, via USART:  
<p align="center">
  <img src="https://github.com/user-attachments/assets/eb00b78a-527b-4ef2-ac6e-87df8b298d1b" width="546" height="143" alt="messageTransmis" />
</p>

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
