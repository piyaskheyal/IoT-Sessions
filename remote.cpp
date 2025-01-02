#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "Router 1";
const char *password = "kheyal2g";

WiFiServer server(80);  // HTTP server on port 80

int ledPin = LED_BUILTIN;  // LED connected to pin 5

void handlePostRequest(String postData);

void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);  // set the LED pin mode

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("#");
    }
    Serial.println("\nWiFi connected.");
    Serial.println(WiFi.localIP());

    server.begin();  // Start the server
}

void loop() {
  WiFiClient client = server.available();  // Check for incoming clients

  if (client) {  // If a client is connected
    Serial.println("New Client.");
    String currentLine = "";
    String postData = "";
    bool headersEnded = false;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        // Process incoming HTTP request
        if (headersEnded) {
          postData += c;  // Capture POST data
        } else {
          // Parse headers
          if (c == '\n' && currentLine.length() == 0) {
            headersEnded = true;  // End of headers, now we can capture the body
            Serial.println("Headers ended, capturing POST data...");
          } else if (c == '\n') {
            currentLine = "";  // Clear currentLine for the next header
          } else if (c != '\r') {
            currentLine += c;  // Append character to the current header line
          }
        }

        // Send HTTP response after receiving POST data
        if (headersEnded && !client.available()) {
          if (postData.length() > 0) {
            handlePostRequest(postData);  // Handle the POST data
          }

          // Send response to the client
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          client.println("<h1>LED Control</h1>");
          client.println("<form action=\"/\" method=\"POST\">");
          client.println("<button name=\"led\" value=\"on\">Turn LED On</button>");
          client.println("<button name=\"led\" value=\"off\">Turn LED Off</button>");
          client.println("</form>");
          break;  // Break the loop after sending the response
        }
      }
    }

    client.stop();  // Close the client connection
    Serial.println("Client Disconnected.");
  }
}

void handlePostRequest(String postData) {
  Serial.println("Received POST Data:");
  Serial.println(postData);

  // Check if the POST data contains "led=on" or "led=off"
  if (postData.indexOf("led=on") != -1) {
    digitalWrite(ledPin, HIGH);  // Turn the LED on
    Serial.println("LED Turned On");
  } else if (postData.indexOf("led=off") != -1) {
    digitalWrite(ledPin, LOW);  // Turn the LED off
    Serial.println("LED Turned Off");
  }
}
