# arduino-nexa #

Library object to controll on/off switches and dimmers using the NEXA 433mhz protocoll.  

Tested on the following brands: NEXA, ANSLUT, PROOVE

Example usage when a transmitter module is connected to pin 13:
```
#include <Nexa.h>

#define RADIO_TRANSMITTER_PIN 13

Nexa nexa(RADIO_TRANSMITTER_PIN);

void loop()
{
    long id = 1234567;           // Receivers unique id (self learned)
    button = 1;                  // Button number

    nexa.sendOn(id, button);     // Turn on
    delay(5000);
    nexa.sendDim(id, button, 7); // Set to 50% light
    delay(5000);
    nexa.sendOff(id, button);    // Turn off
}

```

# Copyright #

Daniel Falk  
+46 (0) 76-80 156 12  
daniel@da-robotteknik.se  

2016  
You are free to use and share but please leave an acknowledge.
Licensed under MIT license

