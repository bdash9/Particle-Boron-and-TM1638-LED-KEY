#include <TM1638.h>

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

void setup() {
    Serial.begin(9600);
    tm1638.clearDisplay();
}

void loop() {
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
            return;
        }
        movingRight = true; // Bounce to the right when hitting the left paddle
    } else if (ballPosition == sizeof(ballSequence) / sizeof(ballSequence[0]) - 1 && movingRight) {
        if (!rightPaddleState) {
            gameOver();
            return;
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
}

// Function to handle game over logic
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

// Function to reset the game
void resetGame() {
    ballPosition = 3;  // Reset to first valid move position
    movingRight = true;
    leftPaddleState = true;
    rightPaddleState = true;
    tm1638.clearDisplay();
}