#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>

// Initialize the I2C LCD with address 0x27 (may vary depending on the module)
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char *ssid = "<add-your-ssid>";  // Replace with your WiFi SSID
const char *pass = "<add-your-pass>";  // Replace with your WiFi password
const char *server = "api.thingspeak.com";
String apiKey = "<add-ypur-api-key>"; // ThingSpeak API key

WiFiClient client;

void setup() {
  Serial.begin(115200);  // Initialize Serial Monitor
  Serial.println("=== AQI Sensor Readings ===");

  lcd.init();  // Initialize the LCD
  lcd.backlight();  // Turn on the LCD backlight
  
  // Display Welcome message
  lcd.setCursor(0, 0);
  lcd.print("Welcome!");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("Platform No 2");
  delay(3000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi...");

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");
  delay(2000);
  lcd.clear();

  randomSeed(analogRead(0));  // Initialize random number generator
}

void loop() {
  // Generate a random AQI percentage between 35% and 60% you can change as per your need 
  int aqiPercentage = random(35, 61);  

  // Mostly show "Good", but occasionally "Moderate" when AQI is above 50
  String aqiStatus = "Good";
  if (aqiPercentage > 50 && random(0, 100) < 30) { // 30% chance to show "Moderate"
    aqiStatus = "Moderate";
  }

  Serial.print("AQI Percentage: ");
  Serial.print(aqiPercentage);
  Serial.print("% - ");
  Serial.println(aqiStatus);

  // Send data to ThingSpeak
  if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(aqiPercentage);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.println("Data sent to ThingSpeak");
  }
  client.stop();

  // Display AQI Percentage on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Air Quality Level");
  lcd.setCursor(0, 1);
  lcd.print("AQI: ");
  lcd.print(aqiPercentage);
  lcd.print("% ");
  lcd.print(aqiStatus);

  delay(15000);  // ThingSpeak requires a minimum 15-second delay between updates
}
