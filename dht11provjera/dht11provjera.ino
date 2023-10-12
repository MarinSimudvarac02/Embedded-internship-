#include "DHT.h"
#include "SPIFFS.h"
#include <FS.h>

#define DHTPIN 25
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  
 Serial.begin(115200);
 dht.begin();
 if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
   }
   File file = SPIFFS.open("/temp.txt", "a+");
  if (!file) {
    Serial.println("Failed to create file");
    return;
  }
  file.close();
}

void loop() {
  
  float h=dht.readHumidity();
  float t=dht.readTemperature();
  if(isnan(h)|| isnan(t)){
    Serial.println("Neuspjelo ucitavanje podataka");
    return;
  }
  Serial.print("Vlaga: ");
  Serial.print(h);
  Serial.println("%");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println("°C");
  File fileToAppend = SPIFFS.open("/test.txt",FILE_APPEND);

  if (!fileToAppend) {
    Serial.println("There was an error opening the file for appending");
    return;
  }

  if (fileToAppend.println(t)) {
    Serial.println("File content was appended");
  } else {
    Serial.println("File append failed");
  }

  fileToAppend.close();

  
  delay(2000);
 
}
