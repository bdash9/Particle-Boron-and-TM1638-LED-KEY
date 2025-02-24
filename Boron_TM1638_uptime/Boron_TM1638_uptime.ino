// This #include statement was automatically added by the Particle IDE.
#include <TM1638.h>

// Initialize TM1638 with DATA, CLOCK, STROBE pins
TM1638 tm1638(D0, D1, D2);

void setup() {
    Serial.begin(9600);  // Begin serial communication for debugging
    tm1638.clearDisplay();
}

void loop() {
    // Calculate uptime using millis
    unsigned long uptimeMillis = millis();

    // Convert uptime to hours, minutes, and seconds
    unsigned long totalSeconds = uptimeMillis / 1000;
    unsigned long hours = totalSeconds / 3600;
    unsigned long minutes = (totalSeconds % 3600) / 60;
    unsigned long seconds = totalSeconds % 60;

    // Prepare the string to display
    char displayStr[9];
    snprintf(displayStr, sizeof(displayStr), "%02lu:%02lu:%02lu", hours, minutes, seconds);

    // Output uptime to serial monitor for verification
    Serial.print("Uptime: ");
    Serial.println(displayStr);

    // Display uptime on TM1638
    tm1638.setDisplayToString(displayStr);

    delay(1000);  // Update every second
}