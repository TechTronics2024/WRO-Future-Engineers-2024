#include <Servo.h>

const int trigPin1 = 3;
const int echoPin1 = 4;
const int trigPin2 = 5;
const int echoPin2 = 6;
long duration1,duration2,duration;
float distanceCm1,distanceCm2,r,l;
int count =1;
Servo ser;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(9,OUTPUT);
  ser.attach(8);
  ser.write(90);

}

void loop() {
  digitalWrite(9,HIGH);
  // put your main code here, to run repeatedly:
  r=distCalc(3,4);
  l=distCalc(5,6);
  Serial.print(r);
  Serial.print("    ");
  Serial.println(l);
  if(r<25.00&&l<25.00){
    ser.write(90);
  }else if(r<25.00){
    ser.write(180);
  }else if(l<25.00){
    ser.write(0);
  }else{
    ser.write(90);
  }

}
float distCalc(int trigPin,int echoPin)
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
return (duration/2) / 29.1;

}