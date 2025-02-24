#include <ESP32Servo.h>

const int PIN_MOTOR = 4;
const int PIN_PIR   = 3;

Servo sg90;
bool isRunning = true;  // Flag to control motor operation

//PIR Censor states
int pinStateCurrent = LOW;
int pinStatePrevious= LOW;

void setup() {
  Serial.begin(115200);  // Initialize serial communication
  pinMode(PIN_PIR, INPUT);
  sg90.setPeriodHertz(50);
  sg90.attach(PIN_MOTOR, 500, 2400);
  Serial.println("Servo control ready. Send any character to stop/start the motor.");
}

void loop() {
  pinStatePrevious = pinStateCurrent; // store old state
  pinStateCurrent = digitalRead(PIN_PIR);
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