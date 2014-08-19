/*
 
 This example gets the time from the GSM network

 Requires support for network time messages from your GSM
 provider, so might not work
 
 Circuit:
 * GSM shield attached 
 
 Created Jun 2014
 by Adrian McEwen - MCQN Ltd
 
 This sample code is part of the public domain
 
 */

// libraries
#include <GSM.h>

// modem object
GSMClock modem;

// IMEI variable
String IMEI = "";

void setup()
{
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  // start modem test (reset and check response)
  Serial.print("Starting modem test...");
  if(modem.begin()) 
    Serial.println("modem.begin() succeeded");
  else
    Serial.println("ERROR, no modem answer.");

  if (modem.setTimeFromNetwork(5*6*1000UL))
  {
    Serial.println("We've received the time from the network");
  }
  else
  {
    Serial.println("No time signal yet");
  }
}

void loop()
{
  String modemTime = modem.getTime();
  
  // check response
  if(modemTime != NULL)
  {
    // show the time in serial monitor
    Serial.println("Modem's time: " + modemTime);
  }
  else
  {
    Serial.println("Error: Could not get time");
  }

  // do nothing for a bit
  delay(1000UL*10);
}

