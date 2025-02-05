# Projet IoT

Ce projet est un système de gestion automatisé des volets roulants et de la lumière en fonction de
capteurs intelligents.

📌 Objectif du Projet
Développer un système intelligent qui ajuste automatiquement l’ouverture des volets
roulants et l’allumage des lumières d’une salle en fonction des conditions ambiantes et de la
présence humaine.

## materiel utilisé

- ESP32
- Capteur de luminosité
- Capteur de présence
- Servomoteur
- LED
- Résistance
- Breadboard
- Fils de connexion
- Alimentation 5V

## Schéma logique

```mermaid
flowchart TD
    Start[Début] --> Presence{Détection de présence?}
    
    Presence -->|Non| CheckTime{Vérifier l'heure}
    CheckTime -->|Jour| DefaultDay[Position jour par défaut]
    CheckTime -->|Nuit| DefaultNight[Position nuit par défaut]
    
    Presence -->|Oui| B{Vérifier niveau de luminosité}
    B -->|Luminosité < Seuil min| C{Vérifier position volets}
    B -->|Luminosité >= Seuil min| D[Maintenir état actuel]
    
    C -->|Volets fermés| E[Ouvrir volets graduellement]
    E --> F{Vérifier nouvelle luminosité}
    F -->|Suffisante| G[Maintenir volets ouverts]
    F -->|Insuffisante| H[Allumer lumière artificielle]
    
    C -->|Volets partiellement ouverts| I[Ouvrir volets complètement]
    I --> J{Vérifier nouvelle luminosité}
    J -->|Suffisante| K[Maintenir volets ouverts]
    J -->|Insuffisante| L[Allumer lumière artificielle]
    
    C -->|Volets déjà ouverts| M[Allumer lumière artificielle]
    
    subgraph "Conditions de sécurité prioritaires"
        N{Détection vent fort?} -->|Oui| O[Fermer volets]
        P{Détection pluie?} -->|Oui| O
    end
    
    subgraph "Gestion absence prolongée"
        NoPresence[Pas de présence depuis X minutes] --> TurnOffLights[Éteindre lumières]
        TurnOffLights --> AdjustBlinds[Ajuster volets selon l'heure]
    end
```

## équipe

- [Jun](https://github.com/Juuunnne)
- [Vincent](https://github.com/Vincent-Altmann)
- [Alexandre](https://github.com/nnaova)
