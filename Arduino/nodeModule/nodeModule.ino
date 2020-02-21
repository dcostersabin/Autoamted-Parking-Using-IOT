// imported files

#include <ESP8266HTTPClient.h>
#include<ESP8266WebServer.h>
#include <Arduino.h>
#ifdef ARDUINO_ARCH_SAMD
#include <WiFi101.h>
#elif defined ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#elif defined ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#error Wrong platform
#endif


// define credentials

//#define STASSID "automatedParkingProtoType"
#define STASSID "linux"
#define STAPSK "frankensteindcoster"
#define Link ""
#define ServerDelay 5000
#define id "automatedParingAgent1"





// define pins
const char* ssid = STASSID;
const char* password = STAPSK;
const int trigPin = 5;
const int blueLed = 2;
const int echoPin = 16;
const int redLed = 15 ;
const int greenLed = 13;





// define Variables
float distance = 0 ;
float duration ;




void setup()
{
  Serial.begin(115200);
  pinMode(trigPin , OUTPUT);
  pinMode(echoPin , INPUT);
  pinMode(redLed , OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(blueLed, LOW);
  wifiConnect();
  if (WiFi.status() == WL_CONNECTED)
  {

    digitalWrite(blueLed, HIGH);
    delay(200);

  }


}

void wifiConnect()
{
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);



  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }



}




void loop()
{
  checkDistance();
  
}

void checkDistance()
{
  digitalWrite(trigPin , HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin , HIGH);
  distance = ((duration / 2) / 29.1);
  Serial.println("The Distance is ");
  Serial.print(distance);

}

void red()
{
  digitalWrite(greenLed , LOW);
  digitalWrite(redLed, HIGH);

}


void green()
{
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, HIGH);
}
