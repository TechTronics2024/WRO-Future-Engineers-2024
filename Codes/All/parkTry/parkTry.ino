#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Servo.h>
Servo ser;

#include <Pixy2.h>
#include <Adafruit_NeoPixel.h>
// This is the main Pixy object 
Pixy2 pixy;

Adafruit_NeoPixel pixels(10, 9, NEO_GRB + NEO_KHZ800);

uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
int str = 98;
int pos=str;
int strdev = 0;

int buttonState = 0;
const int mf = 4;
const int mb = 5;
const int me = 3;
const int button = 6;

const int rt = A0;
const int re = A1;
const int lt = A2;
const int le = A3;
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
 int turnAmtL=45;
 int turnAmtR=30;

const int speed=150;
const int turnSpeed=180;

const int turnSlowSpeed=180;
bool afterReverse=false;
bool isDone=false;



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
  pinMode(button, INPUT_PULLUP);
  Serial.begin(9600);
  pixy.init();
pixels.begin();
pixels.show();
  /* Initialise the sensor */ 
  if (!bno.begin())
  {    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }else{Serial.println("BNO055 deteced");}
  ser.write(pos);
  Serial.println("Steering set");

while(buttonState==1){
buttonState=digitalRead(button);
}
  Serial.println("Motor started");
  analogWrite(me, speed);
  digitalWrite(mf, HIGH);
  digitalWrite(mb, LOW);
 // digitalWrite(ms, HIGH);

  //pixy.setLED(0, 0, 0);
  pixels.setBrightness(200);
  for (int i=0; i<10; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 255, 0));
      //pixels.setPixelColor(i, pixels.Color(0, 0, 255));
      //pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  pixels.show();
}
bool isR=true;
int turnDist=30;
bool neoGreen=true;
int blinkDelay=100;
long long prevBlink=millis();

long long prevSwitch=millis();
bool bright=true;
int switchDelay=300;
void loop(void)
{
  sensors_event_t orientationData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  pixy.ccc.getBlocks();

  bx=getAngle(&orientationData);
  fd=fdistCalc(ft,fe);
  // rd=distCalc(rt, re);
  // ld=distCalc(lt, le);

   printData();
    analogWrite(me, speed);
    if (fd<60) {
    if (millis()-prevBlink>blinkDelay) {
      prevBlink=millis();
      if (neoGreen) {
        for (int i=0; i<10; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 0, 0));
        }
        pixels.show();
        //pixy.setLED(0, 0, 0);
        Serial.println("off");
        neoGreen=false;
      }else {
        for (int i=0; i<10; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 255, 0));
        }
        pixels.show();
        Serial.println("on");
        //pixy.setLED(0, 255, 0);

        neoGreen=true;
      }
    }
    }else {
     for (int i=0; i<10; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 255, 0));
        }
        pixels.show();
        Serial.println("on");
    }

// if (lapCount>=0) {
//     if (millis()-prevSwitch>switchDelay) {
//       prevSwitch=millis();
//       if (bright) {
//         // for (int i=0; i<10; i++) {
//         //   pixels.setPixelColor(i, pixels.Color(0, 0, 0));
//         // }
//         // pixels.show();
//         pixy.setCameraBrightness(250);
//         //pixy.setLED(0, 0, 0);
//         Serial.println("off");
//         bright=false;
//       }else {
//         // for (int i=0; i<10; i++) {
//         //   pixels.setPixelColor(i, pixels.Color(0, 255, 0));
//         // }
//         // pixels.show();

//         pixy.setCameraBrightness(170);
//         Serial.println("on");
//         //pixy.setLED(0, 255, 0);

//         neoGreen=true;
//       }
//     }
//     }else {
//     //  for (int i=0; i<10; i++) {
//     //       pixels.setPixelColor(i, pixels.Color(0, 255, 0));
//     //     }
//     //     pixels.show();
//     pixy.setCameraBrightness(250);
//         Serial.println("on");
//     }
park();

   if (pixy.ccc.numBlocks&&fd>15) {

    Serial.println("pixy turn");
    if (getBigBlockH()>40){
      afterReverse=false;
    if (getBigBlock()==1) {
      turnRed();
      Serial.println("red");
    }else if(getBigBlock()==2){
      turnGreen();
      Serial.println("green");

    }
   }
  }else{
    switch (laneNum) {
  case 1:
    if ((350<bx||bx<10)) {
      if(fd<turnDist){
    turnCheck();}
  center();

  }
    break;

  case 2:
    if ((260<bx&&bx<280)) {
        if(fd<turnDist){
        turnCheck();}
  center();

    }

    break;
  case 3:
    if ((170<bx&&bx<190)) {
      if(fd<turnDist){
        turnCheck();}
 center();

    }

    break;
  case 4:
    if ((80<bx&&bx<100)) {
      if(fd<turnDist){
        turnCheck();}
 center();

    }
    break;

  }
  }
// if (laneNum!=1&&lapCount!=0) {
//   center();
// }

  if (afterReverse){
    rd=distCalc(rt, re);
    ld=distCalc(lt, le);
      if (rd<100&&isR){
        afterReverse=false;
      }
      if (ld<100&&!isR){
        afterReverse=false;
      }
  }

  // if (afterReverse) {
  // for (int i=0; i<10; i++) {
  //     pixels.setPixelColor(i, pixels.Color(0, 255, 0));
  // pixy.setLED(0, 255, 0);
  //     //pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  // }
  // }else {
  // for (int i=0; i<10; i++) {
  //     pixels.setPixelColor(i, pixels.Color(0, 0, 255));
  //     //pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  // }
  // pixy.setLED(0, 0, 255);
  // }
  
  switch (laneNum) {
    case 1:
      if (bx<180) {
        pos=str-bx*multi;
        //ser.write(pos);//left
      }else if(bx>180){
        pos=str+(360-bx)*multi;
        //ser.write(pos);//right
      }else {
        pos=str;
        //ser.write(pos);
      }
      break;

    case 2:
      if (bx<270) {
        pos=str+(270-bx)*multi;
        //ser.write(pos);//left
      }else if(bx>270){
        pos=str-(bx-270)*multi;
        //ser.write(pos);//right
      }else {
        pos=str;
        //ser.write(pos);
      }
      break;
    case 3:
      if (bx>180) {
        pos=str+(180-bx)*multi;
        //ser.write(pos);//left
      }else if(bx<180){
        pos=str-(bx-180)*multi;
        //ser.write(pos);//right
      }else {
        pos=str;
        //ser.write(pos);
      }
      break;
    case 4:
      if (bx<90) {
        pos=str+(90-bx)*multi;
        //ser.write(pos);//left
      }else if(bx>90){
        pos=str-(bx-90)*multi;
        //ser.write(pos);//right
      }else {
        pos=str;
        //ser.write(pos);
      }
      break;

  }

  if (isDone){
    int sig=0;
  int height=0;
  int coor=0;
  pixy.ccc.getBlocks();
  if (pixy.ccc.numBlocks){
  for (int i=0; i<pixy.ccc.numBlocks; i++){
    if(pixy.ccc.blocks[i].m_height>height&&pixy.ccc.blocks[i].m_signature!=3){
      height=pixy.ccc.blocks[i].m_height;
      sig=pixy.ccc.blocks[i].m_signature;
      coor=pixy.ccc.blocks[i].m_x;

    }
  }
  }
  if (sig==3){
    if(isR){
      if(coor<100){
      pixy.setLED(0, 255, 255);
      ser.write(str+20);
      delay(800);
      ser.write(str);
      digitalWrite(mf, LOW);
      digitalWrite(mb, HIGH);
      }
    }else{
      if(coor>215){
      pixy.setLED(0, 255, 255);
      ser.write(str-30);
      delay(800);
      ser.write(str);
      digitalWrite(mf, LOW);
      digitalWrite(mb, HIGH);
      }
    }
  while (true) {
    
  }
  }

  }
  if(pos>120){
    pos=120;
  }else if (pos<50) {
    pos=50;
  }
  ser.write(pos);
}

int fdSum=0;
void turnCheck(){
  Serial.print("turn check");
  for (int i=0; i<10; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 0, 0));
        }
        pixels.show();
  for (int i=0; i<5; i++) {
  fd=fdistCalc(ft, fe);
  fdSum+=fd;
  }
  if(fdSum/5>turnDist){
  if(lapCount==0 && laneNum==1) {
    rd=distCalc(rt, re);
    ld=distCalc(lt, le);
   if (rd>40) {
    isR=true;
   }else if (ld>40) {
    isR=false;
   }
  }
 if (isR) {
    rd=distCalc(rt, re);
    fd=fdistCalc(ft, fe);
    
    if (rd>100&&!afterReverse) {
      while (fd<20) {
        fd=fdistCalc(ft, fe);
      digitalWrite(mf, LOW);
      digitalWrite(mb, HIGH);
      }
      digitalWrite(mf, HIGH);
      digitalWrite(mb, LOW);
      turnR();
    }
    
   }else {
      ld=distCalc(lt, le);
       fd=fdistCalc(ft, fe);
      
    if (ld>100&&!afterReverse) {
      while (fd<20) {
        fd=fdistCalc(ft, fe);

      digitalWrite(mf, LOW);
      digitalWrite(mb, HIGH);
      }
      digitalWrite(mf, HIGH);
      digitalWrite(mb, LOW);
      turnL();

    }
   }
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
  echoD=pulseIn(echoPin, HIGH,6984);
  if (echoD==0) {
  echoD=6984;//120
  }
  echoD=(echoD/2) / 29.1;
  return(echoD); 
}


float fdistCalc(int trigPin,int echoPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  echoD=pulseIn(echoPin, HIGH,11640);
  if (echoD==0) {
  echoD=11640;//200
  }
  echoD=(echoD/2) / 29.1;
  return echoD;
  
   
}

void turnL(){
  if(!isDone){
    analogWrite(me, turnSpeed);
    ser.write(str-turnAmtL);
      delay(400);
    digitalWrite(mf, LOW);
    digitalWrite(mb, HIGH);
  }
  switch (laneNum) {
  case 1:
    while (bx>290||bx<180) {//290 to 270
      sensors_event_t orientationData;
      bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
      bx=getAngle(&orientationData);
        fd=fdistCalc(ft,fe);

        printData();

      ser.write(str+turnAmtR);
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


      ser.write(str+turnAmtR);
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

      ser.write(str+turnAmtR);
    }
    pos=str;
    ser.write(pos);
    laneNum=4;
    break;
  case 4:
     while (bx>20) { //270 to 250
      sensors_event_t orientationData;
      bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
      bx=getAngle(&orientationData);
        fd=fdistCalc(ft,fe);

        printData();

      ser.write(str+turnAmtR);
    }
    Serial.println("skfjkbjakdsnf");
    pos=str;
    ser.write(pos);
    if (lapCount>=2){
       park();
    }
    lapCount++;
    laneNum=1;
    break;
  }
  analogWrite(me, speed);
  //digitalWrite(mf, LOW);
  //digitalWrite(mb, HIGH);
  //delay(1500);
  afterReverse=true;
  digitalWrite(mf, HIGH);
  digitalWrite(mb, LOW);

}
  
void park(){
    for (int i=0; i<10; i++) {
          pixels.setPixelColor(i, pixels.Color(255, 0, 255));
        }
  pixels.show();
  int temp = turnAmtL;
  turnAmtL=-1*turnAmtR;
  turnAmtR=-1*turnAmtL;
  isDone=true;
  turnDist=50;
  // while (true) {
  
  // }
}


void turnR(){
  if(!isDone){
      analogWrite(me, turnSpeed);
      ser.write(str+turnAmtR);
      delay(400);
  digitalWrite(mf, LOW);
  digitalWrite(mb, HIGH);
  }
  switch (laneNum) {
  case 1:
    while (bx>180||bx<70) {//90 to 70
      sensors_event_t orientationData;
      bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
      bx=getAngle(&orientationData);
        fd=fdistCalc(ft,fe);

        printData();

      ser.write(str-turnAmtL);
    }
    pos=str;
    ser.write(pos);
    laneNum=4;
    break;
  case 2:
     while (bx<340) {
      sensors_event_t orientationData;
      bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
      bx=getAngle(&orientationData);
        fd=fdistCalc(ft,fe);

        printData();

      ser.write(str-turnAmtL);
    }
    pos=str;
    ser.write(pos);
    if (lapCount>=2){
      park();
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

      ser.write(str-turnAmtL);
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

      ser.write(str-turnAmtL);
    }
    Serial.println("skfjkbjakdsnf");
    pos=str;
    ser.write(pos);
    
    laneNum=3;
    break;
  }

  analogWrite(me, speed);
  
  //delay(1500);
  afterReverse=true;
  digitalWrite(mf, HIGH);
  digitalWrite(mb, LOW);
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

  
long long initial=0;
long long after = 0;
int dodgeAmtLeft=40;
int dodgeAmtRight=30;
int lastDodgeRed=true;

void turnRed(){
  // ser.write(115);
  // delay(1000);
  // ser.write(65);
  // delay(1000);
  // ser.write(90);
  int x=getBigBlockC();
  // rd=distCalc(rt, re);
  analogWrite(me, turnSlowSpeed);
  if(x<50){
    Serial.println("Red Right, Small Right Turn");
    ser.write(str);
  }
  else if (getBigBlockH()>70) {
  ser.write(str);
  while (getBigBlockH()>70) {
  digitalWrite(mf, LOW);
  digitalWrite(mb, HIGH);
  }
  digitalWrite(mf, HIGH);
  digitalWrite(mb, LOW);
  }
  else if (x>250) {
    int count = 0;
    digitalWrite(mf, LOW);
      ser.write(str-dodgeAmtLeft);
  digitalWrite(mb, HIGH);
  delay(300);
    while (getBigBlockC()>240) {
    digitalWrite(mf, LOW);
  digitalWrite(mb, HIGH);
      ser.write(str-dodgeAmtLeft);
  // if (count<3) {
  // delay(100);
  // }
  //delay(50);
      }
  digitalWrite(mf, HIGH);
  digitalWrite(mb, LOW);
   
    ser.write(str);
  }
  else {
    Serial.println("Red left, Big Right Turn");
    initial=millis();
    while (getBigBlockH()>30&&getBigBlockC()>20) {
      ser.write(str+dodgeAmtRight);
      Serial.print(getBigBlockH());
      Serial.print("\t");
      Serial.println(getBigBlockC());
    }
    delay(400);
    after=millis();
    ser.write(str-dodgeAmtLeft);
  //  Serial.println(after-initial);
    //delay(500);
    delay((after-initial)*1.6);
    ser.write(str);
    Serial.println("done");
    lastDodgeRed=true;

    // if (isR) {
    // ld=distCalc(lt, le);
    // if (ld>70) {
    // ser.write(str+dodgeAmtRight);
    // delay(400);
    // }
    // }else {
    // rd=distCalc(rt, re);
    // if (rd<30) {
    // ser.write(str-dodgeAmtLeft);
    // delay(400)
    // }
    // }
}
}

void turnGreen(){
  analogWrite(me, turnSlowSpeed);

  int x=getBigBlockC();
  if(x>265){
    Serial.println("Green Right, Small Left Turn");
     ser.write(str);
  }
  else if (getBigBlockH()>70) {
  ser.write(str);
  while (getBigBlockH()>70) {
  digitalWrite(mf, LOW);
  digitalWrite(mb, HIGH);
  }
  digitalWrite(mf, HIGH);
  digitalWrite(mb, LOW);
  }
  else if (x<50) {
  digitalWrite(mf, LOW);
    digitalWrite(mb, HIGH);
  delay(300);

  while(getBigBlockC()<25){
  ser.write(str+dodgeAmtRight);
    digitalWrite(mf, LOW);
    digitalWrite(mb, HIGH);
  }
  digitalWrite(mf, HIGH);
  digitalWrite(mb, LOW);
  
  }
  
  else {
    Serial.println("Green left, Steep Left Turn");
    initial=millis();
    while (getBigBlockH()>30&&getBigBlockC()<280) {
      ser.write(str-dodgeAmtLeft);
      Serial.print(getBigBlockH());
      Serial.print("\t");
      Serial.println(getBigBlockC());
    }
    delay(100);
    after=millis();
    ser.write(str+dodgeAmtRight);
  //  Serial.println(after-initial);
    delay((after-initial)*1.6);
    ser.write(str);
    Serial.println("done");
    lastDodgeRed=false;
    //delay(500);
  }
}

int getBigBlock(){
  int sig=0;
  int height=0;
  pixy.ccc.getBlocks();
  if (pixy.ccc.numBlocks){
  for (int i=0; i<pixy.ccc.numBlocks; i++){
    if(pixy.ccc.blocks[i].m_height>height&&pixy.ccc.blocks[i].m_signature!=3){
      height=pixy.ccc.blocks[i].m_height;
      sig=pixy.ccc.blocks[i].m_signature;
    }
  }
  }
  return sig;
}

int getBigBlockH(){
  int h =0;
  pixy.ccc.getBlocks();
  if (pixy.ccc.numBlocks){
  for (int i=0; i<pixy.ccc.numBlocks; i++){
    if(pixy.ccc.blocks[i].m_height>h&&pixy.ccc.blocks[i].m_signature!=3){
      h=pixy.ccc.blocks[i].m_height;
    }
  }
  }
  return h;
}
    
int getBigBlockC(){
  int sig=0;
  int height=0;
  int coor=0;
  pixy.ccc.getBlocks();
  if (pixy.ccc.numBlocks){
  for (int i=0; i<pixy.ccc.numBlocks; i++){
    if(pixy.ccc.blocks[i].m_height>height&&pixy.ccc.blocks[i].m_signature!=3){
      height=pixy.ccc.blocks[i].m_height;
      sig=pixy.ccc.blocks[i].m_signature;
      coor=pixy.ccc.blocks[i].m_x;

    }
  }
  }
  return coor;
}    

void center(){
  if (!isDone) {
  
  if (isR) {
    ld=distCalc(lt, le);
    if (ld<35) {
      ser.write(str+dodgeAmtRight);
      delay(100);
    }else if(ld>55){
      ser.write(str-dodgeAmtLeft);
      delay(100);
    }
  }else {
    rd=distCalc(rt, re);
    if (rd<35) {
      ser.write(str-dodgeAmtLeft);
      delay(100);
    }else if (rd>55) {
    ser.write(str+dodgeAmtRight);
      delay(100);
    }
  }
  }
}