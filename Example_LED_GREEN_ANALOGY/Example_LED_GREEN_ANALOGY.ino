int greenpin =3; // select the pin for the green LED

int val;

void setup() {

  pinMode(greenpin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  for(val = 255; val > 0; val--)
  {
    analogWrite(greenpin, 128 - val);

    Serial.println(val, DEC);
    delay(5); 
  }
  for(val = 0; val < 255; val++)
  {
    analogWrite(greenpin, 128 - val);
    
    Serial.println(val, DEC);
    delay(5); 
  }
}