#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;

// Ultrasonic sensor trig and echo pins
const int trigPin = 3;
const int echoPin = 4;

long duration; // duration of the trig and echo waves
int distance; // distance in inches

// joystick x, y and button
int xPin = A0;
int yPin = A1;
int buttonPin = 2;

// motor
int motorPin = 13;

// x,y positions of joystick and button state
int xPosition = 0;
int yPosition = 0;
int buttonState = 0;

// LED pins
int ledPin = 11;
int ledPin2 = 12;

// piezo pin
int speakerPin = 8;

// temperature sensor pin
int tempPin = A2;

// photoresistor pin
int lightPin = A3;

//Motor A
const int motorPin1  = 5;  // Pin 14 of L293
const int motorPin2  = 6;  // Pin 10 of L293
//Motor B
const int motorPin3  = 10; // Pin  7 of L293
const int motorPin4  = 9;  // Pin  2 of L293
/*
void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}
*/
void setup() { // initialize pin
  
  Serial.begin(9600); // initialize serial communications at 9600 bps:

  pinMode(trigPin, OUTPUT); // sets the trigPin as an Output
  pinMode(echoPin, INPUT); // sets the echoPin as an Input

  lcd.begin(16,2); // initialize the lcd for 16 chars 2 lines, turn on backlight
  
  pinMode(xPin, INPUT); // sets x and y pins of joystick as an input
  pinMode(yPin, INPUT);

  pinMode(ledPin, OUTPUT); // sets LEDs as an output
  pinMode(ledPin2, OUTPUT);

  pinMode(motorPin, OUTPUT); // sets motor pin as an output

  pinMode(speakerPin, OUTPUT); // sets piezo pin as an output
  
  pinMode(buttonPin, INPUT_PULLUP); // activate pull-up resistor on the push-button pin

  // set motor pins as outputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  // startup beeps
  tone(speakerPin, 100);
  delay(100);
  tone(speakerPin, 200);
  delay(100);
  tone(speakerPin, 300);
  delay(100);
  tone(speakerPin, 400);
  delay(100);
  tone(speakerPin, 500);
  delay(100);       
  noTone(speakerPin);
/*
  for (int i = 0; i < length; i++) {
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      playNote(notes[i], beats[i] * tempo);
    }
    
    // pause between notes
    delay(tempo / 2); 
  }
  */
}

void loop() { // Main loop (runs forever)

  digitalWrite(motorPin, HIGH);
  
  xPosition = analogRead(xPin);
  yPosition = analogRead(yPin);
  buttonState = digitalRead(buttonPin);
  
  Serial.print("X: ");
  Serial.print(xPosition);
  Serial.print(" | Y: ");
  Serial.print(yPosition);
  Serial.print(" | Button: ");
  Serial.println(buttonState);

 if (yPosition >= 0 && yPosition <= 50){ // Foward
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  }
 else if (yPosition <= 1023 && yPosition >= 1010){ // Backward
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH); 
  }
 else if (xPosition >= 0 && xPosition <= 50){ // Right
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  }
 else if (xPosition <= 1023 && xPosition >= 1010){ // Left
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  }
 else {
  // this code will stop motors
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  }
  
  //getting the voltage reading from the temperature sensor
  int reading = analogRead(tempPin);  
  float voltage = reading * 5.0;
  voltage /= 1024.0; 
  float temperature = ((voltage - 0.5) * 100) - 32;

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  float distance= duration*0.034/2;

  lcd.setCursor(0,0);
  lcd.print("Tmp: ");
  lcd.print(temperature);
  lcd.setCursor(10,0);
  lcd.print(" C");
  lcd.setCursor(0,1);
  lcd.print("Dist: ");
  lcd.print(distance);
  lcd.setCursor(12,1);
  lcd.print(" cm");
  
  int lightLevel = analogRead(lightPin);
  lightLevel = map(lightLevel, 0, 900, 0, 255);
  lightLevel = constrain(lightLevel, 0, 255);
  Serial.print(lightLevel); Serial.println(" black people");

  if (lightLevel <= 200){
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin2, HIGH);
    }
  else {
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, LOW);
    }

}
