

const int gantry_enable_pin = 21;
const int gantry_in3_pin = 20;
const int gantry_in4_pin = 19;
const int gantry_limit_switch = 18;
const int gantry_station_switch = 17;

const int MOTOR_OFF = 0;
const int MOTOR_FORWARD = 1;
const int MOTOR_BACKWARD = 2;
const int MOTOR_BRAKE = 3;

void initGantry() {
  pinMode(gantry_enable_pin, OUTPUT);
  pinMode(gantry_in3_pin, OUTPUT);
  pinMode(gantry_in4_pin, OUTPUT);
  gantryMotor(MOTOR_OFF);
  pinMode(gantry_limit_switch, INPUT_PULLUP);
  pinMode(gantry_station_switch, INPUT_PULLUP);
}

void gantryMotor(int direction) {
  switch(direction) {
  case MOTOR_OFF:
    digitalWrite(gantry_enable_pin, LOW);
    digitalWrite(gantry_in3_pin, LOW);
    digitalWrite(gantry_in4_pin, LOW);
    break;
  case MOTOR_FORWARD:
    digitalWrite(gantry_enable_pin, LOW);
    digitalWrite(gantry_in3_pin, HIGH);
    digitalWrite(gantry_in4_pin, LOW);
    digitalWrite(gantry_enable_pin, HIGH);
    break;
  case MOTOR_BACKWARD:
    digitalWrite(gantry_enable_pin, LOW);
    digitalWrite(gantry_in3_pin, LOW);
    digitalWrite(gantry_in4_pin, HIGH);
    digitalWrite(gantry_enable_pin, HIGH);
    break;
  case MOTOR_BRAKE:
    digitalWrite(gantry_enable_pin, LOW);
    digitalWrite(gantry_in3_pin, HIGH);
    digitalWrite(gantry_in4_pin, HIGH);
    digitalWrite(gantry_enable_pin, HIGH);
    break;
  }
}

void gantryHome() {
  // Clumsily PWM the motor to limit speed.
  gantryMotor(MOTOR_BACKWARD);
  while (digitalRead(gantry_limit_switch) == HIGH);
  gantryMotor(MOTOR_BRAKE);
}

void gantryNextStation() {
  gantryMotor(MOTOR_FORWARD);
  while (digitalRead(gantry_station_switch) == LOW);
  while (digitalRead(gantry_station_switch) == HIGH);
  gantryMotor(MOTOR_BRAKE);
}

void setup() {
 initGantry();
}

void loop() {
  digitalWrite(11,LOW);
  gantryMotor(MOTOR_FORWARD);
  delay(1000);
  gantryMotor(MOTOR_BRAKE);
  delay(500);
  gantryMotor(MOTOR_OFF);
  delay(500);
  digitalWrite(11,HIGH);
  gantryMotor(MOTOR_BACKWARD);
  delay(1000);
  gantryMotor(MOTOR_BRAKE);
  delay(500);
  gantryMotor(MOTOR_OFF);
  delay(500);
}
  

