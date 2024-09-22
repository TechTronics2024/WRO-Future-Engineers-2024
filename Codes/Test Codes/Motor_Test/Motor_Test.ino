const int mf = 5;
const int mb = 4;
const int me = 3;
const int ms = 6;
void setup() {
  // put your setup code here, to run once:
pinMode(mf, OUTPUT);
pinMode(mb, OUTPUT);
pinMode(me, OUTPUT);
pinMode(ms, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(me, 255);
  digitalWrite(mf, HIGH);
  digitalWrite(mb, LOW);
  digitalWrite(ms, HIGH);
}
