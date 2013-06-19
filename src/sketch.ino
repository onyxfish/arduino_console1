#include <avr/sleep.h>
#include <SPI.h>

//int ledClockPin = 13;
//int ledDataPin = 11;
int ledLatchPin = 10;

int switchPin = 2;

bool is_lit = false;

/* 
 * Initialize the app.
 */
void setup() { 
    pinMode(ledLatchPin, OUTPUT);
    pinMode(switchPin, INPUT_PULLUP);
  
    SPI.begin();
    SPI.setBitOrder(LSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV2);

    // Setup initial state
    is_lit = (digitalRead(switchPin) == HIGH);
    updateLeds();

    // Enable interrupts
    cli();
    PCICR = 0b00000100;     // PORTD, Pins PCINT16-23, digital pins 0-7
    PCMSK2 = 0b00000100;    // PCINT18, digital pin 2
    sei();

    // Go to sleep
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
    sleep_disable();
} 

/*
 * Main loop. Do nothing since interrupts are running.
 */
void loop() { 
}   

/*
 * Switch pin change interrupt.
 */
ISR(PCINT2_vect) {
    is_lit = (digitalRead(switchPin) == HIGH);
    updateLeds();
}

/*
 * Utility function to set the state of all LEDs.
 */
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

