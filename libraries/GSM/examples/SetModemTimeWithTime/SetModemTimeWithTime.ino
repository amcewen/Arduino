/*
 
 This example gets the time from the GSM network and sets
 the internal time (using the Time library written by
 Michael Margolis).

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
#include <Time.h>

// modem object
GSMClock modem;

String modemTime = "";


// Simple class to make it easier to print out time_t variables
class PrintTime : public Printable
{
public:
  PrintTime(time_t aTime) : iTime(aTime) {};
  virtual size_t printTo(Print& p) const;
protected:
  time_t iTime;
};

size_t PrintTime::printTo(Print& p) const
{
  size_t ret = 0;
  tmElements_t time_elements;
  
  breakTime(iTime, time_elements);
  ret += p.print(tmYearToCalendar(time_elements.Year));
  ret += p.print("-");
  if (time_elements.Month < 10)
  {
    ret += p.print("0");
  }
  ret += p.print(time_elements.Month);
  ret += p.print("-");
  if (time_elements.Day < 10)
  {
    ret += p.print("0");
  }
  ret += p.print(time_elements.Day);
  ret += p.print("T");
  if (time_elements.Hour < 10)
  {
    ret += p.print("0");
  }
  ret += p.print(time_elements.Hour);
  ret += p.print(":");
  if (time_elements.Minute < 10)
  {
    ret += p.print("0");
  }
  ret += p.print(time_elements.Minute);
  ret += p.print(":");
  if (time_elements.Second < 10)
  {
    ret += p.print("0");
  }
  ret += p.print(time_elements.Second);
  ret += p.print("Z");
  
  return ret;
}

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
    // Set the Time library time
    modemTime = modem.getTime();
    setTimeFromModem(modemTime);
  }
  else
  {
    Serial.println("No time signal yet");
  }
}

void loop()
{
  // get modem time
  Serial.print("Checking time...");
  modemTime = modem.getTime();
  
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
  Serial.print("Internal time: ");
  Serial.print(now());
  Serial.print(", ");
  PrintTime pt(now());
  Serial.println(pt);
  
  // do nothing for a bit
  delay(1000UL*10);
}

void setTimeFromModem(String& aTime)
{
  const int kDateTimeStringLength = 20;
  const int kYearOffset = 0;
  const int kYearLength = 2;
  const int kMonthOffset = 3;
  const int kMonthLength = 2;
  const int kDayOffset = 6;
  const int kDayLength = 2;
  const int kHourOffset = 9;
  const int kHourLength = 2;
  const int kMinuteOffset = 12;
  const int kMinuteLength = 2;
  const int kSecondOffset = 15;
  const int kSecondLength = 2;
  
  if (aTime.length() >= kDateTimeStringLength)
  {
    String component = aTime.substring(kYearOffset, kYearOffset+kYearLength);
    tmElements_t time_elements;
    Serial.println(component);
    time_elements.Year = y2kYearToTm(component.toInt());
    component = aTime.substring(kMonthOffset, kMonthOffset+kMonthLength);
    time_elements.Month = component.toInt();
    component = aTime.substring(kDayOffset, kDayOffset+kDayLength);
    time_elements.Day = component.toInt();
    component = aTime.substring(kHourOffset, kHourOffset+kHourLength);
    time_elements.Hour = component.toInt();
    component = aTime.substring(kMinuteOffset, kMinuteOffset+kMinuteLength);
    time_elements.Minute = component.toInt();
    component = aTime.substring(kSecondOffset, kSecondOffset+kSecondLength);
    time_elements.Second = component.toInt();
    
    // Now use that to set the time
    setTime(makeTime(time_elements));
  }
}


