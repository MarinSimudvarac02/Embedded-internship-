#include "BluetoothSerial.h"

#define enA 19

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


BluetoothSerial SerialBT;

int speedOfMotor = 0;
int n = 0;
int previousValue = 0;
int delayTime = 100;
int speedA = 20;
int speedB = 120;
int speedC = 250;
int speedD = 0;

void setup() {
  pinMode(enA, OUTPUT);
  Serial.begin(115200);
  SerialBT.begin("ESP32test");
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {

  if (SerialBT.available()) {
    n = getBTcommand();
  }
  speedOfMotor = MotorSpeed(n);
  Serial.println(speedOfMotor);
  if (speedOfMotor > previousValue) {
   previousValue=Acceleration(speedOfMotor,previousValue);
  }
  else if (speedOfMotor < previousValue) {
    previousValue=Deceleration(speedOfMotor,previousValue);

  }
  else {
    analogWrite(enA, speedOfMotor);
    previousValue = speedOfMotor;
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


int Acceleration(int currentSpeedAceleration,int previousValueAcceleration) {
  Serial.println(currentSpeedAceleration);
  do {
    previousValueAcceleration = previousValueAcceleration + 5;
    analogWrite(enA, previousValueAcceleration);
    Serial.println(previousValueAcceleration);
    delay(delayTime);
  } while (previousValueAcceleration <= currentSpeedAceleration);
  previousValueAcceleration = currentSpeedAceleration;
  return previousValueAcceleration;
}

int Deceleration (int currentSpeedDeceleration, int PreviousValueDeceleration) {
  Serial.println(currentSpeedDeceleration);
  do {
    PreviousValueDeceleration = PreviousValueDeceleration - 5;
    analogWrite(enA, PreviousValueDeceleration);
    Serial.println(previousValueDeceleration);
    delay(delayTime);
  } while (PreviousValueDeceleration >= currentSpeedDeceleration);
  PreviousValueDeceleration = currentSpeedDeceleration;
  return PreviousValueDeceleration;
}
