
#include <WiFi.h>

const char* ssid = "SSID";  // Entering WiFi SSID
const char* pass = "PASS";  // Entering WiFi Password

WiFiServer server(80);  // Creating a webserver on Port 80
const int ledPin = 14;  // LED-PIN
bool ledStatus = LOW;   // The initial state of the LED (OFF)

void setup() {
  pinMode(ledPin, OUTPUT);  // Setting the LED-PIN to output
  Serial.begin(115200);  // Starting the serial communication
  WiFi.begin(ssid, pass);  // Starting to establish a connection to the WiFi network
  while (WiFi.status() != WL_CONNECTED) delay(500);  // Waiting until the connection is established

  // Print the IP address in the requested format
  Serial.print("The ESP32's local IP-Address is: ");
  Serial.println(WiFi.localIP());

  server.begin();  // Starting up the webserver on Port 80
}

void loop() {
  WiFiClient client = server.available();  // Checking if a Client is connected

  if (client) {
    String req = client.readStringUntil('\r');  // Reading the client's HTTP request up to the line break ('\r')

    // Checking whether the request contains "/on" to turn on the LED or "/off" to turn it off
    ledStatus = req.indexOf("GET /on") >= 0 ? HIGH : req.indexOf("GET /off") >= 0 ? LOW : ledStatus;

    // Sending an HTTP response to the client (containing HTML)
    client.println("HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: close\n");
    // HTML contents
    client.print("<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><title>ESP32 LED CONTROL</title></head><br><br><br><br><br><br><br><br><br><br><br><hr><center><h1>ESP32 Web Server LED-Control</h1><p>LED <a href=\"on\"><button>ON</button></a> <a href=\"off\"><button>OFF</button></a></p><hr></center></html>");

    client.stop();  // Quitting the Client-connection
    digitalWrite(ledPin, ledStatus);  // Executing the command based on the client's request (controlling the LED / LED-PIN)

    // Reporting the LED status in the Serial Monitor
    Serial.println("LED is now " + String(ledStatus == HIGH ? "ON" : "OFF"));
  }
}
