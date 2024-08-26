#include <M5Stack.h>
#include <TinyGPS++.h>

#define GPS_RX 16
#define GPS_TX 17

TinyGPSPlus gps;
HardwareSerial GPSSerial(2);  // Use UART2 for GPS

void setup() {
  M5.begin();
  M5.Power.begin();
  
  Serial.begin(115200);  // Start serial communication for debugging
  GPSSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);  // Start GPS UART
  
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("GPS Test");
}

void loop() {
  static unsigned long lastDisplayTime = 0;

  while (GPSSerial.available() > 0) {
    if (gps.encode(GPSSerial.read())) {
      displayInfo();
    }
  }

  if (millis() - lastDisplayTime > 5000) {
    lastDisplayTime = millis();
    M5.Lcd.clear();
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("GPS Status:");
    M5.Lcd.print("Chars: "); M5.Lcd.println(gps.charsProcessed());
    M5.Lcd.print("Sentences: "); M5.Lcd.println(gps.sentencesWithFix());
    M5.Lcd.print("Failed: "); M5.Lcd.println(gps.failedChecksum());
    
    if (gps.location.isValid()) {
      M5.Lcd.println("Location: Valid");
      M5.Lcd.print("Lat: "); M5.Lcd.println(gps.location.lat(), 6);
      M5.Lcd.print("Lng: "); M5.Lcd.println(gps.location.lng(), 6);
    } else {
      M5.Lcd.println("Location: Not Valid");
    }
    
    M5.Lcd.print("Satellites: "); M5.Lcd.println(gps.satellites.value());
    M5.Lcd.print("HDOP: "); M5.Lcd.println(gps.hdop.value());
  }
}

void displayInfo() {
  M5.Lcd.clear();
  M5.Lcd.setCursor(0, 0);
  
  M5.Lcd.print("SATS: ");
  M5.Lcd.println(gps.satellites.value());
  
  if (gps.location.isValid()) {
    M5.Lcd.print("LAT: ");
    M5.Lcd.println(gps.location.lat(), 6);
    M5.Lcd.print("LONG: ");
    M5.Lcd.println(gps.location.lng(), 6);
  } else {
    M5.Lcd.println("LOCATION: Not Available");
  }
  
  if (gps.altitude.isValid()) {
    M5.Lcd.print("ALT: ");
    M5.Lcd.println(gps.altitude.meters());
  }
  
  if (gps.speed.isValid()) {
    M5.Lcd.print("SPEED: ");
    M5.Lcd.println(gps.speed.mps());
  }
  
  if (gps.date.isValid()) {
    M5.Lcd.print("Date: ");
    M5.Lcd.print(gps.date.month());
    M5.Lcd.print("/");
    M5.Lcd.print(gps.date.day());
    M5.Lcd.print("/");
    M5.Lcd.println(gps.date.year());
  }
  
  if (gps.time.isValid()) {
    M5.Lcd.print("Time: ");
    if (gps.time.hour() < 10) M5.Lcd.print("0");
    M5.Lcd.print(gps.time.hour());
    M5.Lcd.print(":");
    if (gps.time.minute() < 10) M5.Lcd.print("0");
    M5.Lcd.print(gps.time.minute());
    M5.Lcd.print(":");
    if (gps.time.second() < 10) M5.Lcd.print("0");
    M5.Lcd.println(gps.time.second());
  }
  
  M5.Lcd.println("---------------------------");
  delay(2000);
}
