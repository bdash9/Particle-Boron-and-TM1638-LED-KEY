// This #include statement was automatically added by the Particle IDE.
#include <TM1638.h>

#include <Particle.h>
// Initialize the TM1638 with configured pins for DATA, CLOCK, STROBE
TM1638 tm1638(D0, D1, D2);

void setup() {
    Serial.begin(9600);  // Start Serial communication for debugging
    tm1638.clearDisplay();

    // Retrieve system version
    const char* systemVersion = System.version();
    
    // Print system version to serial
    Serial.print("System Version: ");
    Serial.println(systemVersion);

    // Display system version on TM1638
    displaySystemVersion(systemVersion);
}

void loop() {
    // No update needed in the loop for constant version display
    delay(10000);  // Delay for passive looping or further activities
}

void displaySystemVersion(const char* version) {
    // Ensure the version is formatted and displayed correctly
    size_t len = strlen(version);

    // For longer strings, handle scrolling or truncation if necessary
    if (len <= 8) {
        tm1638.setDisplayToString(version);  // Direct display if it fits
    } else {
        // If longer than 8, implement a simple scroll
        for (int offset = 0; offset < len - 7; offset++) {
            char displayBuffer[9] = {0};

            for (int i = 0; i < 8; i++) {
                displayBuffer[i] = version[offset + i];
            }

            tm1638.setDisplayToString(displayBuffer);
            delay(500);  // Set scrolling speed
        }
    }
}