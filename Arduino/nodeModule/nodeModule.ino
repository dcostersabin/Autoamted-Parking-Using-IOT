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
#define Link "http://0e4402c0.ngrok.io"
#define Route "/arduino/request/"
#define ServerDelay 5000
#define id "1automatedParingAgent1"





// define pins
const char* ssid = STASSID;
const char* password = STAPSK;
const int trigPin = 5;
const int blueLed = 2;
const int echoPin = 16;
const int redLed = 15 ;
const int greenLed = 13;
const int gateOutput = 4;





// define Variables
float duration, distance ;
int gateStatus;
int spaceStatus ;
boolean bookedStatus;




void setup()
{
  Serial.begin(115200);
  pinMode(trigPin , OUTPUT);
  pinMode(echoPin , INPUT);
  pinMode(redLed , OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(gateOutput, OUTPUT);
  digitalWrite(blueLed, LOW);
  bookedStatus = false;
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
  if(!bookedStatus)
  {
    green();
  }
  else
  {
    red();
  }
  checkDistance();
  delay(5000);
  sendServerRequest();
  

}

void openGate()
{
  Serial.println("Opening Gate");
  digitalWrite(gateOutput, HIGH);
  gateStatus = 1;
}
void closeGate()
{
  Serial.println("Closing Gate");
  digitalWrite(gateOutput, LOW);
  gateStatus = 0;
}

void checkDistance()
{
  digitalWrite(trigPin , HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin , HIGH);
  distance = ((duration / 2) / 29.1);
  Serial.println("The Distance is ");
  Serial.println(distance);
  if(distance > 1000)
  {
    spaceStatus = 3; // sensor dirty
  }
  if(distance > 10 )
  {
    spaceStatus = 0; // free space
  }
  if(distance < 4 )
  {
    spaceStatus = 4 ; // too close to sensor
  }
  if(distance < 5 && distance > 4)
  {
    spaceStatus = 1 ; // vehicle present
  }

}

void red()
{
  bookedStatus = true;
  digitalWrite(greenLed , LOW);
  digitalWrite(redLed, HIGH);

}


void green()
{
  bookedStatus = false;
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, HIGH);
}

void sendServerRequest()
{
  HTTPClient http;
  String final_link = String(Link) + String(Route);
  String data = final_link + "?spaceStatus=" + String(spaceStatus) + "&booked=" + String(bookedStatus) +
  "&id=" + String(id) + "&gateStatus=" + String(gateStatus);
  http.begin(data);
  int httpCode = http.GET();
  String payload = http.getString();
  if(httpCode > 0 )
  {
    if(payload == "TrueFalse")
    {
      red();
      if(gateStatus == 1)
      {
        closeGate();
      }
    }
    if(payload == "FalseFalse")
    {
      green();
      if(gateStatus == 1)
      {
        closeGate();
      }
    }
    if(payload == "TrueTrue")
    {
      red();
      if(gateStatus == 0)
      {
        openGate();
      }
    }
    if(payload == "FalseTrue")
    {
      green();
      if(gateStatus == 0)
      {
        openGate(); 
      }
    }
  }
  else
  {
    Serial.println("failed to establish connection to the server");
  }
  http.end();
}
