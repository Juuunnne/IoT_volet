#include <ESP32Servo.h>
#include <BH1750.h>
#include <Wire.h>

BH1750 lightMeter;

const int PIN_MOTOR = 1;
const int PIN_PIR   = 2;
int greenpin = 3; // select the pin for the green LED

Servo sg90;
bool isRunning = true;  // Flag to control motor operation
int val;

//PIR Censor states
int pinStateCurrent = LOW;
int pinStatePrevious= LOW;

void setup() {
  Serial.begin(115200);  // Initialize serial communication
  pinMode(PIN_PIR, INPUT);
  sg90.setPeriodHertz(50);
  sg90.attach(PIN_MOTOR, 500, 2400);
  pinMode(greenpin, OUTPUT);
  Serial.println("Servo control ready. Send any character to stop/start the motor.");
  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  // For Wemos / Lolin D1 Mini Pro and the Ambient Light shield use Wire.begin(D2, D1);
  lightMeter.begin();
  Serial.println(F("BH1750 Test begin"));
}

void loop() {
  pinStatePrevious = pinStateCurrent; // store old state
  pinStateCurrent = digitalRead(PIN_PIR);
  //TEST LUMIERE + LED
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  for(val = 255; val > 0; val--){
    analogWrite(greenpin, 128 - val);
    Serial.println(val, DEC);
  }
  for(val = 0; val < 255; val++){
    analogWrite(greenpin, 128 - val);
    Serial.println(val, DEC);
  }
  //FIN TEST
  
  // Check for serial input
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');  // Clear the serial buffer
    if (message.indexOf("STOP") != -1) {
      Serial.println("Stopping the motor");
      isRunning = !isRunning;
    }
    else if(message.indexOf("START") != -1)
    {
      Serial.println("Starting the motor");
      isRunning = !isRunning;
    }

      // Toggle the running state
    if (!isRunning) {
      sg90.write(90);  // Stop at center position when halted
      Serial.println("Motor stopped");
    } else {
      Serial.println("Motor running");
    }
  }

    if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {   // pin state change: LOW -> HIGH
      Serial.println("Motion detected!");
      isRunning = !isRunning;
    }
    else if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {   // pin state change: HIGH -> LOW
      Serial.println("Motion stopped!");
      isRunning = !isRunning;
    }

  // Only run the motor movements if isRunning is true
  if (isRunning) {

    for (int pos = 0; pos <= 180; pos += 1) {
      if (!isRunning) break;  // Check if we should stop
      sg90.write(pos);
      delay(10);
    }

    // Rotation from 180 to 0 degrees
    for (int pos = 180; pos >= 0; pos -= 1) {
      if (!isRunning) break;  // Check if we should stop
      sg90.write(pos);
      delay(10);
    }
  }


}