#include <SPI.h>

//int ledClockPin = 13;
//int ledDataPin = 11;
int ledLatchPin = 10;

int switchPin = 2;

bool is_lit = false;

void updateLeds() {
    digitalWrite(ledLatchPin, LOW);

    SPI.transfer(is_lit);
    SPI.transfer(!is_lit);
    SPI.transfer(is_lit);
    SPI.transfer(!is_lit);
    SPI.transfer(is_lit);
    SPI.transfer(!is_lit);

    digitalWrite(ledLatchPin, HIGH);
}

void setup() { 
    pinMode(ledLatchPin, OUTPUT);

    pinMode(switchPin, INPUT_PULLUP);
  
    SPI.begin();
    SPI.setBitOrder(LSBFIRST);

    is_lit = (digitalRead(switchPin) == HIGH);
    updateLeds();
} 

void loop() { 
    bool switchState = (digitalRead(switchPin) == HIGH);

    // Check if state changed
    if (switchState != is_lit) {
        is_lit = switchState;

        updateLeds();
    }

    delay(100);
}   
