#define enA 19

int intensity=1;

void setup() {
  pinMode(enA,OUTPUT);
  Serial.begin(115200);

}

void loop() {
  if(Serial.available()){
    intensity=Serial.parseInt();
  }
  if(intensity>=0 && intensity<=255){
    analogWrite(enA,intensity);
    Serial.println(intensity);
  }
  else{
    intensity=127;
    Serial.println(intensity);
  }
  
}
