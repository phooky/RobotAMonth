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


const int CRANE_MIN = 20;
const int CRANE_MAX = 105;
int cranePos;

const int CRANE_UP = CRANE_MAX;
const int CRANE_LOAD = 75;
const int CRANE_DOWN = CRANE_MIN;

void initCrane() {
  craneServo.attach(crane_servo_pin);
  cranePos = CRANE_UP;
  craneServo.write(CRANE_UP);
}

void setCrane(int angle) {
  if (angle < CRANE_MIN) { angle = CRANE_MIN; }
  if (angle > CRANE_MAX) { angle = CRANE_MAX; }
  int dir = (angle>cranePos)?1:-1;
  while (cranePos != angle) {
    cranePos += dir;
    craneServo.write(cranePos);
    delay(30);
  }
  cranePos = angle;
  craneServo.write(angle);
}

void craneUp() {
  setCrane(CRANE_UP);
}

void craneLoad() {
  setCrane(CRANE_LOAD);
}

void craneDown() {
  setCrane(CRANE_DOWN);
}

const int CLAW_MIN = 25;
const int CLAW_MAX = 85;
const int CLAW_OPEN = 43;
const int CLAW_CLOSED = 27;

int clawPos;

PWMServo clawServo;
void initClaw() {
  clawServo.attach(claw_servo_pin);
  clawPos = CLAW_OPEN;
  clawServo.write(clawPos);
}

void setClaw(int angle) {
  if (angle < CLAW_MIN) { angle = CLAW_MIN; }
  if (angle > CLAW_MAX) { angle = CLAW_MAX; }
  int dir = (angle>clawPos)?1:-1;
  while (clawPos != angle) {
    clawPos += dir;
    clawServo.write(clawPos);
    delay(20);
  }
}

void closeClaw() {
  setClaw(CLAW_CLOSED);
}

void openClaw() {
  setClaw(CLAW_OPEN);
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
  delay(500); // settle oscillations
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
  delay(500); // settle oscillations
}

void setup() {
 initGantry();
 initCrane();
 initClaw();
 Serial.begin(19200);
}

void run() {
  Serial.println("Running program.");
  openClaw();
  craneUp();
  Serial.println("1. Home");
  gantryHome();
  Serial.println("2. To loading station");
  gantryNextStation();
  Serial.println("3. Acquire egg");
  openClaw();
  craneLoad();
  closeClaw();
  craneUp();
  Serial.println("4. To cooking station");
  gantryNextStation();
  gantryNextStation();
  delay(500);
  Serial.println("5. Cook egg");
  craneDown();
  delay(2000);
  craneUp();
  Serial.println("6. To unloading station");
  gantryHome();
  gantryNextStation();
  Serial.println("7. Deposit egg");
  craneLoad();
  openClaw();
  craneUp();
  gantryHome();
  Serial.println("Egg cooking procedure complete.");
}

void loop() {
  if (Serial.available()) {
    int c = Serial.read();
    switch (c) {
      case '+':
      case '=':
        setCrane(cranePos + 1);
        Serial.print("CRANE: ");
        Serial.println(cranePos);
        break;
      case '-':
        setCrane(cranePos - 1);
        Serial.print("CRANE: ");
        Serial.println(cranePos);
        break;
      case '[':
        setClaw(clawPos - 1);
        Serial.print("CLAW: ");
        Serial.println(clawPos);
        break;
      case ']':
        setClaw(clawPos + 1);
        Serial.print("CLAW: ");
        Serial.println(clawPos);
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
      case 'c':
      case 'C':
        Serial.println("*Claw close.");
        closeClaw();
        break;
      case 'o':
      case 'O':
        Serial.println("*Claw open.");
        openClaw();
        break;
      case 'u':
      case 'U':
        Serial.println("+Crane up.");
        craneUp();
        break;
      case 'd':
      case 'D':
        Serial.println("+Crane down.");
        craneDown();
        break;
      case 'l':
      case 'L':
        Serial.println("+Crane load.");
        craneLoad();
        break;
      case 'r':
      case 'R':
        run();
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
  

