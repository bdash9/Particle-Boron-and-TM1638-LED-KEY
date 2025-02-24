#include "Particle.h"
#include <TM1638.h>

// Custom TM1638 class for specific interactions
class TM1638Custom : public TM1638 {
public:
    TM1638Custom(uint8_t dataPin, uint8_t clockPin, uint8_t strobePin)
        : TM1638(dataPin, clockPin, strobePin) {}

    void sendSegment(uint8_t position, uint8_t segments) {
        sendChar(position, segments, false);
    }

    void displayGameOver() {
        const char* gameOverMessage = "GAMEOVER";
        for (int i = 0; i < 3; i++) {
            setDisplayToString(gameOverMessage);
            delay(1000); // Show for 1 second
            clearDisplay();
            delay(500);  // Off for half a second
        }
    }
};

// Initialize TM1638
TM1638Custom tm1638(D0, D1, D2);

// Segment definitions
#define SEG_A 0b00000001
#define SEG_B 0b00000010
#define SEG_C 0b00000100
#define SEG_D 0b00001000
#define SEG_E 0b00010000
#define SEG_F 0b00100000
#define SEG_G 0b01000000

const uint8_t ballSequence[][8] = {
    {SEG_C, 0, 0, 0, 0, 0, 0, 0},  // Digit 1, segment c
    {SEG_E, 0, 0, 0, 0, 0, 0, 0},  // Digit 1, segment e
    {SEG_G, 0, 0, 0, 0, 0, 0, 0},  // Digit 1, segment g
    {0, SEG_E, 0, 0, 0, 0, 0, 0},  // Digit 2, segment e
    {0, SEG_G, 0, 0, 0, 0, 0, 0},  // Digit 2, segment g
    {0, SEG_B, 0, 0, 0, 0, 0, 0},  // Digit 2, segment b
    {0, 0, SEG_A, 0, 0, 0, 0, 0},  // Digit 3, segment a
    {0, 0, 0, SEG_F, 0, 0, 0, 0},  // Digit 4, segment f
    {0, 0, 0, SEG_G, 0, 0, 0, 0},  // Digit 4, segment g
    {0, 0, 0, SEG_C, 0, 0, 0, 0},  // Digit 4, segment c
    {0, 0, 0, 0, SEG_E, 0, 0, 0},  // Digit 5, segment e
    {0, 0, 0, 0, SEG_G, 0, 0, 0},  // Digit 5, segment g
    {0, 0, 0, 0, SEG_B, 0, 0, 0},  // Digit 5, segment b
    {0, 0, 0, 0, 0, SEG_A, 0, 0},  // Digit 6, segment a
    {0, 0, 0, 0, 0, 0, SEG_F, 0},  // Digit 7, segment f
    {0, 0, 0, 0, 0, 0, SEG_G, 0},  // Digit 7, segment g
    {0, 0, 0, 0, 0, 0, SEG_C, 0}   // Digit 7, segment c
};

int ballPosition = 3;    // Start position at the first movable part within range
bool movingRight = true;

// Paddle states
bool leftPaddleState = true;  // true for SEG_C, false for SEG_B
bool rightPaddleState = true; // true for SEG_E, false for SEG_F

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

        // Detect simultaneous press of button 1 and button 8
        if ((buttons & (1 << 0)) && (buttons & (1 << 7))) {
            tm1638.clearDisplay();
            if (scrollText("Boro-PONG")) {
                loadGameFunction();  // Launch the Pong game
            }
            return;
        }

        for (int i = 0; i < 8; i++) {
            if (buttons & (1 << i)) {
                tm1638.setLEDs(0x00);
                tm1638.setLED(1, i);
                tm1638.clearDisplay(); // Clear "Bttn 1-8" when any button is pressed
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
    tm1638.setLEDs(0xFF); // Turn on all LEDs to indicate readiness for button press
    tm1638.setDisplayToString("Bttn 1-8"); // Display "Bttn 1-8" after initial sequence
    delay(1000); // Keep the message for a while
}

bool scrollText(const char* text) {
    int textLength = strlen(text);
    const int displayLength = 8;

    for (int start = 0; start <= textLength; start++) {
        char displayBuffer[displayLength + 1] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'};
        for (int i = 0; i < displayLength; i++) {
            if (start + i < textLength) {
                displayBuffer[i] = text[start + i];
            }
        }
        tm1638.setDisplayToString(displayBuffer);

        // Pause on "BORO"
        if (strncmp(displayBuffer, "BORO", 4) == 0) {
            delay(2000); // Pause for 2 seconds for "BORO"
        } else {
            delay(300);
        }
    }
    return true;
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
    // Placeholder for battery function logic without using FuelGauge
    Serial.println("Battery function not implemented.");
    tm1638.setDisplayToString("No BATT");
    delay(1500);
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

void loadGameFunction() {
    // Boro-PONG Game Logic
    Serial.println("Starting Boro-PONG...");

    // Initialized game variables
    ballPosition = 3;
    movingRight = true;
    leftPaddleState = true;
    rightPaddleState = true;

    while (true) {
        // Read button inputs
        byte buttons = tm1638.getButtons();

        // Toggle the left paddle position on button S1 press
        if (buttons & (1 << 0)) {
            leftPaddleState = !leftPaddleState;
            delay(200);
        }

        // Toggle the right paddle position on button S8 press
        if (buttons & (1 << 7)) {
            rightPaddleState = !rightPaddleState;
            delay(200);
        }

        // Check for collision with paddles to change direction
        if (ballPosition == 3 && !movingRight) {
            if (!leftPaddleState) {
                gameOver();
                break;
            }
            movingRight = true; // Bounce to the right when hitting the left paddle
        } else if (ballPosition == sizeof(ballSequence) / sizeof(ballSequence[0]) - 1 && movingRight) {
            if (!rightPaddleState) {
                gameOver();
                break;
            }
            movingRight = false; // Bounce to the left when hitting the right paddle
        }

        // Render ball and paddles
        for (int i = 0; i < 8; i++) {
            if (i == 0) {
                tm1638.sendSegment(i, leftPaddleState ? SEG_C : SEG_B);
            } else if (i == 7) {
                tm1638.sendSegment(i, rightPaddleState ? SEG_E : SEG_F);
            } else {
                tm1638.sendSegment(i, ballSequence[ballPosition][i]);
            }
        }

        // Synchronize LEDs with ball position
        uint8_t ledState = 1 << (ballPosition / 3);  // A simple division to determine LED
        tm1638.setLEDs(ledState);

        // Update ball position
        if (movingRight) {
            ballPosition++;
        } else {
            ballPosition--;
        }

        delay(200);  // Control speed

        if (tm1638.getButtons()) {
            break; // Exit game if any button is pressed
        }
    }

    tm1638.clearDisplay();
}

void gameOver() {
    const char* gameOverMessage = "GAMEOVER";
    for (int i = 0; i < 3; i++) {
        tm1638.setDisplayToString(gameOverMessage);
        delay(1000);
        tm1638.clearDisplay();
        delay(500);
    }
    resetGame();
}

void resetGame() {
    // Reset game to initial state
    ballPosition = 3;  // Start position at the first movable part within range
    movingRight = true;
    leftPaddleState = true;
    rightPaddleState = true;
    tm1638.clearDisplay();
}