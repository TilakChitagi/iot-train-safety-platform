#include <SoftwareSerial.h>  
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "NewPing.h"

#include <dht11.h>
#define DHT11PIN 12
dht11 DHT11;


String apiKey = "<your-api-key>";    
String Host_Name = "<your-host-name>";        
String Password = "<your-password>";         

SoftwareSerial ser(2,3);

int i = 1;
String cmd;

LiquidCrystal_I2C lcd(0x27, 16, 2); 

Servo s1;

// Pin definitions for ultrasonic sensors
#define TRIGGER_PIN1 4
#define ECHO_PIN1 5
NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1);

#define TRIGGER_PIN2 6
#define ECHO_PIN2 7
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2);


#define buzzer 13

#define ldr A1
int value1=0;

int fan = 8;
int bulb = 9;


void setup() 
{
 Serial.begin(9600); 
  Serial.begin(115200);                 
  ser.begin(115200);
  ser.println("AT+RST");              

    Serial.begin(115200);
  
  char inv = '"';
  String cmd = "AT+CWJAP";

  cmd += "=";
  cmd += inv;
  cmd += Host_Name;
  cmd += inv;
  cmd += ",";
  cmd += inv;
  cmd += Password;
  cmd += inv;
  ser.println(cmd);

  
  lcd.init();
  lcd.backlight();
  lcd.clear();
 
  lcd.print("add your text");
  lcd.setCursor(0, 1);
  lcd.print(" add your text     ");
  delay(3000);
  lcd.clear();

  lcd.print("     add your text");
  lcd.setCursor(0, 1);
  lcd.print("  add your text");
  delay(3000);
  lcd.clear();

  lcd.print("smart railway station");
  lcd.setCursor(0, 1);
  lcd.print("with iot monitoring");
  delay(3000);
  lcd.clear();
  
  pinMode(buzzer, OUTPUT);
    pinMode(ldr,INPUT_PULLUP);
    pinMode(fan,OUTPUT);
    pinMode(bulb,OUTPUT);
  s1.attach(10);


  // Initialize servos to 0 degree
  s1.write(120);
}

void loop() 
        {
          messageq();
          side1();
          side2();
          checboth1(); 

             LIGHTT();
               moisturetemp();
            EstablishTCPconnection();
            UpdatetoTS();
        }

void messageq()
{
  lcd.setCursor(1, 0);
  lcd.print("You can move");
  lcd.setCursor(0, 1);
  lcd.print("   on platform");
  delay(300);
}

void side1() 
{
lcd.clear();
  int distance1 = sonar1.ping_cm();
  if (distance1 > 0 && distance1 < 20) 
  {
     lcd.clear();
     digitalWrite(buzzer,HIGH);
      delay(1500);
       digitalWrite(buzzer,LOW);
    s1.write(120);
    lcd.print("Train ON right");
    lcd.setCursor(0, 1);
    lcd.print("Platform closed");
    delay(3000);
    lcd.clear();

  } 
  else
  {
    digitalWrite(buzzer,LOW);
  }
}

void side2() 
{
   lcd.clear();
  int distance2 = sonar2.ping_cm();
  if (distance2 > 0 && distance2 < 20) 
  {
     lcd.clear();
         digitalWrite(buzzer,HIGH);
      delay(1500);
       digitalWrite(buzzer,LOW);
    s1.write(120);
    lcd.print("Train ON Left");
    lcd.setCursor(0, 1);
    lcd.print("Platform closed");
    delay(3000);
    lcd.clear();

  } 
  else 
  {
    digitalWrite(buzzer,LOW);
  }
}

void checboth1() 
{
    int distance1 = sonar1.ping_cm();
    int distance2 = sonar2.ping_cm();

    if ((distance1 > 0 && distance1 < 20) || (distance2 > 0 && distance2 < 20)) 
        {
            s1.write(120); // Close gate
        }
    else 
      {
          s1.write(28); // Open gate
      }
}


void LIGHTT()

      {
        value1=analogRead(ldr);
        lcd.clear();
        lcd.print("LIGHT:   ");
        lcd.print(value1);
        delay(2000);
        
  if( value1 < 500)
              {
                    lcd.clear();
                    lcd.setCursor(1, 0);
                    lcd.print("DAY");
                    delay(2000);
                    lcd.clear(); 
                    digitalWrite(bulb,LOW);
                    
              }
            else
            {
                                  lcd.clear();
                    lcd.setCursor(1, 0);
                    lcd.print("NIGHT");
                    delay(2000);
                    lcd.clear();
                    digitalWrite(bulb,HIGH);
         
            } 
      }

void moisturetemp()
      {
          int chk = DHT11.read(DHT11PIN);
          lcd.setCursor(1, 0);
          lcd.print("TEMP: ");     
          lcd.print((float)DHT11.temperature);          
          lcd.setCursor(0, 1);
          lcd.print("HUMIDITY: ");  
          lcd.print((float)DHT11.humidity);                
          delay(3000);
          lcd.clear();  

            if(((float)DHT11.temperature)>38)
                {
                 lcd.setCursor(1, 0);
          lcd.print("Temperature is ");  
          lcd.setCursor(0, 1);
          lcd.print("     HIGH ");                
          delay(3000);
          lcd.clear();  
                  digitalWrite(fan,HIGH);
                  delay(2000);
                    lcd.clear();  
                }

            else
                {
                  
                  digitalWrite(fan,LOW);
                            side1();
                      side2();
                    checboth1(); 
                }
          
      }

      
void EstablishTCPconnection()
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";          
  cmd += "184.106.153.149";                     
  cmd += "\",80";                                 

  ser.println(cmd);
  Serial.println(cmd);
  if (ser.find("Error")) 
  {
    Serial.println("AT+CIPSTART error");
    return;
  }
}

void UpdatetoTS()
{
 int distance1 = sonar1.ping_cm();
   int distance2 = sonar2.ping_cm();
          value1=analogRead(ldr);
   int chk = DHT11.read(DHT11PIN);
            
  String state1 = String(value1);              
  String state2 = String(distance1);
  String state3 = String(distance2);
  String state4 = String((float)DHT11.humidity);
 String state5 = String((float)DHT11.temperature);
 
  String getStr = "GET /update?api_key=";         
  getStr += apiKey;
  getStr += "&field1=";
  getStr += String(state1);
  getStr += "&field2=";
  getStr += String(state2);
  getStr += "&field3=";
  getStr += String(state3);
  getStr += "&field4=";
  getStr += String(state4);
   getStr += "&field5=";
  getStr += String(state5);
  getStr += "\r\n\r\n";
  
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());            

  ser.println(cmd);
  Serial.println(cmd);

  if (ser.find(">")) 
  {
    ser.print(getStr);
    Serial.print(getStr);
      lcd.setCursor(0, 1);
  lcd.print("data sent");
  delay(1000);
  lcd.clear();
      ser.print(getStr);
    Serial.print(getStr);
  }
  else 
  {
    ser.println("AT+CIPCLOSE");                
    // alert user
    Serial.println("AT+CIPCLOSE");
   lcd.setCursor(1, 0);
  lcd.print("Check Internet");
  delay(1000);
  lcd.clear();
  }                                
  
}