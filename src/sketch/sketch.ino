#include <SPI.h>

//int ledClockPin = 13;
//int ledDataPin = 11;
int ledLatchPin = 10;

int switchPin = 2;

bool is_lit = false;

void setup() { 
    pinMode(ledLatchPin, OUTPUT);

    pinMode(switchPin, INPUT);
  
    SPI.begin();
    SPI.setBitOrder(LSBFIRST);
} 

void loop() { 
    int switchState = (digitalRead(switchPin) == HIGH);

    // Check if state changed
    if (switchState != is_lit) {
        is_lit = switchState;

        digitalWrite(ledLatchPin, LOW);

        SPI.transfer(is_lit);
        SPI.transfer(!is_lit);
        SPI.transfer(is_lit);
        SPI.transfer(!is_lit);
        SPI.transfer(is_lit);
        SPI.transfer(!is_lit);

        digitalWrite(ledLatchPin, HIGH);
    }

    delay(100);
}   
