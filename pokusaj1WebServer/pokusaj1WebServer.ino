#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"

#define DHTTYPE DHT11
#define DHTPIN 25

const char* ssid="SAMSUNGA70";
const char* password="marin123";
float temperature;
float humidity;

WebServer server(80);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
 WiFi.mode(WIFI_STA);
 dht.begin();

 Serial.println("Connecting to ");
 Serial.println(ssid);
 WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  
  server.handleClient();
  delay(2000);

}

void handle_OnConnect() {

 temperature = dht.readTemperature(); // Gets the values of the temperature
  humidity = dht.readHumidity(); // Gets the values of the humidity 
  server.send(200, "text/html", SendHTML(temperature,humidity)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float Temperaturestat,float Humiditystat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css'>\n";
  ptr +="<title>ESP32 Webpage</title>\n";
  ptr +="<style>html { font-family: Garamond; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;}\n";
  ptr +="h1 {font-size: 50px;color: #990505;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 30px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>ESP32 TEST DHT podataka na Web stranici</h1>\n";
  
  ptr +="<p><i class='fa fa-thermometer' style='font-size:48px;color:#990505'></i> Temperature: ";
  ptr +=(float)Temperaturestat;
  ptr +="&deg;C</p>";
  ptr +="<p><i class='fas fa-humidity'></i> Humidity: ";
  ptr +=(float)Humiditystat;
  ptr +="%</p>";
  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
