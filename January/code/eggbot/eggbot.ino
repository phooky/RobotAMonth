#include <PWMServo.h>

const int gantry_enable_pin = 21;
const int gantry_in3_pin = 20;
const int gantry_in4_pin = 19;
const int gantry_limit_switch = 18;
const int gantry_station_switch = 17;
const int gantry_brake_pin = 16;

const int crane_servo_pin = 15;
const int claw_servo_pin = 14;

const int MOTOR_OFF = 0;
const int MOTOR_FORWARD = 1;
const int MOTOR_BACKWARD = 2;
const int MOTOR_BRAKE = 3;

void initGantry() {
  pinMode(gantry_enable_pin, OUTPUT);
  pinMode(gantry_in3_pin, OUTPUT);
  pinMode(gantry_in4_pin, OUTPUT);
  digitalWrite(gantry_brake_pin, LOW);
  pinMode(gantry_brake_pin, OUTPUT);
  gantryMotor(MOTOR_OFF);
  pinMode(gantry_limit_switch, INPUT_PULLUP);
  pinMode(gantry_station_switch, INPUT_PULLUP);
}

PWMServo craneServo;

void initCrane() {
  craneServo.attach(crane_servo_pin);
  setCrane(40);
}

const int CRANE_MIN = 25;
const int CRANE_MAX = 80;
int cranePos = CRANE_MIN;

void setCrane(int angle) {
  if (angle < CRANE_MIN) { angle = CRANE_MIN; }
  if (angle > CRANE_MAX) { angle = CRANE_MAX; }
  cranePos = angle;
  craneServo.write(angle);
}

const int CLAW_MIN = 8;
const int CLAW_MAX = 94;

PWMServo clawServo;
void initClaw() {
  clawServo.attach(claw_servo_pin);
  clawServo.write(90);
}

void setClaw(int angle) {
  clawServo.write(angle);
}

void gantryMotor(int direction) {
  switch(direction) {
  case MOTOR_OFF:
    digitalWrite(gantry_brake_pin, LOW);
    digitalWrite(gantry_enable_pin, LOW);
    digitalWrite(gantry_in3_pin, LOW);
    digitalWrite(gantry_in4_pin, LOW);
    break;
  case MOTOR_FORWARD:
    digitalWrite(gantry_brake_pin, LOW);
    digitalWrite(gantry_enable_pin, LOW);
    digitalWrite(gantry_in3_pin, HIGH);
    digitalWrite(gantry_in4_pin, LOW);
    digitalWrite(gantry_enable_pin, HIGH);
    break;
  case MOTOR_BACKWARD:
    digitalWrite(gantry_brake_pin, LOW);
    digitalWrite(gantry_enable_pin, LOW);
    digitalWrite(gantry_in3_pin, LOW);
    digitalWrite(gantry_in4_pin, HIGH);
    digitalWrite(gantry_enable_pin, HIGH);
    break;
  case MOTOR_BRAKE:
    digitalWrite(gantry_enable_pin, LOW);
    digitalWrite(gantry_in3_pin, LOW);
    digitalWrite(gantry_in4_pin, LOW);
    digitalWrite(gantry_brake_pin, HIGH);
    break;
  }
}

void gantryHome() {
  // Clumsily PWM the motor to limit speed.
  gantryMotor(MOTOR_BACKWARD);
  while (digitalRead(gantry_limit_switch) == HIGH);
  gantryMotor(MOTOR_BRAKE);
  delay(500);
  gantryMotor(MOTOR_OFF);
}

void debounceWaitFor(int pin, int value) {
  int state = digitalRead(pin);
  delayMicroseconds(40);
  int newState = digitalRead(pin);
  while ((newState != state) || (value != state)) {
    state = newState;
    newState = digitalRead(pin);
    delayMicroseconds(40);
  }
}

void gantryNextStation() {
  gantryMotor(MOTOR_FORWARD);
  debounceWaitFor(gantry_station_switch,HIGH);
  debounceWaitFor(gantry_station_switch,LOW);
  gantryMotor(MOTOR_BRAKE);
  delay(500);
  gantryMotor(MOTOR_OFF);
}

void setup() {
 initGantry();
 initCrane();
 initClaw();
 Serial.begin(19200);
}

void loop() {
  if (Serial.available()) {
    int c = Serial.read();
    switch (c) {
      case '1': setCrane(40); break;
      case '2': setCrane(50); break;
      case '3': setCrane(60); break;
      case '4': setCrane(70); break;
      case '5': setCrane(80); break;
      case '6': setCrane(90); break;
      case '7': setCrane(100); break;
      case '8': setCrane(110); break;
      case '9': setCrane(120); break;
      case '0': setCrane(130); break;
      case '+':
      case '=':
        setCrane(cranePos + 1);
        Serial.println(cranePos);
        break;
      case 'h':
      case 'H':
        Serial.println("Homing.");
        gantryHome();
        break;
      case 's':
      case 'S':
        Serial.println("Next station.");
        gantryNextStation();
        break;
      case '-':
        setCrane(cranePos - 1);
        Serial.println(cranePos);
        break;
    }
  }
  //digitalWrite(11,digitalRead(gantry_limit_switch));
//  digitalWrite(11,LOW);
//  gantryHome();
//  delay(1000);
//  digitalWrite(11,HIGH);
//  gantryNextStation();
//  delay(1000);
//  gantryNextStation();
//  delay(1000);
  //gantryNextStation();
  //delay(1000);
}
  

