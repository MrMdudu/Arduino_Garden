/*
 * Code for ESP8266 in project Arduino_Garden
 * Sends data recieved from arduino board to the Google sheets
 * Using a google script
 *
 * Credit to electronics guy for HTTPSRedirect library and 
 * code example I modified to fit my project
 * Visit his GitHub at : https://github.com/electronicsguy
 * 
 * MrMdudu   15.09.2018   (version 1.0)
 * 
 */
 
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "HTTPSRedirect.h"

ESP8266WiFiMulti wifiMulti;
char c = '\n';

const char *GScriptId = "<-----------------Your script ID here----------------->";


const char* host = "script.google.com";
const char* googleRedirHost = "script.googleusercontent.com";

const int httpsPort =     443;
HTTPSRedirect client(httpsPort);

// Prepare the url (without the varying data)
String url = String("/macros/s/") + GScriptId + "/exec?";

const char* fingerprint = "F0 5C 74 77 3F 6B 25 D7 3B 66 4D 43 2F 7E BC 5B E9 28 86 AD";

/*****************************************SETUP**************************************************/
void setup() {
  Serial.begin(115200);
  delay(2000);
/**********************Wifi***************************/
// Setup Wifi connection as client station using wifiMulti to connect to the strongest signal 
// between all known wifis
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP("ssid_from_AP_1", "your_password_for_AP_1");
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");

  Serial.println("Connecting Wifi");
  int i = 0;
  while(wifiMulti.run() != WL_CONNECTED && i<20){
    delay(500);
    Serial.print(".");
    i++;
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

/*****************Google services *******************/
// Connection to google services 
  Serial.print(String("Connecting to "));
  Serial.println(host);
  
  bool flag = false;
  for (int i=0; i<5; i++){
    int retval = client.connect(host, httpsPort);
    if (retval == 1) {
                flag = true;
                break;
    }
    else
    Serial.println("Connection failed. Retrying…");
  }
// Connection Status, 1 = Connected, 0 is not.
  Serial.println("Connection Status: " + String(client.connected())? "connected" : "disconnected");
  Serial.flush();
  if (!flag){
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    Serial.println("Exiting…");
    Serial.flush();
    return;
  }

// Data will still be pushed even certification don’t match.
  if (client.verify(fingerprint, host)) {
    Serial.println("Certificate match.");
  } else {
    Serial.println("Certificate mis-match");
  }
}
/***************************************SETUP-END************************************************/

/*****************************************LOOP***************************************************/
void loop() {
  if(Serial.available()){
    delay(10);   
  // Get incoming data from the arduino board
    float h = Serial.parseFloat();
    float t = Serial.parseFloat();
    int rawlum = Serial.parseInt();
    int soil = Serial.parseInt();
    while(Serial.read() != '\n');   //emptiying input serial buffer
  // Formating the payload for the final url and sending it via Serial for debugging
    String payload = "airhum=" + String(h) + "&temp=" + String(t) 
                  + "&rawlum=" + String(rawlum) + "&soilhum=" + String(soil);
    Serial.println("Interpreted: Air humidity= " + String(h)
                 + ", Air temperature= " + String(t)
                 + ", raw luminosity= " + String(rawlum)
                 + ", raw soil humidity= " + String(soil));
    Serial.println("GET request payload: " + payload);
  // If not yet connected to client try to connect
    if (!client.connected()){
        Serial.println("Connecting to client again…");
        client.connect(host, httpsPort);
    }
  // GET request with payload
    String urlFinal = url + payload;
    client.GET(urlFinal, host);
  }
  delay(1000);
}
/****************************************LOOP_END**************************************************/
