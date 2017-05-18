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

#include "Nexa.h"

Nexa::Nexa(byte txPin) : txPin_(txPin) {
  pinMode(txPin, OUTPUT);
}

// Send a command to put on a receiver
// ID = transmitter ID (0 ... 67M)
// button = 1, 2, 3 or 4
void Nexa::sendOn(long id, byte button){
  sendCommand(id, button, 1, 0);
}

// Send a command to put off a receiver
// ID = transmitter ID (0 ... 67M)
// button = 1, 2, 3 or 4
void Nexa::sendOff(long id, byte button){
  sendCommand(id, button, 0, 0);
}

// Send a command to put dimmer receiver to an absolute value
// ID = transmitter ID (0 ... 67M)
// button = 1, 2, 3 or 4
// dimValue = 0...15
void Nexa::sendDim(long id, byte button, byte dimValue){
  sendCommand(id, button, 0, dimValue);
}

// Send a command to a receiver
void Nexa::sendCommand(long id, byte button, boolean value, byte dim){
  setBits(idArray, id, LENGTH_ID);
  setBits(groupArray, 0, LENGTH_GROUP);
  setBits(valueArray, value, LENGTH_VALUE);
  setBits(channelArray, 0, LENGTH_CHANNEL);
  setBits(buttonArray, button - 1, LENGTH_BUTTON);
  setBits(dimArray, dim, LENGTH_DIM);
  sendCommand(dim!=0);
}

void Nexa::setBits(boolean * dst, long number, byte length){
  for(byte i = 1; i <= length; i++)
    dst[length-i] = number & ((long)1 << (i-1));
}

// Sends data given in predefined arrays (with start and stop bit appended) four times
void Nexa::sendCommand(boolean dim){
  for (int i = 0; i < 4; i ++){
    sendBit('s');
    sendPackage(idArray, LENGTH_ID);
    sendPackage(groupArray, LENGTH_GROUP);
    if (!dim)
      sendPackage(valueArray, LENGTH_VALUE);
    else{
      sendRawBit('1'); // Dimmer call is only case in NEXA protocol where two raw bits can have the same value..
      sendRawBit('1');
    }
    sendPackage(channelArray, LENGTH_CHANNEL);
    sendPackage(buttonArray, LENGTH_BUTTON);
    if (dim)
      sendPackage(dimArray, LENGTH_DIM);
    sendBit('s');
    delayMicroseconds(LOW_DELAY_NEW_TRANSMIT-LOW_DELAY_START);
  }   
}

// Send the given data
void Nexa::sendPackage(boolean * data, byte length){
  for (int i = 0; i < length; i ++)
    sendBit('0' + data[i]);
}

// Send a complete bit (ie one -> 0,1 and zero -> 1,0)
// value = 's' -> startbit / stopbit
// value = '1' -> one
// value = '0' -> zero
void Nexa::sendBit(char value){
  if (value == 's')
    sendRawBit(value);
  else if (value == '0'){
    sendRawBit('1');
    sendRawBit('0');
  }
  else if (value == '1'){
    sendRawBit('0');
    sendRawBit('1');
  }
}

// Send raw bits
void Nexa::sendRawBit(char value){
  if (value != 's' && value != '0' && value != '1')
    return;
  highPulse(HIGH_DELAY);
  if (value == 's')
    delayMicroseconds(LOW_DELAY_START);
  else if (value == '1')
    delayMicroseconds(LOW_DELAY_ONE);
  else if (value == '0')
    delayMicroseconds(LOW_DELAY_ZERO);
}

// Send a high pulse with specified on time
void Nexa::highPulse(int delayHigh){
  digitalWrite(txPin_, HIGH);
  delayMicroseconds(delayHigh);
  digitalWrite(txPin_, LOW);
}
