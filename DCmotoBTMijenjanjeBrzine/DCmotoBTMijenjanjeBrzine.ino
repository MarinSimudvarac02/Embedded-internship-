#include "BluetoothSerial.h"

#define enA 19

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


BluetoothSerial SerialBT;

int n = 1;
int previousValue = 20;
int delayTime = 20;

int speedA = 20;
int speedB = 120;
int speedC = 250;


void setup() {
  pinMode(enA, OUTPUT);
  Serial.begin(115200);
  SerialBT.begin("ESP32test");
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {

  if (SerialBT.available()) {
    n = getBTcommand();
    Serial.println(n);
  }
  MotorSpeed(n);

  /*
    if (n >= 0 && n <= 255) {
    analogWrite(enA, n);
    Serial.println("u ifu");
    Serial.println(n);
    }
    else if (n > 255) {

    n = 255;
    analogWrite(enA, n);
    Serial.println("u elifu");
    Serial.println(n);
    }
    else {
    n = 0;
    analogWrite(enA, n);
    Serial.println("u elu");
    Serial.println(n);
    }
  */
  delay(1000);
}

int getBTcommand() {
  int commandBT = SerialBT.parseInt();
  Serial.print("Bluetooth command: ");
  Serial.println(commandBT);
  return commandBT;
}


void MotorSpeed (int x) {
  Serial.println(x);
  if (x == 0) {
    Deceleration(previousValue);
    /*
      do{
      previousValue = previousValue - 5;
        analogWrite(enA, previousValue);
        delay(delayTime);
      } while (previousValue >= 0);
    */
    previousValue = 0;
  }
  if (x == 1) {
    if (previousValue > speedA) {
      Deceleration(previousValue);      /*
      do {
        previousValue = previousValue - 5;
        analogWrite(enA, previousValue);
        delay(delayTime);
      } while (previousValue >= speedA);
*/
    }
    else if (previousValue < speedA) {
      Acceleration(previousValue);
      /*
      do {
        previousValue = previousValue + 5;
        analogWrite(enA, previousValue);
        delay(delayTime);
      } while (previousValue <= speedA);
      */
    }
    else {
      analogWrite(enA, speedA);
    }
    previousValue = speedA;
  }

  else if (x == 2) {
    
    if (previousValue > speedB) {
      Deceleration(previousValue);
     /* do {
        previousValue = previousValue - 5;
        analogWrite(enA, previousValue);
        delay(delayTime);
      } while (previousValue >= speedB);
    */
  }
  else if (previousValue < speedB) {
    Acceleration(previousValue);
    /*
    do {
      previousValue = previousValue + 5;
      analogWrite(enA, previousValue);
      delay(delayTime);
    } while (previousValue <= speedB);*/
  }
  else {
    analogWrite(enA, speedB);
  }
  previousValue = speedB;
}

else if (x == 3) {
  Deceleration(previousValue);
  /*
    if (previousValue < speedC) {
    do {
      previousValue = previousValue + 5;
      analogWrite(enA, previousValue);
      delay(delayTime);
    } while (previousValue <= speedC);

    
  */}
  else {
    analogWrite(enA, speedC);
  }
  previousValue = speedC;
}
}


void Acceleration(int currentSpeedDeceleration) {
  do {
    currentSpeedDeceleration = currentSpeedDeceleration a - 5;
    analogWrite(enA, currentSpeedDeceleration);
    delay(delayTime);
  } while (currentSpeedDeceleration >= speedA);
}

void Deceleration (int currentSpeedAcceleration) {
  do {
    currentSpeedAcceleration = currentSpeedAcceleration + 5;
    analogWrite(enA, currentSpeedAcceleration);
    delay(delayTime);
  } while (currentSpeedAcceleration <= speedA);
}
