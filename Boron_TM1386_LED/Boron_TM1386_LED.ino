// This works - Blips the LED on every 5 seconds

#include <TM1638.h>  // Ensure the TM1638 library is included

// Initialize the TM1638 with DATA, CLOCK, STROBE pins
TM1638 tm1638(D0, D1, D2);

void setup() {
    Serial.begin(9600);  // Initialize Serial for debugging

    // Clear display and turn all LEDs off initially
    tm1638.clearDisplay();
    tm1638.setLEDs(0x00);
}

void loop() {
    // Simulate less bright blip by briefly turning the LED on with a short period
    for (int i = 0; i < 5; i++) {   // Repeat to simulate one dim "blip"
        tm1638.setLED(1, 0);
        delay(20);  // LED on for a brief instant to appear dim
        tm1638.setLED(0, 0);
        delay(30);  // LED off for a slightly longer time
    }

    // Wait for the rest of the 5-second cycle
    delay(4700);  // Adjusted delay to complete the 5-second cycle
}