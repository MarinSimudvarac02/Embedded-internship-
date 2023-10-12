#include "arduino.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"

#define DHTTYPE DHT11
#define DHTPIN 25

unsigned long lastTime = 0;
unsigned long timerDelay = 2000;

const char* ssid = "SAMSUNGA70";
const char* password = "marin123";
float temperature;
float humidity;
int numOfLines=0;

AsyncWebServer server(80);

AsyncEventSource events("/events");

DHT dht(DHTPIN, DHTTYPE);
JSONVar readings;

void deleteLineFromFile(int lineToDelete) {
  
  File file = SPIFFS.open("/temp.txt", "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  File tempFile = SPIFFS.open("/temp2.txt", "w+");
  if (!tempFile) {
    Serial.println("Failed to create temp file");
    file.close();
    return;
  }

  int currentLine = 0;
  char buffer[20];
  int max_len=file.available();

  while (file.available()) {
    file.readBytesUntil('\n', buffer, max_len);
    if (currentLine != lineToDelete) {
      tempFile.println(buffer);
    }

    currentLine++;
  }

  file.close();
  tempFile.close();
  SPIFFS.remove("/temp.txt" );
  SPIFFS.rename("/temp2.txt", "/temp.txt" );
  Serial.println("Line deleted successfully.");
}

String getSensorReadings() {
  readings["temperature"] = String(dht.readTemperature());
  readings["humidity"] =  String(dht.readHumidity());
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void lsSPIFFS(){
  Serial.println("###List Spiffs###");
  File root=SPIFFS.open("/");
  File file2=root.openNextFile();
  while(file2){
    Serial.print("File: ");
    Serial.println(file2.name());
    file2=root.openNextFile();
  }
}

float printDataSpiffsHumidity(){
     numOfLines=numOfLines+1;
  float h=dht.readHumidity();
  if(isnan(h)){
    Serial.println("Neuspjelo ucitavanje podataka");
    return 0;
  }
  Serial.print("Vlaga: ");
  Serial.print(h);
  Serial.println("%");
  return h;
}

float printDataSpiffsTemperature(){
  float t=dht.readTemperature();
  if(isnan(t)){
    Serial.println("Neuspjelo ucitavanje podataka");
    return 0;
  }
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println("°C");
  return t;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();
  initWiFi();
  initSPIFFS();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");

  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest * request) {
    String json = getSensorReadings();
    request->send(2000, "application/json", json);
    json = String();
  });

  events.onConnect([](AsyncEventSourceClient * client) {
    if (client->lastId()) {
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);


  server.begin();

  
  File file = SPIFFS.open("/temp.txt", "a+");
  if (!file) {
    Serial.println("Failed to create file");
    return;
  }
  file.close();
  SPIFFS.remove("/temp.txt");
  SPIFFS.remove("/temp2.txt");
   lsSPIFFS();
  
}

void loop() {
  // put your main code here, to run repeatedly:
    events.send("ping",NULL,millis());
    events.send(getSensorReadings().c_str(),"new_readings" ,millis());

   float temperatura=printDataSpiffsTemperature();
  float vlaga=printDataSpiffsHumidity();
  
  File fileToAppend = SPIFFS.open("/temp.txt",FILE_APPEND);

  if (!fileToAppend) {
    Serial.println("There was an error opening the file for appending");
    return;
  }
  
  if (fileToAppend.print(temperatura)) {
    Serial.print("File content was appended");
  } else {
    Serial.println("File append failed");
  }
   if (fileToAppend.print(" ")) {
    Serial.println("File content was appended");
  } else {
    Serial.println("File append failed");
  }

  fileToAppend.close();

  File fileToAppend2 = SPIFFS.open("/temp.txt",FILE_APPEND);

  if (!fileToAppend2) {
    Serial.println("There was an error opening the file for appending");
    return;
  }
  
  if (fileToAppend2.println(vlaga)) {
    Serial.println("File content was appended");
  } else {
    Serial.println("File append failed");
  }

  fileToAppend2.close();
   File file3 = SPIFFS.open("/temp.txt");
 
    if(!file3){
        Serial.println("Failed to open file for reading");
        return;
    }
 
    Serial.println("File Content:");
 
    while(file3.available()){
 
        Serial.write(file3.read());
    }
 
    file3.close();
    if(numOfLines==10){
       Serial.println("Deleting first line from file...");
      deleteLineFromFile(0);
      numOfLines=9;
    }
    delay(5000);
}
