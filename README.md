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
stateDiagram-v2
    direction TB
    [*] --> InitState : Démarrage système
    
    state InitState {
        [*] --> ConnectWiFi : Connexion réseau
        ConnectWiFi --> ConnectMQTT : Connexion MQTT
        ConnectMQTT --> WaitCommands : Attente commandes
    }
    
    state WaitCommands {
        direction LR
        AutoMode --> ManualMode : Ordre MQTT
        ManualMode --> AutoMode : Ordre MQTT
    }
    
    state AutoMode {
        direction TB
        [*] --> CheckPresence
        CheckPresence --> CheckLuminosity : Présence détectée
        CheckPresence --> DefaultState : Pas de présence
        
        CheckLuminosity --> AdjustBlinds : Luminosité < Seuil
        CheckLuminosity --> MaintainState : Luminosité OK
        
        AdjustBlinds --> CheckNewLuminosity
        CheckNewLuminosity --> AllumageLight : Luminosité insuffisante
        CheckNewLuminosity --> MaintainState : Luminosité suffisante
    }
    
    state ManualMode {
        direction LR
        [*] --> WaitMQTTCommands
        WaitMQTTCommands --> BlindControl : Commande Volets
        WaitMQTTCommands --> LightControl : Commande Lumière
    }
    
    state BlindControl {
        OpenBlinds
        CloseBlinds
        StopBlinds
        PartialBlinds
    }
    
    state LightControl {
        TurnOnLight
        TurnOffLight
        DimLight
    }
    
    state PublishState {
        SendCurrentStatus
        SendSensorData
    }
    
    note left of PublishState
      Remontée régulière 
      des informations
    end note
```

## Architecture

```mermaid
flowchart TB
    subgraph "Raspberry Pi"
        MQTT[Serveur MQTT Mosquitto]
        Database[Base de données]
        WebServer[Serveur Web]
    end

    subgraph "ESP32"
        Sensors[Capteurs]
        Relays[Relais volets/lumière]
        MCU[Microcontrôleur]
    end

    subgraph "Application Mobile"
        MobileApp[Interface Utilisateur]
        MobileControl[Contrôle]
        MobileDashboard[Tableau de bord]
    end

    Sensors --> |Données| MCU
    MCU --> |Publier données| MQTT
    MQTT --> |Abonner topics| MCU

    MobileApp --> |Envoyer commandes| MQTT
    MQTT --> |Transmettre commandes| MCU

    MQTT --> |Stocker données| Database
    MQTT --> |Log événements| WebServer

    MCU --> |Contrôler| Relays

    MobileApp <--> |Synchronisation| WebServer
    WebServer <--> |Stocker historique| Database

    style MQTT fill:#f9f,stroke:#333,stroke-width:4px
    style MobileApp fill:#bbf,stroke:#333,stroke-width:4px
    style ESP32 fill:#bfb,stroke:#333,stroke-width:4px
```

## équipe

- [Jun](https://github.com/Juuunnne)
- [Vincent](https://github.com/Vincent-Altmann)
- [Alexandre](https://github.com/nnaova)
