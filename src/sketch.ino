#include <avr/sleep.h>
#include <SPI.h>

const byte LEFT_DRIVERS = 4;
const byte RIGHT_DRIVERS = 2;
const byte LEFT_LED_PAIRS = 16;
const byte RIGHT_LED_PAIRS = 7;

//int ledClockPin = 13;
//int ledDataPin = 11;
byte ledLatchPin = 10;
byte switchPin = 2;
byte leftPin = A1;

byte leftLit = 0;
byte rightLit = 0;
byte leftState[LEFT_DRIVERS];
byte rightState[RIGHT_DRIVERS];

/* 
 * Initialize the app.
 */
void setup() { 
    Serial.begin(9600);

    pinMode(ledLatchPin, OUTPUT);
    pinMode(switchPin, INPUT_PULLUP);
  
    SPI.begin();
    SPI.setBitOrder(LSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV2);

    leftLit = (float(analogRead(leftPin)) / 1000) * 16;

    // Setup initial state
    updateState();
    updateLeds();
} 

void updateState() {
    if (leftLit == 16) {
        leftState[0] = 0b10101010;
        leftState[1] = 0b10101010;
        leftState[2] = 0b10101010;
        leftState[3] = 0b10101010;
    } else if (leftLit >= 12) {
        leftState[0] = 0b10101010;
        leftState[1] = 0b10101010;
        leftState[2] = 0b10101010;

        if (leftLit == 12) {
            leftState[3] = 0b01010101;
        } else if (leftLit == 13) {
            leftState[3] = 0b10010101;
        } else if (leftLit == 14) {
            leftState[3] = 0b10100101;
        } else {
            leftState[3] = 0b10101001;
        }
    } else if (leftLit >= 8) {
        leftState[0] = 0b10101010;
        leftState[1] = 0b10101010;

        if (leftLit == 8) {
            leftState[2] = 0b01010101;
        } else if (leftLit == 9) {
            leftState[2] = 0b10010101;
        } else if (leftLit == 10) {
            leftState[2] = 0b10100101;
        } else {
            leftState[2] = 0b10101001;
        }

        leftState[3] = 0b01010101;
    } else if (leftLit >= 4) {
        leftState[0] = 0b10101010;
        
        if (leftLit == 4) {
            leftState[1] = 0b01010101;
        } else if (leftLit == 5) {
            leftState[1] = 0b10010101;
        } else if (leftLit == 6) {
            leftState[1] = 0b10100101;
        } else {
            leftState[1] = 0b10101001;
        }

        leftState[2] = 0b01010101;
        leftState[3] = 0b01010101;
    } else {
        if (leftLit == 0) {
            leftState[0] = 0b01010101;
        } else if (leftLit == 1) {
            leftState[0] = 0b10010101;
        } else if (leftLit == 2) {
            leftState[0] = 0b10100101;
        } else {
            leftState[0] = 0b10101001;
        }

        leftState[1] = 0b01010101;
        leftState[2] = 0b01010101;
        leftState[3] = 0b01010101;
    }
}

/*
 * Main loop. Do nothing since interrupts are running.
 */
void loop() { 
    byte newLeftLit = (float(analogRead(leftPin)) / 1000) * 16;

    Serial.println(newLeftLit);

    if (newLeftLit != leftLit) {
        leftLit = newLeftLit;

        updateState();
        updateLeds();
    }
}

/*
 * Utility function to set the state of all LEDs.
 */
void updateLeds() {
    digitalWrite(ledLatchPin, LOW);

    for (byte i = 0; i < LEFT_DRIVERS; i++) {
        SPI.transfer(leftState[i]);
    }

    for (byte i = 0; i < RIGHT_DRIVERS; i++) {
        SPI.transfer(rightState[i]);
    }

    digitalWrite(ledLatchPin, HIGH);
}

