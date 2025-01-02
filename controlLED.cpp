#include <Arduino.h>

const int potentiometerPin = 34; // Analog pin connected to potentiometer
const int ledPin = 26;           // PWM pin connected to LED

// PWM channel and frequency settings
const int pwmChannel = 0;
const int pwmFrequency = 5000; // 5 kHz
const int pwmResolution = 8;  // 8-bit resolution (0-255)

void setup() {
    Serial.begin(115200);
    pinMode(potentiometerPin, INPUT);

    // Configure LED pin for PWM
    ledcSetup(pwmChannel, pwmFrequency, pwmResolution);
    ledcAttachPin(ledPin, pwmChannel);

    Serial.println("Setup Complete");
}

void loop() {
    // Read the analog value from the potentiometer (0-4095 for ESP32)
    int potValue = analogRead(potentiometerPin);

    // Map the potentiometer value to PWM duty cycle (0-255)
    int ledBrightness = map(potValue, 0, 4095, 0, 255);

    // Write the PWM signal to control LED brightness
    ledcWrite(pwmChannel, ledBrightness);

    // Debugging output
    Serial.print("Potentiometer Value: ");
    Serial.print(potValue);
    Serial.print(" | LED Brightness: ");
    Serial.println(ledBrightness);

    delay(10); // Small delay for stability
}
