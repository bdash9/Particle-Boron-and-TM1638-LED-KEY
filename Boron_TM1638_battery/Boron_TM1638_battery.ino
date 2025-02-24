#include <Particle.h>
#include <TM1638.h>

// Setup TM1638 with DATA, CLOCK, STROBE pins
TM1638 tm1638(D0, D1, D2);

void setup() {
    Serial.begin(9600);  // Initialize Serial communication for debugging
    tm1638.clearDisplay();

    // Enable the battery fuel gauge
    FuelGauge fuel;
    fuel.begin();
}

void loop() {
    FuelGauge fuel;

    // Read the battery state of charge
    float batteryPercentage = fuel.getSoC();  // Get battery SOC as a percentage

    // Prepare a display string
    char displayStr[9];
    snprintf(displayStr, sizeof(displayStr), "Batt%3.0f", batteryPercentage);

    // Output to serial for debugging
    Serial.print("Battery Life: ");
    Serial.print(batteryPercentage);
    Serial.println("%");

    // Display this battery percentage on the TM1638
    tm1638.setDisplayToString(displayStr);

    delay(5000);  // Update every 5 seconds
}