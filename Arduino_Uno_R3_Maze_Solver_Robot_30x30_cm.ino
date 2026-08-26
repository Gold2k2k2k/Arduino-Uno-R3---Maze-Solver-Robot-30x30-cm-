// ==========================================
// MOTOR A (Right Motor)
// ==========================================
#define en1 3
#define in1 2
#define in2 4

// ==========================================
// MOTOR B (Left Motor)
// ==========================================
#define en2 6
#define in3 5
#define in4 7

#define deltaERROR 3

// ==========================================
// ULTRASONIC SENSORS PINS
// ==========================================
int front_trig = 10;
int front_echo = 11;

int left_trig = 12;
int left_echo = 13;

int right_trig = 8;
int right_echo = 9;

long tg = 0, x, y, z;

// Variables for distance and time calculations
long front_time, front_dis, right_time, right_dis, left_time, left_dis;
long front_distmp;
int count = 0, cnt = 0;

void setup() {
  // Initialize ultrasonic sensor pins
  pinMode(front_trig, OUTPUT);
  pinMode(front_echo, INPUT);

  pinMode(left_trig, OUTPUT);
  pinMode(left_echo, INPUT);

  pinMode(right_trig, OUTPUT);
  pinMode(right_echo, INPUT);
  
  // Initialize motor driver pins
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);
  
  // Start serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  // ----------------------------------------
  // 1. READ FRONT SENSOR
  // ----------------------------------------
  digitalWrite(front_trig, LOW);
  delayMicroseconds(5);
  digitalWrite(front_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(front_trig, LOW);
  front_time = pulseIn(front_echo, HIGH);
  front_dis = front_time * 0.034 / 2; // Convert time to distance in cm

  // ----------------------------------------
  // 2. READ LEFT SENSOR
  // ----------------------------------------
  digitalWrite(left_trig, LOW);
  delayMicroseconds(5);
  digitalWrite(left_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(left_trig, LOW);
  left_time = pulseIn(left_echo, HIGH);
  left_dis = left_time * 0.034 / 2;

  // ----------------------------------------
  // 3. READ RIGHT SENSOR
  // ----------------------------------------
  digitalWrite(right_trig, LOW);
  delayMicroseconds(5);
  digitalWrite(right_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(right_trig, LOW);
  right_time = pulseIn(right_echo, HIGH);
  right_dis = right_time * 0.034 / 2;
  
  // Print sensor data for debugging
  Serial.print(left_dis);
  Serial.print("   :");
  Serial.print(front_dis);
  Serial.print("    :   ");
  Serial.print(right_dis);

  // ==========================================
  // BRAIN OF THE ROBOT (NAVIGATION LOGIC)
  // ==========================================
  
  if (front_dis >= 15) {
    // If there is enough space in front (> 15cm)
    
    if (left_dis > 2 && left_dis <= 8) {
      // Too close to the left wall -> Balance to the right
      balancedright();
      delay(50);
      Serial.print("   BALANCED TO RIGHT");
    } else if (left_dis > 8 && left_dis <= 15) {
      // Perfect distance from the left wall -> Move straight forward
      forward();
      Serial.print("   FORWARD");
    }
    
  } else if (front_dis >= 5 && left_dis > 4 && left_dis <= 30) {
    // Obstacle in front, but space on the right (deduced by moving right)
    right();
    delay(150);
    Serial.print("   MOVE RIGHT");
  }
  
  if (left_dis >= 35) {
    // Large gap on the left wall -> Take a left turn
    left();
    Serial.print("   MOVE LEFT");
  }

  // ==========================================
  // ANTI-STUCK MECHANISM (Fix front force error)
  // ==========================================
  
  // Check if the front distance hasn't changed significantly (stuck against wall)
  if (abs(front_dis - front_distmp) <= 2) {
    if (count >= 55) {
      // If stuck for 55 consecutive loops, move backward to escape
      back();
      delay(50);
      Serial.print("    Back force");
      Serial.print(count);
    } else {
      // Increment stuck counter
      count++;
      front_distmp = front_dis;
    }
  } else {
    // Not stuck, reset counter
    count = 0;
    front_distmp = front_dis;
  }
  
  // Emergency backup if hitting side walls
  if (left_dis <= 3 || right_dis <= 3) {
    back();
    delay(50);
  }

  Serial.println(); // Next line for serial monitor
}

// ==========================================
// MOTOR CONTROL FUNCTIONS
// ==========================================

// Adjust trajectory slightly to the right
void balancedright() {
  analogWrite(en1, 90);  // Right motor slower
  analogWrite(en2, 130); // Left motor faster
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

// Move straight forward
void forward() {
  analogWrite(en1, 130);
  analogWrite(en2, 115); // Adjust for motor speed mismatch if necessary
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

// Move forward at high speed
void maxspeed() {
  analogWrite(en1, 162);
  analogWrite(en2, 180);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

// Turn right in place (or sharp right)
void right() {
  analogWrite(en1, 120);
  analogWrite(en2, 130);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

// Stop all motors
void stop() {
  analogWrite(en1, 0);
  analogWrite(en2, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

// Turn left in place (or sharp left)
void left() {
  analogWrite(en1, 200);
  analogWrite(en2, 40);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

// Full reverse action
void reverse() {
  analogWrite(en1, 150);
  analogWrite(en2, 150);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

// Move backward slowly (escape mechanism)
void back() {
  analogWrite(en1, 110);
  analogWrite(en2, 110);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
