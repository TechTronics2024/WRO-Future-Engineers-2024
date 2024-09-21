#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Servo.h>
Servo ser;

#include <Pixy2.h>

// This is the main Pixy object 
Pixy2 pixy;

uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
int str = 98;
int pos=str;
int strdev = 0;

const int mf = 4;
const int mb = 5;
const int me = 3;
const int ms = 6;

const int rt = A2;
const int re = A3;
const int lt = A0;
const int le = A1;
const int ft = 7;
const int fe = 8;

int lapCount = 0;

float rd;
float ld;
float fd;

float bx;

long echoD;

int dif=0;

int laneNum=1;

const float multi=1;
const int turnAmt=30;
const int speed=225;
const int turnSpeed=200;

void setup(void)
{
  ser.attach(2);
  pinMode(rt, OUTPUT);
  pinMode(lt, OUTPUT);
  pinMode(ft, OUTPUT);
  pinMode(re, INPUT);
  pinMode(le, INPUT);
  pinMode(fe, INPUT);
  pinMode(mf, OUTPUT);
  pinMode(mb, OUTPUT);
  pinMode(me, OUTPUT);
  pinMode(ms, OUTPUT);
  Serial.begin(9600);
  
  /* Initialise the sensor */ 
  if (!bno.begin())
  {    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }else{Serial.println("BNO055 deteced");}
  ser.write(pos);
  Serial.println("Steering set");
  delay(1000);
  Serial.println("Motor started");
  analogWrite(me, speed);
  digitalWrite(mf, HIGH);
  digitalWrite(mb, LOW);
  digitalWrite(ms, HIGH);
}
bool isR=true;
void loop(void)
{
  sensors_event_t orientationData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);

  bx=getAngle(&orientationData);
  fd=fdistCalc(ft,fe);
  // rd=distCalc(rt, re);
  // ld=distCalc(lt, le);

   printData();
    analogWrite(me, speed);


 if(fd<90){
  if(lapCount==0 && laneNum==1){
    rd=distCalc(rt, re);
  ld=distCalc(lt, le);
   if (rd>100) {
    isR=true;
   }else if (ld>100) {
    isR=false;
   }
  }
 if (isR) {
      rd=distCalc(rt, re);
  if (rd>100) {
     turnR();
  }
   }else {
      ld=distCalc(lt, le);
    if (ld>100) {
       turnL();
    }
   }
  }
 
  switch (laneNum) {
  case 1:
    if (bx<180) {
      pos=(str-bx)*multi;
      ser.write(pos);//left
    }else if(bx>180){
      pos=(str+(360-bx))*multi;
      ser.write(pos);//right
    }else {
      pos=str;
      ser.write(pos);
    }
    break;

  case 2:
    if (bx<270) {
      pos=(str+(270-bx))*multi;
      ser.write(pos);//left
    }else if(bx>270){
      pos=(str-(bx-270))*multi;
      ser.write(pos);//right
    }else {
      pos=str;
      ser.write(pos);
    }
    break;
  case 3:
    if (bx>180) {
      pos=(str+(180-bx))*multi;
      ser.write(pos);//left
    }else if(bx<180){
      pos=(str-(bx-180))*multi;
      ser.write(pos);//right
    }else {
      pos=str;
      ser.write(pos);
    }
    break;
  case 4:
    if (bx<90) {
      pos=(str+(90-bx))*multi;
      ser.write(pos);//left
    }else if(bx>90){
      pos=(str-(bx-90))*multi;
      ser.write(pos);//right
    }else {
      pos=str;
      ser.write(pos);
    }
    break;

  }
}

float getAngle(sensors_event_t* event) {
  double x = -1000000; //dumb values, easy to spot problem
  if (event->type == SENSOR_TYPE_ORIENTATION) {
    x = event->orientation.x;
  }
  return x;
}


float distCalc(int trigPin,int echoPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  echoD=pulseIn(echoPin, HIGH);
  echoD=(echoD/2) / 29.1;
  return(echoD); 
}

float spikeDiff=100.0;
float prevDist=100.0;
float tempDist=0.0;
float fdistCalc(int trigPin,int echoPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  echoD=pulseIn(echoPin, HIGH);
  echoD=(echoD/2) / 29.1;
  if (abs(echoD-prevDist)>spikeDiff){
    tempDist=prevDist;
    prevDist=echoD;
    return(tempDist);
  }else {
    prevDist=echoD;
    return(echoD);
  }
   
}

void turnL(){
    analogWrite(me, turnSpeed);

  switch (laneNum) {
  case 1:
    while (bx>290||bx<180) {//290 to 270
      sensors_event_t orientationData;
      bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
      bx=getAngle(&orientationData);
        fd=fdistCalc(ft,fe);

        printData();

      ser.write(str-turnAmt);
    }
    pos=str;
    ser.write(pos);
    laneNum=2;
    break;
  case 2:
     while (bx>200||bx<90) {//200 to 180
      sensors_event_t orientationData;
      bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
      bx=getAngle(&orientationData);
        fd=fdistCalc(ft,fe);

        printData();


      ser.write(str-turnAmt);
    }
    pos=str;
    ser.write(pos);
    laneNum=3;
    break;

  case 3:
     while (bx>110) {//110 to 90
      sensors_event_t orientationData;
      bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
      bx=getAngle(&orientationData);
        fd=fdistCalc(ft,fe);

        printData();

      ser.write(str-turnAmt);
    }
    pos=str;
    ser.write(pos);
    laneNum=4;
    break;
  case 4:
     while (bx<250) { //270 to 250
      sensors_event_t orientationData;
      bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
      bx=getAngle(&orientationData);
        fd=fdistCalc(ft,fe);

        printData();

      ser.write(str-turnAmt);
    }
    Serial.println("skfjkbjakdsnf");
    pos=str;
    ser.write(pos);
    if (lapCount>=2){
       analogWrite(me, 0);
  digitalWrite(mf, LOW);
  digitalWrite(mb, LOW);
  digitalWrite(ms, LOW);
 // ser.detach();
    }
    lapCount++;
    laneNum=1;
    break;
  }
}
  



void turnR(){
      analogWrite(me, turnSpeed);

  switch (laneNum) {
  case 1:
    while (bx>180||bx<70) {//90 to 70
      sensors_event_t orientationData;
      bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
      bx=getAngle(&orientationData);
        fd=fdistCalc(ft,fe);

        printData();

      ser.write(str+turnAmt);
    }
    pos=str;
    ser.write(pos);
    laneNum=4;
    break;
  case 2:
     while (bx>90&&bx<340) {
      sensors_event_t orientationData;
      bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
      bx=getAngle(&orientationData);
        fd=fdistCalc(ft,fe);

        printData();

      ser.write(str+turnAmt);
    }
    pos=str;
    ser.write(pos);
    if (lapCount>=2){
       analogWrite(me, 0);
  digitalWrite(mf, LOW);
  digitalWrite(mb, LOW);
  digitalWrite(ms, LOW);
 // ser.detach();
    }
    lapCount++;
    laneNum=1;
    break;

  case 3:
     while (bx<250) {
      sensors_event_t orientationData;
      bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
      bx=getAngle(&orientationData);
        fd=fdistCalc(ft,fe);

        printData();

      ser.write(str+turnAmt);
    }
    pos=str;
    ser.write(pos);
    laneNum=2;
    break;
  case 4:
     while (bx<160||bx>270) {
      sensors_event_t orientationData;
      bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
      bx=getAngle(&orientationData);
        fd=fdistCalc(ft,fe);

        printData();

      ser.write(str+turnAmt);
    }
    Serial.println("skfjkbjakdsnf");
    pos=str;
    ser.write(pos);
    
    laneNum=3;
    break;
  }
}
  void printData(){
      Serial.print("\tx= ");
      Serial.print(bx);
      Serial.print("\tdist= ");
      Serial.print(fd);
      Serial.print("\ts angle= ");
      Serial.print(pos);
      Serial.print("\tLane = ");
      Serial.println(laneNum);
    }

  
    
