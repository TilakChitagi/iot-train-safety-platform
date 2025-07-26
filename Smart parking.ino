// Created by Simple Circuits

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;

int IR1 = 2;
int IR2 = 3;
int servoPin = 4;  // Using pin 4 for the servo
int Slot = 4;      // Total parking slots (max 4)
int flag1 = 0;
int flag2 = 0;

void setup() {
    Serial.begin(9600); 
    lcd.init(); // Initialize the LCD
    lcd.backlight(); // Turn on the backlight

    pinMode(IR1, INPUT);
    pinMode(IR2, INPUT);

    myservo.attach(servoPin);
    myservo.write(100);  // Set initial servo position

    lcd.setCursor(0, 0);
    lcd.print("     ARDUINO    ");
    lcd.setCursor(0, 1);
    lcd.print(" PARKING SYSTEM ");
    delay(2000);
    lcd.clear();
}

void loop() { 
    // Car entering
    if (digitalRead(IR1) == LOW && flag1 == 0) {
        if (Slot > 0) {  // Only decrease if slots are available
            flag1 = 1;
            myservo.write(0);
            delay(1000);
            Slot--;  
            Serial.println("Car Entered. Slots left: " + String(Slot));
        } else {
            lcd.setCursor(0, 0);
            lcd.print("    SORRY :(    ");
            lcd.setCursor(0, 1);
            lcd.print("  Parking Full  ");
            delay(3000);
            lcd.clear();
        }
    }

    // Car exiting
    if (digitalRead(IR2) == LOW && flag2 == 0) {
        if (Slot < 4) {  // Prevent exceeding 4 slots
            flag2 = 1;
            myservo.write(0);
            delay(1000);
            Slot++;  
            Serial.println("Car Exited. Slots left: " + String(Slot));
        }
    }

    // Close gate when both flags are set
    if (flag1 == 1 || flag2 == 1) {
        delay(1000);
        myservo.write(100);  // Close gate
        flag1 = 0;
        flag2 = 0;
    }

    // Display available slots
    lcd.setCursor(0, 0);
    lcd.print("    WELCOME!    ");
    lcd.setCursor(0, 1);
    lcd.print("Slot Left: ");
    lcd.print(Slot);
    lcd.print("  "); // Clears extra digits if slot count changes

    delay(500);
}

