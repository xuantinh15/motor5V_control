#define IN1 25
#define IN2 26

int speedMotor = 0;
bool forward = true;
bool motorRun = false;

void setup() {
  Serial.begin(115200);

  pinMode(IN2, OUTPUT);
  digitalWrite(IN2, LOW);

  // API mới của Core 3.x
  ledcAttach(IN1, 1000, 8);

  ledcWrite(IN1, 0);

  Serial.println("READY");
}

void loop() {

  if (Serial.available()) {

    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "START") {

      motorRun = true;
      updateMotor();

    }

    else if (cmd == "STOP") {

      motorRun = false;
      updateMotor();

    }

    else if (cmd.startsWith("SPD:")) {

      speedMotor = cmd.substring(4).toInt();

      if (speedMotor < 0) speedMotor = 0;
      if (speedMotor > 100) speedMotor = 100;

      updateMotor();

    }

    else if (cmd == "DIR:F") {

      forward = true;
      updateMotor();

    }

    else if (cmd == "DIR:R") {

      forward = false;
      updateMotor();

    }

  }

}

void updateMotor() {

  int pwm = map(speedMotor, 0, 100, 0, 255);

  if (!motorRun) {

    ledcWrite(IN1, 0);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

    return;

  }

  if (forward) {

    digitalWrite(IN2, LOW);

    ledcWrite(IN1, pwm);

  }

  else {

    // Đảo chiều bằng cách đổi chân PWM
    ledcDetach(IN1);

    pinMode(IN1, OUTPUT);
    digitalWrite(IN1, LOW);

    ledcAttach(IN2, 1000, 8);

    ledcWrite(IN2, pwm);

  }

}