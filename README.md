IoT-Based Autonomous Train Safety System

This project presents an IoT-powered solution designed to enhance railway safety using modern technologies such as solar energy, ultrasonic sensors, GPS, GSM, IR, and piezoelectric sensors. The system focuses on real-time track monitoring, energy harvesting, and passenger safety with a hybrid power setup.

---

Features

- Autonomous Train Monitoring
- GPS and GSM for Location Tracking and Alerts
- Solar and Piezoelectric Energy Harvesting
- Real-time Dashboard Monitoring using ESP8266
- Obstacle and Crack Detection using IR and Ultrasonic Sensors
- LCD Display for Local Data Output
- Servo Motor for Barrier Control
- Hybrid Power System (Solar + Piezo + Battery Backup)

---

Components Used

- Arduino Uno (ATmega328P)
- ESP8266 (NodeMCU)
- Ultrasonic Sensor (HC-SR04)
- GPS Module (e.g., NEO-6M)
- GSM Module (SIM800L)
- IR Sensors
- Piezoelectric Sensor
- Solar Panel
- LCD Display (I2C)
- Servo Motor
- Battery and Charging Module
- Jumper Wires, Breadboard, Resistors, etc.

---

System Architecture

1. Train Track Monitoring:  
   Detects obstacles and cracks using IR and ultrasonic sensors.

2. Energy Harvesting:  
   Generates electricity through solar panels and piezoelectric sources.

3. Real-Time Data Logging:  
   Sends data to a dashboard using ESP8266 via Wi-Fi.

4. Safety Alerts:  
   GSM module sends alerts in case of danger or anomalies.

5. Visual Output:  
   LCD display shows live system status and voltage readings.

---

Dashboard Integration

- Platforms: ThingSpeak, Blynk, or Firebase
- Parameters Monitored:
  - Voltage from Solar and Piezo sources
  - Obstacle Detection Status
  - GPS Coordinates
  - System Health and Alerts

---

Power Supply

- Solar Panel with Rechargeable DC Battery
- Piezoelectric Pads beneath the Railway Tracks
- Backup Circuit Protection using Diodes

---

Getting Started

1. Clone the Repository:
   ```bash
   git clone https://github.com/your-username/iot-train-safety-platform.git
