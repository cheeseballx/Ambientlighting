#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266WebServer.h>   // And the webserver
#include <uri/UriRegex.h>       // take URIRegex Library for simple path parameter checking

#include "secrets.h"            // Secret Passwords
#include "strip.h"              // My Lighning Library

//Connection variables
const char* ssid     = SECRET_WIFI_SSID;   
const char* password = SECRET_WIFI_PASSWORD;

ESP8266WebServer server(80);

//Our Rest Server
void restServerRouting(){
  server.on("/", HTTP_GET, []() {
        Serial.println("called / "); 
        server.send(200, F("text/html"),F("<h2>Welcome to BBQ Ambient Lighting API </h2><h1>BALA</h1>"));
  });

  server.on(F("/helloWorld"), HTTP_GET, []() {
        Serial.println("world"); 
        server.send(200, F("text/json"), F("{\"name\": \"Hello world\"}"));
  });

  server.on(UriRegex("^\\/([a-z]{3})\\/([0-9]+)\\/([0-9]+)\\/([0-9]+)$"), HTTP_GET, []() {
    String prog = server.pathArg(0);
    
    String r = server.pathArg(1);
    String g = server.pathArg(2);
    String b = server.pathArg(3);

    if (prog == "sta") {
      prog_static(r.toInt(),g.toInt(),b.toInt());
      server.send(200, F("text/html"), "prog static colorize");
    }

    else if ( prog == "sin") {
      prog_sin(r.toInt(),g.toInt(),b.toInt());
      server.send(200, F("text/html"), "prog sinusal wave colorize");
    }

    else{
      prog_off();
      server.send(200, F("text/html"), "fallback is off, because we cannont recognize what programm should be run");
    }
  });

  
  // Set not found response
  server.onNotFound([]() { 
    String message = "404: " + server.uri();
    Serial.println(message); 
    server.send(404, "text/plain", message);
  });
  
}


void setup() {
  
  //Start the program and the Seriel Monitor
  Serial.begin(115200); 
  delay(100);
  Serial.println("Starting Program");

  //Connect to the wifi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid); 
  Serial.println(" ...");

  //check connection until connected
  int seconds = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);                  //wait a second
    Serial.print("Status: ");
    Serial.println(WiFi.status()); //status
  }

  //reaching this code means were connected, give out ip then
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  //Start rest server
  restServerRouting();
  server.begin();
  Serial.println("HTTP server started");

  //Start LED SEtup
  led_setup();
}

void loop() {
  //here we just need to keep the Client alive
  server.handleClient();

  led_update();
}
