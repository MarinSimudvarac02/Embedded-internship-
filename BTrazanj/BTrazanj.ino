#include "BluetoothSerial.h"
#include <Stepper.h>
const int stepsPerRevolution = 2038;

#define IN1 19
#define IN2 18
#define IN3 5
#define IN4 21

Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {

  /*
    ConnectionToBluetooth();
    long terminalRead;
    terminalRead = FromCharToInt();
  */
  int n;
  if (SerialBT.available()) {
    n = getBTcommand() ;
  }
  else{
    n=1;
  }
  if (n >= 1 && n <= 16) {
    myStepper.setSpeed(n);
    myStepper.step(stepsPerRevolution / 100);
    Serial.println(n);
  }
  else {
    n = 1;
    myStepper.setSpeed(n);
    myStepper.step(stepsPerRevolution / 100);
    Serial.println(n);
  }
}


int getBTcommand() {
  int commandBT = SerialBT.parseInt();
  Serial.print("Bluetooth command: ");
  Serial.println(commandBT);
  return commandBT;
}






/*
  void ConnectionToBluetooth(){
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  }

  long FromCharToInt(){
  char buffer[50];
  long dataIn;
    while(Serial.available()>0){
      dataIn=Serial.parseInt();
      sprintf(buffer,"Uneseni broj je: %l ",dataIn);
      Serial.println("\n %c ",buffer);
    }
    return dataIn ;
  }
*/
