#include <TM1638.h>  // Ensure the TM1638 library is correctly included

// Configure the TM1638 with DATA, CLOCK, STROBE pins
TM1638 tm1638(D0, D1, D2);

void setup() {
    Serial.begin(9600);  // Set up Serial for debugging
}

void loop() {
    CellularSignal sig = Cellular.RSSI();  // Fetch the cellular signal information
    int signalStrength = sig.getStrength();  // Get the RSSI value

    // Print signal strength for debugging
    Serial.print("Signal Strength: ");
    Serial.println(signalStrength);

    // Update the TM1638 display with updated logic
    displaySignalStrength(signalStrength);

    delay(5000);  // Refresh every 5 seconds
}

void displaySignalStrength(int strength) {
    tm1638.clearDisplay();  // Clear any prior display content

    String displayStr = " ";  // Begin with a space to align elements

    // Append the minus sign and the absolute signal strength
    displayStr += "-";
    displayStr += String(abs(strength)); // Display -42 or -43 or such

    // Shift "dBm" one position to the left
    displayStr += "dbnn";

    // Remove the last characters we don't want
    displayStr = displayStr.substring(0, 8);

    // Display the formatted string on the TM1638
    tm1638.setDisplayToString(displayStr.c_str());
}