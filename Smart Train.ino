#include <SoftwareSerial.h> 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "NewPing.h"
#include <TinyGPS++.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);  

// Ultrasonic sensors
#define TRIGGER_PIN1 4
#define ECHO_PIN1 5
NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1);

#define TRIGGER_PIN2 6
#define ECHO_PIN2 7
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2);

// GSM Module
SoftwareSerial gsmSerial(2, 3); 

// GPS Module
TinyGPSPlus gps;
SoftwareSerial gpsSerial(8, 9); // GPS TX → pin 8, RX → pin 9

// Pins
int motor = 10;
#define buzzer 11

// Timing
int _timeout;
unsigned long stime = 0;
unsigned long currentMillis;
const unsigned long period = 5000;

void setup() {
  gsmSerial.begin(9600); 
  Serial.begin(9600);  
  gpsSerial.begin(9600);

  lcd.backlight();
  lcd.init(); 
  lcd.clear();

  lcd.print("Govt Poly");
  lcd.setCursor(0, 1);
  lcd.print("        College");
  delay(3000);
  lcd.clear();

  lcd.print("     E/C");
  lcd.setCursor(0, 1);
  lcd.print("  DEPARTMENT");
  delay(3000);
  lcd.clear();

  lcd.print("smart railway station");
  lcd.setCursor(0, 1);
  lcd.print("with iot monitoring");
  delay(3000);
  lcd.clear();
  
  pinMode(motor, OUTPUT);
  pinMode(buzzer, OUTPUT);  
}

void loop() {
  updateGPS();
  messageq();
  object();
  crack();
}

// Display idle message
void messageq() {
  lcd.setCursor(1, 0);
  lcd.print("TRAIN IS");
  lcd.setCursor(0, 1);
  lcd.print("         SAFE");
  delay(100);
}

// Read GPS and return location link
String getGPSLocation() {
  if (gps.location.isValid()) {
    float latitude = gps.location.lat();
    float longitude = gps.location.lng();
    return "https://www.google.com/maps/place/" + String(latitude, 6) + "," + String(longitude, 6);
  }
  return "Location not available";
}

// Update GPS data
void updateGPS() {
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }
}

// Object Detection
void object() {
  lcd.clear();
  int distance1 = sonar1.ping_cm();
  if (distance1 > 0 && distance1 < 20) {
    lcd.clear();
    lcd.print(" object detected");
    digitalWrite(motor, LOW);
    digitalWrite(buzzer, HIGH);
    delay(2500);
    digitalWrite(buzzer, LOW);
    lcd.setCursor(0, 1);
    lcd.print("  train stop");
    delay(3000);
    lcd.clear();
    sendAlert("OBJECT DETECTED", getGPSLocation());
  } else {
    digitalWrite(buzzer, LOW);
    digitalWrite(motor, HIGH);
  }
}

// Crack Detection
void crack() {
  lcd.clear();
  int distance2 = sonar2.ping_cm();
  if (distance2 > 6 && distance2 < 20) {
    lcd.clear();
    lcd.print(" crack detected");
    digitalWrite(motor, LOW);
    digitalWrite(buzzer, HIGH);
    delay(2500);
    digitalWrite(buzzer, LOW);
    lcd.setCursor(0, 1);
    lcd.print("  train stop");
    delay(3000);
    lcd.clear();
    sendAlert("CRACK DETECTED", getGPSLocation());
  } else {
    digitalWrite(buzzer, LOW);
    digitalWrite(motor, HIGH);
  }
}

// General SMS Alert Function
void sendAlert(String type, String location) {
  String message = type + " ! " + location;
  String numbers[] = {
    "+91XXXXXXXXXX",
    "+91XXXXXXXXXX",
    "+91XXXXXXXXXX"
  };

  for (int i = 0; i < 3; i++) {
    gsmSerial.println("AT+CMGF=1");
    delay(1000);
    gsmSerial.println("AT+CMGS=\"" + numbers[i] + "\"\r");
    delay(1000);
    gsmSerial.println(message);
    delay(100);
    gsmSerial.println((char)26); // End of message
    delay(2000);
    lcd.print("GSM MESSAGE SENT");
    lcd.setCursor(0, 1);
    lcd.print(type);
    delay(3000);
    lcd.clear();
  }
}

