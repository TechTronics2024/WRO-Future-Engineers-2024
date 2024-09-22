long EchoDuration;

const int rt = A2;
const int re = A3;
const int lt = A0;
const int le = A1;
const int ft = 7;
const int fe = 8;

void setup() {
Serial.begin(9600);
  pinMode(lt, OUTPUT);
  pinMode(ft, OUTPUT);
  pinMode(rt, OUTPUT);
  pinMode(re, INPUT);
  pinMode(le, INPUT);
  pinMode(fe, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  distCalc(lt, le);
   Serial.print((EchoDuration/2) / 29.1);
   Serial.print("\t");
  distCalc(ft,fe);
   Serial.print((EchoDuration/2) / 29.1);
   Serial.print("\t");
  distCalc(rt, re);
   Serial.println((EchoDuration/2) / 29.1);

}
void distCalc(int trigPin,int echoPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  EchoDuration=pulseIn(echoPin, HIGH);
 
}