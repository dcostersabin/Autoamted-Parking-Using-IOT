#include<Servo.h>


const int moduleInput = 14 ;
const int moduleOutput = 15 ;

int currentServoStatus = 0;
Servo servo;

void setup() {
  Serial.begin(9600);
  servo.attach(9);
  pinMode(moduleInput, INPUT);
  pinMode(moduleOutput, OUTPUT);
  digitalWrite(moduleOutput,HIGH);
  servo.write(0);

}

void loop() {

servoOpen();
delay(2000);
servoClose();
delay(2000);



}

void servoOpen()
{
  Serial.println("opening Gate");
  for (int i = 0 ; i < 90 ; i++)
  {
    servo.write(i);
    delay(20);
  }

  currentServoStatus = 1 ;
}

void servoClose()
{
  Serial.println("Closing Gate");
  for (int j = 90 ; j > 0 ; j--)
  {
    servo.write(j);
    delay(20);
  }

  currentServoStatus = 10 ;
}
