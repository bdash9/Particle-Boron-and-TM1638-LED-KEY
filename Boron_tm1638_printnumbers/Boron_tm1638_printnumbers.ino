// // This works

// This #include statement was automatically added by the Particle IDE.
#include <TM1638.h>  // Make sure this is the correct library for your TM1638 module

// Initialize the TM1638 module with defined pin numbers
TM1638 tm1638(D0, D1, D2);  // Example pin assignments (Strobe, Clock, Data)

void setup() {
  // Optionally, any initialization specific to your TM1638
  // It's often just the constructor, so this might be empty
}

void loop() {
  // Test the display with known numeric values for troubleshooting
  displayFixedText();
  
  // Once testing is confirmed, remove the displayFixedText() and uncomment below

  // CellularSignal sig = Cellular.RSSI();  // Get cellular signal information
  // int signalStrength = sig.getStrength();  // Get the signal strength value

  // displaySignalStrength(signalStrength);

  delay(5000); // Update every 5 seconds
}

void displayFixedText() {
  // Sample numeric display, to ensure TM1638 works with recognized digits
  tm1638.setDisplayDigit(1, 0, false); // Display '1' at position 0
  tm1638.setDisplayDigit(2, 1, false); // Display '2' at position 1
  tm1638.setDisplayDigit(3, 2, false); // Display '3' at position 2
  tm1638.setDisplayDigit(4, 3, false); // Display '4' at position 3
  tm1638.setDisplayDigit(5, 4, false); // Display '5' at position 4
  tm1638.setDisplayDigit(6, 5, false); // Display '6' at position 5
  tm1638.setDisplayDigit(7, 6, false); // Display '7' at position 6
  tm1638.setDisplayDigit(8, 7, false); // Display '8' at position 7
}

void displaySignalStrength(int strength) {
  char buffer[9]; // Buffer to hold the string representation of signal strength
  snprintf(buffer, sizeof(buffer), "Sig:%d", strength);

  // Assuming an appropriate method to send characters to the display
  // Adjust your library call to correctly map characters to TM1638 segments
  for (int i = 0; i < 8 && buffer[i] != '\0'; i++) {
    tm1638.setDisplayDigit(buffer[i] - '0', i, false); // Convert char to digit index
  }
}