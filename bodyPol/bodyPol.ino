/*
  ITP Critical Objects Midterm Spring 2019
  Morgan Mueller
  Get request from Node Server and parse the string sent over, then control motor.
*/
#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"
#include <LiquidCrystal.h>


///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// Wifi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

char serverAddress[] = "e34c8e2d.ngrok.io";  // server address

WiFiClient wifi;
HttpClient http(wifi, serverAddress);
int status = WL_IDLE_STATUS;
boolean forward = true;


//LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

unsigned long ts = millis () ;   // time accounting.
#define DELAY 20

void setup() {
  Serial.begin(9600);
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    ///////// LCD Code /////////////
    lcd.begin(16, 2); //define the LCD's size



  }


  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {


  Serial.println("making GET request");
  http.get("/stats");

  // read the status code and body of the response
  int statusCode = http.responseStatusCode();
  String response = http.responseBody();
  int lastResponse;
  int displayResponse;
  int baseSbs = 120;
  int newSbs;
  int multSbs;
  int newRes = response.toInt();
  Serial.println(response);

  lcd.clear();
  lcd.setCursor(0, 0);


  newSbs = newRes - baseSbs;
  multSbs = newSbs * 0.61;
  displayResponse = multSbs * 50000;


  if (newRes == lastResponse) {
    lcd.clear();
    lcd.print("Next patient please");
    //  lcd.setCursor(0, 1);
  }
  else {

    lcd.print("We prescribe you: ");
    lcd.setCursor(0, 1);
    lcd.print("$");
    lcd.print(displayResponse);
  }

  Serial.println();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  Serial.println("Wait five seconds");

  delay(5000);
  lastResponse = newRes;
}
