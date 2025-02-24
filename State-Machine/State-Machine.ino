#include <ESP32Servo.h>
#include <BH1750.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>

BH1750 lightMeter;

const int PIN_MOTOR = 1;
const int PIN_PIR   = 2;
int greenpin = 3; // select the pin for the green LEDl

Servo sg90;
bool isRunning = true;  // Flag to control motor operation
int val;

//PIR Censor states
int pinStateCurrent = LOW;
int pinStatePrevious= LOW;

// Configuration MQTT
const char* ssid = "VotreSSID";
const char* password = "VotreMotDePasse";
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttTopic = "arduino/data";

WiFiClient espClient;
PubSubClient client(espClient);

struct SensorData {
  float lightLevel;
  int motionDetected;
};

// Definition of possible states
enum State {
  IDLE,
  ACTUATOR_CONTROL,
  ERROR
};

// Initializing current state
State currentState = IDLE;

SensorData data;
bool trigger = false;

void setup() {
  Serial.begin(9600);
  // Initializing components
  initSensors();
  initActuators();
  initMQTT();
}

void loop() {
  switch (currentState) {
    case IDLE:
      if (!client.connected()) {
        reconnectMQTT();
      }
      handleIdleState();
      break;

    case ACTUATOR_CONTROL:
      handleActuatorControlState();
      break;

    case ERROR:
      handleErrorState();
      break;

    default:
      currentState = ERROR;
      break;
  }
}

// State management functions
void handleIdleState() {
  Serial.println("État : IDLE");
  data = readSensors();
  trigger = read();
  // Transition logic
  if (conditionToRead(data, trigger)) {
    currentState = ACTUATOR_CONTROL;
  }
}

void handleActuatorControlState() {
  Serial.println("État : ACTUATOR_CONTROL");
  // Control actuators based on sensor data
  controlActuators();
  // Return to IDLE state
  currentState = IDLE;
}

void handleErrorState() {
  Serial.println("État : ERROR");
  // Error handling
  resetSystem();
  // Return to IDLE state after correction
  currentState = IDLE;
}

// Initialization functions
void initSensors() {
  // Code pour initialiser les capteurs
  Wire.begin();
  lightMeter.begin();
}

void initActuators() {
  // Code pour initialiser les actionneurs
  pinMode(PIN_PIR, INPUT);
  servoMotor.attach(1); // Broche du servo moteur
  pinMode(greenpin, OUTPUT);
}

void initMQTT() {
  WiFi.begin(ssid, password);
  Serial.print("Connexion au WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connecté");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connexion à MQTT...");
    if (client.connect("ArduinoClient")) {
      Serial.println("connecté");
      client.subscribe(mqttTopic);
    } else {
      Serial.print("Échec, rc=");
      Serial.print(client.state());
      Serial.println(" nouvelle tentative dans 5 secondes");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message reçu sur le topic: ");
  Serial.println(topic);
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println("Données reçues: " + message);
}

// Utility functions
bool conditionToRead(sensorData data, bool trigger) {
  bool actionRequire = false;
  if () actionRequire = true;
  // Condition for control actuator
  return (actionRequire || trigger);
}

SensorData readSensors() {
  // Code de lecture des capteurs
  SensorData data;
  // Lecture du capteur de lumière BH1750
  data.lightLevel = lightMeter.readLightLevel();
  Serial.print("Niveau de lumière : ");
  Serial.print(data.lightLevel);
  Serial.println(" lx");
  
  // Lecture du capteur de mouvement PIR
  data.motionDetected = digitalRead(pirPin);
  Serial.print("Mouvement détecté : ");
  Serial.println(data.motionDetected ? "Oui" : "Non");
  
  return data;
}

bool read(){
  // Code pour écouter
  client.loop();
  if () return true;
  else return false;
}

void controlActuators() {
  // Code de contrôle des actionneurs
  Serial.println("Contrôle des actionneurs...");
  
  // Exemple de contrôle du servo moteur
  servoMotor.write(90); // Tourne le servo à 90°
  delay(1000);
  servoMotor.write(0); // Retour à 0°
  
  // Exemple de contrôle de la LED
  digitalWrite(greenpin, HIGH); // Allume la LED
  delay(500);
  digitalWrite(greenpin, LOW); // Éteint la LED
}

void resetSystem() {
  // Code pour réinitialiser le système en cas d'erreur
  Serial.println("Réinitialisation du système...");
  
  // Réinitialiser les actionneurs
  servoMotor.write(0); // Remettre le servo en position initiale
  digitalWrite(greenpin, LOW); // Éteindre la LED
  
  // Vérifier les capteurs
  if (!lightMeter.begin()) {
    Serial.println("Erreur : Impossible d'initialiser le capteur BH1750");
  }
  
  // Attendre avant de redémarrer le processus
  delay(2000);
  
  Serial.println("Système réinitialisé, retour à l'état IDLE.");
  currentState = IDLE;
}
