#include <Servo.h>

Servo myServo;  // Create a servo object
int servoPin = 2;  // Define the pin connected to the servo

void setup() {
  myServo.attach(servoPin);  // Attach the servo to pin 2
  Serial.begin(9600);  // Begin serial communication at 9600 baud
  Serial.println("Enter a value between 0 and 180 to control the servo:");
}

void loop() {
  if (Serial.available() > 0) {
    int angle = Serial.parseInt();  // Read the input value as an integer
    if (angle >= 0 && angle <= 180) {
      myServo.write(angle);  // Set the servo position
      Serial.print("Servo moved to: ");
      Serial.println(angle);
    } else {
      Serial.println("Please enter a value between 0 and 180.");
    }
  }
}
