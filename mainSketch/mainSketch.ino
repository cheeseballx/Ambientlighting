#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include "secrets.h"            // Secret Passwords

//Connection variables
const char* ssid     = SECRET_WIFI_SSID;   
const char* password = SECRET_WIFI_PASSWORD;

WiFiServer server(80);
String header;

void setup() {
  
  //Start the program
  Serial.begin(9600); 
  delay(100);
  Serial.println("Starting Program");

  //Connect to the wifi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid); 
  Serial.println(" ...");

  //check connection
  int seconds = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);                  //wait a second
    Serial.print("Status: ");
    Serial.print(WiFi.status()); //status
    Serial.print(" ");           //empty space
    Serial.println(++seconds);   //seconds that are run
  }

  //reaching this code means were connected
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  //now Start WebServer
  server.begin();
  delay(100);
}

void loop() {

  //check server
  WiFiClient client = server.available();
  if (client) { 
    Serial.println("New Client."); // print a message out in the serial port
    String currentLine = ""; // make a String to hold incoming data from the client
    while (client.connected()) { // loop while the client's connected
      if (client.available()) { // if there's bytes to read from the client,
      char c = client.read(); // read a byte, then
      Serial.write(c); // print it out the serial monitor
      header += c;
      if (c == '\n') { 
        if (currentLine.length() == 0) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();
        }
      }
     }
    }
  }
  
}
