#include <avr/sleep.h>
#include <SPI.h>

/*
    Driver A = Upper left = leftState[0]
    Driver B = leftState[1]
    Driver C = leftState[2]
    Driver D = Lower left = leftState[3]

    Driver E = Upper Right = rightState[0]
    Driver F = Lower Right rightState[1]
*/

const byte LEFT_DRIVERS = 4;
const byte RIGHT_DRIVERS = 2;
const byte LEFT_LED_PAIRS = 16;
const byte RIGHT_LED_PAIRS = 7;

//int ledClockPin = 13;
//int ledDataPin = 11;
byte ledLatchPin = 10;
byte leftSwitchPin = 2;
byte rightSwitchPin = 3;
byte leftPotPin = A1;
byte rightPotPin = A2;

bool leftOn = false;
bool rightOn = false;
byte leftNumLit = 0;
byte rightNumLit = 0;
byte leftState[LEFT_DRIVERS];
byte rightState[RIGHT_DRIVERS];

/* 
 * Initialize the app.
 */
void setup() { 
    //Serial.begin(9600);

    pinMode(ledLatchPin, OUTPUT);
    pinMode(leftSwitchPin, INPUT_PULLUP);
    pinMode(rightSwitchPin, INPUT_PULLUP);
  
    SPI.begin();
    SPI.setBitOrder(LSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV2);

    leftOn = (digitalRead(leftSwitchPin) == LOW);
    rightOn = (digitalRead(rightSwitchPin) == LOW);
    leftNumLit = readNumLit(leftPotPin, 16);
    rightNumLit = readNumLit(rightPotPin, 8);

    // Setup initial state
    updateLeftState();
    updateRightState();
    updateLeds();
} 

byte readNumLit(byte pin, byte maxVal) {
    byte num = (float(analogRead(pin)) / 800) * 16;

    if (num > maxVal) {
        num = maxVal;
    }

    return maxVal - num;
}

void updateLeftState() {
    if (!leftOn) {
        leftState[0] = 0;
        leftState[1] = 0;
        leftState[2] = 0;
        leftState[3] = 0;

        return;
    }

    if (leftNumLit == 16) {
        leftState[0] = 0b10101010;
        leftState[1] = 0b10101010;
        leftState[2] = 0b10101010;
        leftState[3] = 0b10101010;
    } else if (leftNumLit >= 12) {
        leftState[0] = 0b10101010;
        leftState[1] = 0b10101010;
        leftState[2] = 0b10101010;

        if (leftNumLit == 12) {
            leftState[3] = 0b01010101;
        } else if (leftNumLit == 13) {
            leftState[3] = 0b10010101;
        } else if (leftNumLit == 14) {
            leftState[3] = 0b10100101;
        } else {
            leftState[3] = 0b10101001;
        }
    } else if (leftNumLit >= 8) {
        leftState[0] = 0b10101010;
        leftState[1] = 0b10101010;

        if (leftNumLit == 8) {
            leftState[2] = 0b01010101;
        } else if (leftNumLit == 9) {
            leftState[2] = 0b10010101;
        } else if (leftNumLit == 10) {
            leftState[2] = 0b10100101;
        } else {
            leftState[2] = 0b10101001;
        }

        leftState[3] = 0b01010101;
    } else if (leftNumLit >= 4) {
        leftState[0] = 0b10101010;
        
        if (leftNumLit == 4) {
            leftState[1] = 0b01010101;
        } else if (leftNumLit == 5) {
            leftState[1] = 0b10010101;
        } else if (leftNumLit == 6) {
            leftState[1] = 0b10100101;
        } else {
            leftState[1] = 0b10101001;
        }

        leftState[2] = 0b01010101;
        leftState[3] = 0b01010101;
    } else {
        if (leftNumLit == 0) {
            leftState[0] = 0b01010101;
        } else if (leftNumLit == 1) {
            leftState[0] = 0b10010101;
        } else if (leftNumLit == 2) {
            leftState[0] = 0b10100101;
        } else {
            leftState[0] = 0b10101001;
        }

        leftState[1] = 0b01010101;
        leftState[2] = 0b01010101;
        leftState[3] = 0b01010101;
    }
}

void updateRightState() {
    if (!rightOn) {
        rightState[0] = 0;
        rightState[1] = 0;

        return;
    }

    if (rightNumLit >= 8) {
        rightState[0] = 0b10101010;
        rightState[1] = 0b10101010;
    } else if (rightNumLit >= 4) {
        rightState[0] = 0b10101010;
        
        if (rightNumLit == 4) {
            rightState[1] = 0b01010101;
        } else if (rightNumLit == 5) {
            rightState[1] = 0b10010101;
        } else if (rightNumLit == 6) {
            rightState[1] = 0b10100101;
        } else {
            rightState[1] = 0b10101001;
        }
    } else {
        if (rightNumLit == 0) {
            rightState[0] = 0b01010101;
        } else if (rightNumLit == 1) {
            rightState[0] = 0b10010101;
        } else if (rightNumLit == 2) {
            rightState[0] = 0b10100101;
        } else {
            rightState[0] = 0b10101001;
        }

        rightState[1] = 0b01010101;
    }
}


/*
 * Main loop. Do nothing since interrupts are running.
 */
void loop() { 
    bool newLeftOn = (digitalRead(leftSwitchPin) == LOW);
    bool newRightOn = (digitalRead(rightSwitchPin) == LOW);
    byte newLeftNumLit = readNumLit(leftPotPin, 16);
    byte newRightNumLit = readNumLit(rightPotPin, 8);

    if (newRightOn != rightOn || newRightNumLit  != rightNumLit || newLeftOn != leftOn|| newLeftNumLit != leftNumLit) {
        /*Serial.println(newLeftOn);
        Serial.println(newRightOn);
        Serial.println(newLeftNumLit);
        Serial.println(newRightNumLit);*/
        
        leftOn = newLeftOn;
        rightOn = newRightOn;
        leftNumLit = newLeftNumLit;
        rightNumLit = newRightNumLit;

        updateLeftState();
        updateRightState();
        updateLeds();
    }
}

/*
 * Utility function to set the state of all LEDs.
 */
void updateLeds() {
    digitalWrite(ledLatchPin, LOW);

    SPI.transfer(rightState[1]);
    SPI.transfer(rightState[0]);
    SPI.transfer(leftState[3]);
    SPI.transfer(leftState[2]);
    SPI.transfer(leftState[1]);
    SPI.transfer(leftState[0]);

    digitalWrite(ledLatchPin, HIGH);
}

