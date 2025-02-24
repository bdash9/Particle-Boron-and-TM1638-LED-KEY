#include <TM1638.h>  // Make sure the TM1638 library is included

// Initialize TM1638 with DATA, CLOCK, STROBE pins
TM1638 tm1638(D0, D1, D2);

void setup() {
    Serial.begin(9600);  // Begin Serial communication for debugging (optional)

    // Clear display and ensure LEDs are initially off
    tm1638.clearDisplay();
    tm1638.setLEDs(0x00);
}

void loop() {
    // Light up LEDs from right (D8) to left (D1)
    for (int i = 0; i < 8; i++) {
        tm1638.setLEDs(1 << i);  // Turn on LED at position i
        delay(200);              // Wait to allow visibility
    }

    // Light up LEDs from left (D1) to right (D8)
    for (int i = 7; i >= 0; i--) {
        tm1638.setLEDs(1 << i);  // Turn on LED at position i
        delay(200);              // Wait to allow visibility
    }
}