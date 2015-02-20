/*
  Digital Pot Control
  
  This example controls an Analog Devices AD5206 digital potentiometer.
  The AD5206 has 6 potentiometer channels. Each channel's pins are labeled
  A - connect this to voltage
  W - this is the pot's wiper, which changes when you set it
  B - connect this to ground.
 
 The AD5206 is SPI-compatible,and to command it, you send two bytes, 
 one with the channel number (0 - 5) and one with the resistance value for the
 channel (0 - 255).  
 
 The circuit:
  * All A pins  of AD5206 connected to +5V
  * All B pins of AD5206 connected to ground
  * An LED and a 220-ohm resisor in series connected from each W pin to ground
  * CS - to digital pin 10  (SS pin)
  * SDI - to digital pin 11 (MOSI pin)
  * CLK - to digital pin 13 (SCK pin)
 
 created 10 Aug 2010 
 by Tom Igoe
 
 Thanks to Heather Dewey-Hagborg for the original tutorial, 2005
 
*/


// inslude the SPI library:
#include <SPI.h>


// here we set up the negative and positive pin phase outputs
const int positive_pin = 12;
const int slaveSelectPin = 10;
const int sensorPin = A0;

int sensorValue = 0;

int isi_ms = 100;           // milliseconds between end of negative on and beginning of positive on
int freq = 100;              // not currently used variable
int pulse_width_us = 80;    // microseconds that positive pulse width should last
int trough_width_us = 300;    // microse

void setup() {
 
  pinMode (slaveSelectPin, OUTPUT);
  // initialize SPI:
  SPI.begin();   
  
  pinMode(positive_pin, OUTPUT);
  
}
void positive_on() {
  // turn on positive pin, off negative pin
  digitalWrite(positive_pin, HIGH);
  delayMicroseconds(pulse_width_us);
}

  // here we check to see which pin is producing an output, then we switch the pins.
  
void negative_on() {
  
  digitalWrite(positive_pin, LOW);
  delayMicroseconds(trough_width_us);
}
  
void turn_both_off() {
  //digitalWrite(positive_pin, LOW);
  //digitalWrite(negative_pin, LOW);
  // delays the amount of time for the remainder of the desired frequency
  delay(isi_ms-trough_width_us/1000-pulse_width_us/1000);
}

void digitalPotWrite(int address, int value) {
  // take the SS pin low to select the chip:
  digitalWrite(slaveSelectPin,LOW);
  //  send in the address and value via SPI:
  SPI.transfer(address);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin,HIGH); 
}

void checkInput() {
 
  sensorValue = analogRead(sensorPin)/4; // yields a value of 0 through 255

}

void loop() {
  
  positive_on();
  negative_on();
  turn_both_off();
  
   //the below is just sample code of how to write a value to the digiPot
  checkInput();
  digitalPotWrite(0, sensorValue);
  
}

