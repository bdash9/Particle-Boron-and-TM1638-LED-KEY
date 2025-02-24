// Works - timer

#include <TM1638.h>  // Make sure the TM1638 library is correctly included

// Initialize the TM1638 with specific pins for DATA, CLOCK, and STROBE
TM1638 tm1638(D0, D1, D2);

// Initialize timer variables
unsigned long previousMillis = 0;
const long interval = 1000;  // 1000 milliseconds == 1 second

// Timer counts for days, hours, minutes, and seconds
int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;

void setup() {
    Serial.begin(9600);  // Start Serial communication for debugging
}

void loop() {
    unsigned long currentMillis = millis();

    // Check if one second has passed
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        // Increment the seconds and manage overflow for minutes, hours, days
        seconds++;

        if (seconds >= 60) {
            seconds = 0;
            minutes++;
        }

        if (minutes >= 60) {
            minutes = 0;
            hours++;
        }

        if (hours >= 24) {
            hours = 0;
            days++;
        }

        // Update the display to reflect current time
        displayTime(days, hours, minutes, seconds);
    }
}

void displayTime(int days, int hours, int minutes, int seconds) {
    // Prepare time format as: DD:HH:MM:SS
    char timeStr[9];  // Buffer for display string
    snprintf(timeStr, sizeof(timeStr), "%02d%02d%02d%02d", days, hours, minutes, seconds);

    // Set the display with specific decimal points (none after seconds)
    tm1638.setDisplayToString(timeStr, 0b01010100);  // Adjust bitmask for decimals
}