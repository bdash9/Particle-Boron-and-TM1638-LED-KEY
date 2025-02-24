#include "Particle.h"
#include <TM1638.h>
//#include "FuelGauge.h"

// Initialize TM1638 with DATA, CLOCK, STROBE pins
TM1638 tm1638(D0, D1, D2);
FuelGauge fuelGauge;  // Initialize the fuel gauge for battery monitoring

SYSTEM_MODE(MANUAL);  // Set system mode to MANUAL for direct BLE control

void setup() {
    Serial.begin(9600);
    tm1638.setupDisplay(true, 7);
    tm1638.clearDisplay();
    tm1638.setLEDs(0x00);

    // Set time to Eastern Standard Time (UTC-5)
    Time.zone(-5);

    if (!Particle.connected()) {
        Particle.connect();
    }
    waitUntil(Particle.connected);

    Time.setDSTOffset(1);

    Serial.println("Setup complete - Check connections and serial setup.");

    initialScreenSequence();
}

void loop() {
    checkButtons();
}

void checkButtons() {
    static byte lastButtons = 0;
    byte buttons = tm1638.getButtons();

    if (buttons != lastButtons) {
        lastButtons = buttons;

        for (int i = 0; i < 8; i++) {
            if (buttons & (1 << i)) {
                tm1638.setLEDs(0x00);
                tm1638.setLED(1, i);
                Serial.printf("Button %d pressed\n", i + 1);
                processButtonAction(i + 1);
                return;
            }
        }
    }

    delay(100);
}

void initialScreenSequence() {
    const char* initialText = "Boron    ";
    const char* message = "Boron Thing ";
    tm1638.setDisplayToString(initialText);
    delay(1000);
    scrollTextOnDisplay(message);

    for (int cycle = 0; cycle < 2; cycle++) {
        for (int i = 0; i < 8; i++) {
            tm1638.setLED(1, i);
            delay(100);
        }
        for (int i = 7; i >= 0; i--) {
            tm1638.setLED(1, i);
            delay(100);
        }
    }
    tm1638.setLEDs(0x00);
    tm1638.clearDisplay();
    delay(1000);
}

void scrollTextOnDisplay(const char* text) {
    size_t len = strlen(text);

    for (int offset = 0; offset < len; offset++) {
        char displayBuffer[9] = {0};

        for (int i = 0; i < 8; i++) {
            if (offset + i < len) {
                displayBuffer[i] = text[offset + i];
            } else {
                displayBuffer[i] = ' ';
            }
        }

        tm1638.setDisplayToString(displayBuffer);
        delay(300);
    }
}

void processButtonAction(int buttonNumber) {
    switch (buttonNumber) {
        case 1:
            displayFunctionName("CLOCK");
            loadClockFunction();
            break;
        case 2:
            displayFunctionName("TIMER");
            loadTimerFunction();
            break;
        case 3:
            displayFunctionName("BATTERY");
            loadBatteryFunction();
            break;
        case 4:
            displayFunctionName("BT SCAN");
            loadBluetoothScannerFunction();
            break;
        case 5:
            displayFunctionName("CELL SIG");
            loadCellSignalFunction();
            break;
        case 6:
            displayFunctionName("FREE MEM");
            loadFreeMemFunction();
            break;
        case 7:
            displayFunctionName("SYS VER");
            loadSysVersionFunction();
            break;
        case 8:
            displayFunctionName("UPTIME");
            loadUptimeFunction();
            break;
        default:
            Serial.println("Unknown button number");
            break;
    }
    delay(200);
    tm1638.getButtons();
}

void displayFunctionName(const char* name) {
    tm1638.setDisplayToString(name);
    delay(1500);
    tm1638.clearDisplay();
}

void loadClockFunction() {
    unsigned long lastUpdate = 0;
    const unsigned long updateInterval = 1000;

    while (true) {
        unsigned long currentMillis = millis();
        if (currentMillis - lastUpdate >= updateInterval) {
            lastUpdate = currentMillis;
            char timeStr[9];
            snprintf(timeStr, sizeof(timeStr), "%02d.%02d.%02d", Time.hour(), Time.minute(), Time.second());
            tm1638.setDisplayToString(timeStr);
            Serial.println(timeStr);
        }
        if (tm1638.getButtons()) break;
    }

    tm1638.clearDisplay();
}

void loadTimerFunction() {
    unsigned long previousMillis = 0;
    const long interval = 1000;
    int hours = 0, minutes = 0, seconds = 0;

    while (true) {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            seconds++;
            if (seconds >= 60) {
                seconds = 0;
                minutes++;
            }
            if (minutes >= 60) {
                minutes = 0;
                hours++;
            }
            if (hours >= 100) {
                hours = 0;
            }
            char timeStr[9];
            snprintf(timeStr, sizeof(timeStr), "%02d %02d %02d", hours, minutes, seconds);
            tm1638.setDisplayToString(timeStr);
            Serial.println(timeStr);
        }
        if (tm1638.getButtons()) break;
    }

    tm1638.clearDisplay();
}

void loadBatteryFunction() {
    Serial.println("Executing Battery Function Logic");

    unsigned long lastUpdate = 0;
    const unsigned long updateInterval = 5000;

    while (true) {
        unsigned long currentMillis = millis();
        if (currentMillis - lastUpdate >= updateInterval) {
            lastUpdate = currentMillis;

            // Get battery percentage using the FuelGauge library
            float batteryPercentage = fuelGauge.getSoC();

            char batteryStr[9];
            snprintf(batteryStr, sizeof(batteryStr), "Batt %3.0f", batteryPercentage);
            tm1638.setDisplayToString(batteryStr);
            Serial.print("Battery Percentage: ");
            Serial.println(batteryPercentage);
        }

        if (tm1638.getButtons()) break;
    }

    tm1638.clearDisplay();
}

void loadBluetoothScannerFunction() {
    Serial.println("Executing Bluetooth Scanner Logic");
    unsigned long lastScanTime = millis();
    while (true) {
        unsigned long currentTime = millis();
        if (currentTime - lastScanTime >= 15000) {
            lastScanTime = currentTime;
            Serial.println("Starting BLE scan...");
            BLE.scan(scanResultCallback, NULL);
            delay(10000);
            BLE.stopScanning();
        }
        if (tm1638.getButtons()) break;
    }

    tm1638.clearDisplay();
}

void scanResultCallback(const BleScanResult* scanResult, void* context) {
    const BleAddress& addr = scanResult->address();
    char addressStr[18];
    snprintf(addressStr, sizeof(addressStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

    String deviceName = scanResult->advertisingData().deviceName();

    Serial.print("Detected BLE Device: ");
    Serial.print(addressStr);
    if (deviceName.length() > 0) {
        Serial.print(", Name: ");
        Serial.print(deviceName);
    }
    Serial.println();

    if (deviceName.length() > 0) {
        scrollTextOnDisplay(deviceName.c_str());
    } else {
        scrollTextOnDisplay(addressStr);
    }
}

void loadCellSignalFunction() {
    unsigned long lastUpdate = 0;

    while (true) {
        unsigned long currentMillis = millis();
        if (currentMillis - lastUpdate >= 5000) {
            lastUpdate = currentMillis;
            int signalStrength = random(0, 100);
            char displayStr[9];
            snprintf(displayStr, sizeof(displayStr), "Sig %3d", signalStrength);
            tm1638.setDisplayToString(displayStr);
            Serial.print("Signal Strength: ");
            Serial.println(signalStrength);
        }
        if (tm1638.getButtons()) break;
    }

    tm1638.clearDisplay();
}

void loadFreeMemFunction() {
    unsigned long lastUpdate = 0;

    while (true) {
        unsigned long currentMillis = millis();
        if (currentMillis - lastUpdate >= 2000) {
            lastUpdate = currentMillis;
            int freeMemory = System.freeMemory();
            char displayStr[9];
            snprintf(displayStr, sizeof(displayStr), "Mem %4d", freeMemory);
            tm1638.setDisplayToString(displayStr);
            Serial.print("Free Memory: ");
            Serial.println(freeMemory);
        }
        if (tm1638.getButtons()) break;
    }

    tm1638.clearDisplay();
}

void loadSysVersionFunction() {
    const char* systemVersion = System.version();
    tm1638.setDisplayToString(systemVersion);
    Serial.println(systemVersion);
    delay(1500);
    tm1638.clearDisplay();
}

void loadUptimeFunction() {
    unsigned long lastUpdate = 0;

    while (true) {
        unsigned long currentMillis = millis();
        if (currentMillis - lastUpdate >= 1000) {
            lastUpdate = currentMillis;
            unsigned long uptimeMillis = millis();
            unsigned long seconds = uptimeMillis / 1000;
            unsigned long minutes = (seconds / 60) % 60;
            unsigned long hours = (seconds / 3600);
            seconds %= 60;
            char displayStr[9];
            snprintf(displayStr, sizeof(displayStr), "%02lu:%02lu:%02lu", hours, minutes, seconds);
            tm1638.setDisplayToString(displayStr);
            Serial.printf("Uptime: %02lu:%02lu:%02lu\n", hours, minutes, seconds);
        }
        if (tm1638.getButtons()) break;
    }

    tm1638.clearDisplay();
}