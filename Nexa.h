/* 433 MHz transmitter for wireless wall sockets. Tested with NEXA, ANSLUT and PROOVE brands.
 * Works for on/off switches and dimmers.
 *
 * Daniel Falk
 * DA-Robotteknik
 * +46 (0) 76-80 156 12
 * daniel @T da-robotteknik.se
 *
 * 2016
 * You are free to use and share but please leave a acknowledge.
 *
 */

#ifndef NEXA_H
#define NEXA_H

#include "Arduino.h"

// Define delays for different part of the signal
#define HIGH_DELAY 278 //µS
#define LOW_DELAY_ONE 300 //µS
#define LOW_DELAY_ZERO 1343 //µS
#define LOW_DELAY_START 3174 // Delay after startbit (µS)
#define LOW_DELAY_NEW_TRANSMIT 12941 // Time between repeated transmissions (µS)

// Define length of data
#define LENGTH_ID 26
#define LENGTH_GROUP 1
#define LENGTH_VALUE 1
#define LENGTH_CHANNEL 2
#define LENGTH_BUTTON 2
#define LENGTH_DIM 4

class Nexa {
  public:
    Nexa(byte txPin);

    // Send a command to put on a receiver
    void sendOn(long id, byte button);
   
    // Send a command to put off a receiver
    void sendOff(long id, byte button);
   
    // Send a command to put dimmer receiver to an absolute value (1-16)
    void sendDim(long id, byte button, byte dimValue);

  private:
    const byte txPin_;
    boolean idArray[LENGTH_ID];
    boolean groupArray[LENGTH_GROUP];
    boolean valueArray[LENGTH_VALUE];
    boolean channelArray[LENGTH_CHANNEL];
    boolean buttonArray[LENGTH_BUTTON];
    boolean dimArray[LENGTH_DIM];
   
    // Send a command to a receiver
    void sendCommand(long id, byte button, boolean value, byte dim);
   
    void setBits(boolean * dst, long number, byte length);
   
    // Sends data given in predefined arrays (with start and stop bit appended) four times
    void sendCommand(boolean dim);
   
    // Send the given data
    void sendPackage(boolean * data, byte length);
   
    // Send a complete bit (ie one -> 0,1 and zero -> 1,0)
    // value = 's' -> startbit / stopbit
    // value = '1' -> one
    // value = '0' -> zero
    void sendBit(char value);
   
    // Send raw bits
    void sendRawBit(char value);
   
    // Send a high pulse with specified on time
    void highPulse(int delayHigh);
};

#endif
