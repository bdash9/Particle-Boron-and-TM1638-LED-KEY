#include <Particle.h>
#include <TM1638.h>  // Ensure the TM1638 library is included

// Initialize the TM1638 with DATA, CLOCK, STROBE pins
TM1638 tm1638(D0, D1, D2);

void setup() {
    Serial.begin(9600);  // Start Serial communication for debugging
    tm1638.clearDisplay();
}

void loop() {
    // Retrieve the amount of free memory
    int freeMemory = System.freeMemory();

    // Create a display string for free memory
    char displayStr[9];
    snprintf(displayStr, sizeof(displayStr), "mEm %4d", freeMemory);

    // Output to Serial for verification
    Serial.print("Free Memory: ");
    Serial.println(freeMemory);

    // Display free memory on the TM1638
    tm1638.setDisplayToString(displayStr);

    delay(5000);  // Update every 5 seconds
}