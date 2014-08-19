/*
This file is part of the GSM3 communications library for Arduino
-- Multi-transport communications platform
-- Fully asynchronous
-- Includes code for the Arduino-Telefonica GSM/GPRS Shield V1
-- Voice calls
-- SMS
-- TCP/IP connections
-- HTTP basic clients

This library has been developed by Telefónica Digital - PDI -
- Physical Internet Lab, as part as its collaboration with
Arduino and the Open Hardware Community. 

September-December 2012

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

The latest version of this library can always be found at
https://github.com/BlueVia/Official-Arduino
*/

#include <GSM3ShieldV1Clock.h>

// constructor
GSM3ShieldV1Clock::GSM3ShieldV1Clock()
{
};

// reset the modem for direct access
int GSM3ShieldV1Clock::begin()
{
	int result=0;
	String modemResponse;
	
	// check modem response
	modemAccess.begin();

	// reset hardware
	modemAccess.restartModem();

	modemResponse=modemAccess.writeModemCommand("AT", 1000);
	if(modemResponse.indexOf("OK")>=0)
		result=1;
	modemResponse=modemAccess.writeModemCommand("ATE0", 1000);
	return result;
}

bool GSM3ShieldV1Clock::setTimeFromNetwork(unsigned long aWaitTime)
{
	unsigned long start = millis();
	bool timeSynchronized = false;

	String modemResponse=modemAccess.writeModemCommand("AT+CCLK?", 1000);
	if(modemResponse.indexOf("OK")==-1)
		return false;
	// Ask for network time synchronization
	modemResponse=modemAccess.writeModemCommand("AT+QNITZ=1", 1000);
	if(modemResponse.indexOf("OK")==-1)
		return false;

	// Now we need to wait for a network time sync message
	while (!timeSynchronized && (millis() < start+aWaitTime))
	{
		// Check when the last time synchronization message arrived
		// FIXME We'll get an unsolicited "+QNITZ=<time>,<ds>" message
		// FIXME which would be better to spot, but this is easier
		// FIXME for me to code up with minimal GSM library knowledge
		modemResponse = modemAccess.writeModemCommand("AT+QLTS", 2000);
		// Include space for the 
		char res_to_compare[modemResponse.length()];
		modemResponse.toCharArray(res_to_compare, modemResponse.length());
		if(modemResponse.indexOf("OK") != -1)
		{	
			// We'll either receive '+QLTS: ""\n\nOK' or 
                	// something like '+QLTS: "14/05/26,17:11:59+04,1"\n\nOK'
			if (modemResponse.charAt(10) != '"')
			{
				// The 9th character isn't a ", so assume it's
				// a date/time.
				// Set the clock
				String setCmd = "AT+CCLK=" + modemResponse.substring(9, 30) + "\"";
				modemResponse=modemAccess.writeModemCommand(setCmd, 1000);
				if(modemResponse.indexOf("OK")>=0)
				{
					timeSynchronized = true;
				}
			}
		}
		// No need to hammer the modem, checking approx each second will work
		delay(700);
	}
	// Turn off network time synchronization messages now, so
	// the unsolicited "+QNITZ=<time>,<ds>" messages won't confuse
	// anything elsewhere (not sure it will, but doesn't hurt to be nice)
	modemResponse=modemAccess.writeModemCommand("AT+QNITZ=0", 1000);
	if(modemResponse.indexOf("OK")==-1)
		return false;
	return timeSynchronized;
}

bool GSM3ShieldV1Clock::setTime(String aWaitTime)
{
	String cmd("AT+CCLK=\"");
	cmd += aWaitTime;
	cmd += "\"";
	// AT command to set the time
	String modemResponse = modemAccess.writeModemCommand(cmd, 2000);
	// Parse and check response
	char res_to_compare[modemResponse.length()];
	modemResponse.toCharArray(res_to_compare, modemResponse.length());
	if(modemResponse.indexOf("OK") == -1)
	{	
		return false;
	}
	else
	{
		return true;
	}
}

// get the time
String GSM3ShieldV1Clock::getTime()
{
	String the_time;
	// AT command for obtain time
	String modemResponse = modemAccess.writeModemCommand("AT+CCLK?", 2000);
	// Parse and check response
	char res_to_compare[modemResponse.length()];
	modemResponse.toCharArray(res_to_compare, modemResponse.length());
	if(modemResponse.indexOf("OK") == -1)
	{	
		return NULL;
	}
	else
	{
		the_time = modemResponse.substring(10, 30);
		return the_time;
	}
}
