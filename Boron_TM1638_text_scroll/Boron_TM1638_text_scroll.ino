#include <TM1638.h>  // Include the TM1638 library

// Initialize the TM1638 with specified pins for DATA, CLOCK, and STROBE
TM1638 tm1638(D0, D1, D2);

void setup() {
    Serial.begin(9600);  // Begin serial communication for potential debugging if needed

    // Clear the display initially
    tm1638.clearDisplay();
}

void loop() {
    // Fixed initial text to display
    const char* initialText = "Boron    ";  // Padded to fit display width for centering
    const char* message = "Boron Thing ";

    // Display the initial text "Boron" and pause for 1 second
    tm1638.setDisplayToString(initialText);
    delay(1000);  // Pause for 1 second

    // Scroll the message "Boron Thing" across the display
    scrollTextOnDisplay(message);

    // Optional: Delay between complete cycles
    delay(1000);  // Wait a second before restarting the scrolling
}

void scrollTextOnDisplay(const char* text) {
    size_t len = strlen(text);

    // Scroll text by updating display positions over time
    for (int offset = 0; offset < len; offset++) {
        char displayBuffer[9] = {0};  // Buffer for display (8 chars max) plus null terminator

        for (int i = 0; i < 8; i++) {
            if (offset + i < len) {
                displayBuffer[i] = text[offset + i];  // Fill display buffer with the current text segment
            } else {
                break;  // End of message reached
            }
        }

        // Output the current buffer state to the TM1638 display
        tm1638.setDisplayToString(displayBuffer);
        delay(300);  // Adjust scrolling speed (300 ms per update)
    }
}