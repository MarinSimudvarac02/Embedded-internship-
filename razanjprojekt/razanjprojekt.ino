#include <Stepper.h>
const int stepsPerRevolution = 2038;

#define IN1 19
#define IN2 18
#define IN3 5
#define IN4 21

Stepper myStepper(stepsPerRevolution,IN1,IN3,IN2,IN4);

void setup() {
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:


  for(int i = 1; i<16; i++){
  myStepper.setSpeed(i);
  myStepper.step(stepsPerRevolution/25);
  
  }
  
  for(int i = 16; i>=0; i--){
  myStepper.setSpeed(i);
  myStepper.step(stepsPerRevolution/25);
  
  }
}
