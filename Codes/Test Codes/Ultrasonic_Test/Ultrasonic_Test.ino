long EchoDuration;
void setup() {
Serial.begin(9600);
pinMode(A2, OUTPUT);
pinMode(A3, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  distCalc(A2,A3);

}
void distCalc(int trigPin,int echoPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  EchoDuration=pulseIn(echoPin, HIGH);
  Serial.println((EchoDuration/2) / 29.1);
}