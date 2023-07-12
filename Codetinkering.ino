#include <LiquidCrystal_I2C.h>

const int distancePingPin = 9;
const int distanceEchoPin = 10;
const int buttonPin = 6;
const int detectionDistance = 150;
unsigned long startTime;
unsigned long currentTime;
bool timerStop = false;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  pinMode(buttonPin, INPUT);
  startTime = 0;
  StartClock();
}

void loop() {
  currentTime = millis();
  lcd.clear();
  int distance = DistanceToObstacle();
  Serial.println(distance);

  if (distance <= detectionDistance) {
    if (timerStop == false) {
      lcd.clear();
      delay(100);
      float currentT = currentTime;
      float startT = startTime;
      if ((currentT - startT) / 1000 > 60) {
        Serial.println(currentT - startT);
        lcd.setCursor(0,0);
        lcd.print(((currentT - startT) / 1000) / 60);
        lcd.setCursor(5,0);
        lcd.print(" Minutes");
      }
      else {
        lcd.setCursor(0,0);
        lcd.print((currentT - startT) / 1000);
        lcd.setCursor(5,0);
        lcd.print(" Seconds");
        Serial.println(currentT - startT);
        delay(5000);
      }
      timerStop = true;
    }
    lcd.clear();
  }

  if (digitalRead(buttonPin) == HIGH) {
    StartClock();
  }
}

void StartClock() {
  timerStop = false;
  startTime = millis();
  lcd.clear();
  delay(100);
  lcd.setCursor(0,0);
  lcd.print("Clock Started");
  delay(500); 
}

float DistanceToObstacle() {
  delay(100);
  long duration, inches, cm;
  pinMode(distancePingPin, OUTPUT);
  digitalWrite(distancePingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(distancePingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(distancePingPin, LOW);
  pinMode(distanceEchoPin, INPUT);
  duration = pulseIn(distanceEchoPin, HIGH);
  cm = MicrosecondsToCentimeters(duration);
  return cm;
}

long MicrosecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}