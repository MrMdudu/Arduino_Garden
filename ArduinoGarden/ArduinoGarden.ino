#include <DHT.h>
#include <SD.h>
#include <SPI.h>
#include <LiquidCrystal.h>

// Maybe temporary, soft serial used to display info on PC
// Hard Serial is used to communicate with ESP8266 module 
  #include <SoftwareSerial.h>
  
  #ifndef HAVE_HWSERIAL1
  #include "SoftwareSerial.h"
  SoftwareSerial Serial1(2, 3); // RX, TX
  #endif

// DHT 
  #define DHTPIN 11   
  #define DHTTYPE DHT22  
  DHT dht(DHTPIN, DHTTYPE);
  float h = 0;
  float t = 0;

// LCD 
  const int rs = 4, en = 5, d4 = 7, d5 = 8, d6 = 9, d7 = 10;
  LiquidCrystal LCD(rs, en, d4, d5, d6, d7);

// light sensor
  const int lumSensor = A0;
  int rawlum;
  int lux;

// soil humidity sensor
  const int soilSensor = A1;
  int rawsoil;
  int soil;
  
// Flag to make sure ESP8266 recieves enough numbers
  int flag = 0;
// delay interval variables
  const unsigned long interval = 900000; // 15min
  unsigned long currentMillis = 0;
  unsigned long previousMillis = 0;
/****************************SETUP********************************/
void setup() {
// Initialize Serial connection 
  Serial.begin(115200);
  delay(2000);
  Serial1.begin(9600);
  delay(2000);
  Serial1.println("Both Arduino serial connections have begun");
  
// DHT setup
  dht.begin();
  
// LCD setup
  LCD.begin(16,2);
  delay(100);
  LCD.clear();
  delay(100);
}
/****************************SETU_END******************************/

/*****************************LOOP*********************************/
void loop() {

  // DHT readings
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    t = dht.readTemperature();
    delay(100);
    
  // luminosity readings
    rawlum = analogRead(lumSensor);
  
  // soil humidity readings
    rawsoil = analogRead(soilSensor);
    soil = map(rawsoil,1023,0, 0, 100);
  
  // LCD display  
    LCD.home();
    if (isnan(h) || isnan(t)) {
      LCD.print("Failed to read");
      LCD.setCursor(0,1);
      LCD.print("from DHT sensor");
      flag = 1;
      delay(1000);
      return;
    } else {
      flag = 0;
      LCD.print("Humid.: ");
      LCD.print(h);
      LCD.print(" %");
      LCD.setCursor(0,1);
      LCD.print("Temp. : ");
      LCD.print(t);
      LCD.print(" *C ");
      delay(1000);
      LCD.clear();
      LCD.home();
      LCD.print("Raw lum  : ");
      LCD.print(rawlum);
      LCD.setCursor(0,1);
      LCD.print("Raw soil : ");
      LCD.print(soil);
      LCD.print(" %");
      delay(1000);
    }
    
  // Sending Data to the ESP8266 module 
  // Data is: Air humidity, Air temperature, raw luminosity, raw soil humidity
  //  if(Serial.available()){
  //    delay(10);
   //   String inSerial = Serial.readString();
   //   Serial1.println("Got message");
     // if(find_text("Waiting", inSerial) != -1){
     //if(inSerial == "Waiting\n"){

    currentMillis = millis();
    if ((unsigned long)(currentMillis - previousMillis) >= interval) {
          previousMillis = currentMillis;
          if(flag) {
            h = -1;
            t = 100;
          }
            Serial.println("Air humidity= " + String(h)
                   + ", Air temperature= " + String(t)
                   + ", raw luminosity= " + String(rawlum)
                   + ", raw soil humidity= " + String(soil));
            Serial1.println("Sent message");
    }
      //}
    //}
    delay(100);
  // Listen for eventual ESP response
    if(Serial.available()) {
      String espMessage = "";
      while(Serial.available()) {
        espMessage = Serial.readStringUntil('\n');
      }
      Serial1.println("ESP sends: " + espMessage);
    }
  
    if(Serial1.available()) {
      String pcMessage = "";
      while(Serial1.available()) {
        pcMessage = Serial1.readString();
      }
      Serial.println(pcMessage);
    }
}
/*****************************LOOP_END*********************************/

