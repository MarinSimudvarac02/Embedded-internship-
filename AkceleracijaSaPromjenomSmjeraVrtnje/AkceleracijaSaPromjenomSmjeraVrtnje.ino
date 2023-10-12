#include "BluetoothSerial.h"
#define in3 18
#define in4 21
#define enA 19

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


BluetoothSerial SerialBT;

int speedOfMotor = 0;
int n = 0;

int previousValue = 0;
int delayTime = 10;

int speedA = 192;
int speedB = 224;
int speedC = 255;
int speedD = 0;

int callChangeOfDirectionClockWise = 0;
int callChangeOfDirectionAntiClockWise = 0;

int ClockWiseDecelerationCall = 0;
int ClockWiseDecelerationSpeed = 0;
int ClockWiseAccelerationCall = 0;

int AntiClockWiseDecelerationCall = 0;
int AntiClockWiseDecelerationSpeed = 0;
int AntiClockWiseAccelerationCall = 0;

int previousDirection = 1;
int wasMotorTurnedOff = 0;
int accSpeed=1;

void setup() {
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  Serial.begin(115200);
  SerialBT.begin("ESP32");
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {

  if (SerialBT.available()) {
    n = getBTcommand();
  }
  if (n >= 1 && n <= 3) {
    speedOfMotor = MotorSpeed(n);
    if (wasMotorTurnedOff == 0) {
      Serial.println("Motor je bio iskljucen");
      delay(500);
      if (previousDirection == 1) {
        Serial.println("Motor se kreće vrtjeti u smjeru kazaljke na satu");
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
        previousValue = Acceleration(speedOfMotor, previousValue);
      }
      else if (previousDirection == 2) {
        Serial.println("Motor se kreće vrtjeti u smjeru suprotnom od kazaljke na satu");
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
        previousValue = Acceleration(speedOfMotor, previousValue);
      }
    }
    else {
      if (speedOfMotor > previousValue) {
        previousValue = Acceleration(speedOfMotor, previousValue);
 
      }
      else if (speedOfMotor < previousValue) {
        previousValue = Deceleration(speedOfMotor, previousValue);

      }
      else {
        analogWrite(enA, speedOfMotor);
        previousValue = speedOfMotor;
      }
    }
    wasMotorTurnedOff = 1;
  }
  else if (n == 0) {
    speedOfMotor = MotorSpeed(n);
    wasMotorTurnedOff = 0;
    previousValue = Deceleration(speedOfMotor, previousValue);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    Serial.println("Hardware-ski isključeno");
    delay(1000);
  }
  else if (n == 4) {
    wasMotorTurnedOff = 1;
    previousDirection = 1;
    callChangeOfDirectionClockWise = ChangeOfDirectionClockwise(speedOfMotor);
    analogWrite(enA, speedOfMotor);
    previousValue = speedOfMotor;
    if (speedOfMotor == speedA) {
      n = 1;
    }
    else if (speedOfMotor == speedB) {
      n = 2;
    }
    else if (speedOfMotor == speedC) {
      n = 3;
    }
    else if (speedOfMotor == speedD) {
      n = 0;
    }
  }
  else if (n == 5) {
    wasMotorTurnedOff = 1;
    previousDirection = 2;
    callChangeOfDirectionAntiClockWise = ChangeOfDirectionAntiClockwise(speedOfMotor);
    analogWrite(enA, speedOfMotor);
    previousValue = speedOfMotor;
    if (speedOfMotor == speedA) {
      n = 1;
    }
    else if (speedOfMotor == speedB) {
      n = 2;
    }
    else if (speedOfMotor == speedC) {
      n = 3;
    }
    else if (speedOfMotor == speedD) {
      n = 0;
    }
  }
  delay(1000);
}

int getBTcommand() {
  int commandBT = SerialBT.parseInt();
  Serial.print("Bluetooth command: ");
  Serial.println(commandBT);
  return commandBT;
}


int MotorSpeed (int x) {
  Serial.println(x);
  if (x == 0) {
    return speedD;
  }
  else if (x == 1) {
    return speedA;
  }
  else if (x == 2) {
    return speedB;
  }
  else if (x == 3) {
    return speedC;
  }
}


int Acceleration(int currentSpeedAceleration, int previousValueAcceleration) {
  do {
    previousValueAcceleration = previousValueAcceleration + accSpeed;
    analogWrite(enA, previousValueAcceleration);
    Serial.println(previousValueAcceleration);
    delay(delayTime);
  } while (previousValueAcceleration <= currentSpeedAceleration);
  previousValueAcceleration = currentSpeedAceleration;
  return previousValueAcceleration;
}

int Deceleration (int currentSpeedDeceleration, int PreviousValueDeceleration) {
  do {
    PreviousValueDeceleration = PreviousValueDeceleration - accSpeed;
    analogWrite(enA, PreviousValueDeceleration);
    Serial.println(PreviousValueDeceleration);
    delay(delayTime);
  } while (PreviousValueDeceleration >= currentSpeedDeceleration);
  PreviousValueDeceleration = currentSpeedDeceleration;
  return PreviousValueDeceleration;
}

int ChangeOfDirectionClockwise(int speedBeforeChangeOfDirection) {
  delay(1000);
  ClockWiseDecelerationCall = Deceleration(ClockWiseDecelerationSpeed, speedBeforeChangeOfDirection);
  if (ClockWiseDecelerationCall == 0) {
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    Serial.println("Hardware-ski isključeno");
    delay(1000);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    Serial.println("Hardware-ski uključeno");
  }
  ClockWiseAccelerationCall = Acceleration(speedBeforeChangeOfDirection, ClockWiseDecelerationCall);
  return speedBeforeChangeOfDirection;
}

int ChangeOfDirectionAntiClockwise(int speedBeforeChangeOfDirectionAntiClockwise) {
  delay(1000);
  AntiClockWiseDecelerationCall = Deceleration(AntiClockWiseDecelerationSpeed, speedBeforeChangeOfDirectionAntiClockwise);
  if (AntiClockWiseDecelerationCall == 0) {
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    Serial.println("Hardware-ski isključeno");
    delay(1000);
    Serial.println("Hardware-ski uključeno");
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }
  AntiClockWiseAccelerationCall = Acceleration(speedBeforeChangeOfDirectionAntiClockwise, AntiClockWiseDecelerationCall);
  return speedBeforeChangeOfDirectionAntiClockwise;
}
