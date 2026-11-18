
const int IN1 = 7;
const int IN2 = 8;
const int PWM = 9;
const int STBY = 6;

const int TRIG_PIN = 10;
const int ECHO_PIN = 11;

// Settings
const int DISTANCE_THRESHOLD = 30; // cm
const int MOTOR_SPEED = 180;
const unsigned long ROTATION_TIME = 5000;

// Track motor position
bool motorExtended = false;

void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(STBY, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(STBY, HIGH);
  stopMotor();

  Serial.println("System ready!");
}

void loop() {
  float distance = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Ignore invalid distance readings
  if (distance < 0) {
    delay(100);
    return;
  }

  // Object is within 30 cm
  if (distance <= DISTANCE_THRESHOLD && !motorExtended) {

    Serial.println("Object detected! Rotating clockwise.");

    rotateClockwise();
    delay(ROTATION_TIME);
    stopMotor();

    motorExtended = true;

    Serial.println("Clockwise rotation complete.");
  }

  // Object moves beyond 30 cm
  else if (distance > DISTANCE_THRESHOLD && motorExtended) {

    Serial.println("Rotating counterclockwise.");

    rotateCounterClockwise();
    delay(ROTATION_TIME);
    stopMotor();

    motorExtended = false;

    Serial.println("Counterclockwise rotation complete.");
  }

  delay(100);
}

// Measure distance in centimeters
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 25000);

  if (duration == 0) {
    return -1;
  }

  return duration * 0.0343 / 2.0;
}

// Rotate clockwise
void rotateClockwise() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(PWM, MOTOR_SPEED);
}

// Rotate counterclockwise
void rotateCounterClockwise() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(PWM, MOTOR_SPEED);
}

// Stop motor
void stopMotor() {
  analogWrite(PWM, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}
