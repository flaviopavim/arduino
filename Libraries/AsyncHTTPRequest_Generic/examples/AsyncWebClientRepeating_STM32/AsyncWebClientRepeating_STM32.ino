/****************************************************************************************************************************
  AsyncWebClientRepeating_STM32.ino - Dead simple AsyncHTTPRequest for ESP8266, ESP32 and currently STM32 with built-in LAN8742A Ethernet

  For ESP8266, ESP32 and STM32 with built-in LAN8742A Ethernet (Nucleo-144, DISCOVERY, etc)

  AsyncHTTPRequest_Generic is a library for the ESP8266, ESP32 and currently STM32 run built-in Ethernet WebServer

  Based on and modified from asyncHTTPrequest Library (https://github.com/boblemaire/asyncHTTPrequest)

  Built by Khoi Hoang https://github.com/khoih-prog/AsyncHTTPRequest_Generic
  Licensed under MIT license

  Copyright (C) <2018>  <Bob Lemaire, IoTaWatt, Inc.>
  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
  as published bythe Free Software Foundation, either version 3 of the License, or (at your option) any later version.
  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *****************************************************************************************************************************/

#include "defines.h"

// Select a test server address
const char GET_ServerAddress[] = "arduino.tips";

// GET location
String GET_Location = "/asciilogo.txt";

#define ASYNC_HTTP_REQUEST_GENERIC_VERSION_MIN_TARGET      "AsyncHTTPRequest_Generic v1.10.2"
#define ASYNC_HTTP_REQUEST_GENERIC_VERSION_MIN             1010002

// 600s = 10 minutes to not flooding, 60s in testing
#define HTTP_REQUEST_INTERVAL_MS     60000  //600000

// Uncomment for certain HTTP site to optimize
//#define NOT_SEND_HEADER_AFTER_CONNECTED        true

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include <AsyncHTTPRequest_Generic.h>             // https://github.com/khoih-prog/AsyncHTTPRequest_Generic

#include <Ticker.h>                             // https://github.com/sstaub/Ticker

AsyncHTTPRequest request;

void sendRequest();

// Repeat forever, millis() resolution
Ticker sendHTTPRequest(sendRequest, HTTP_REQUEST_INTERVAL_MS, 0, MILLIS);

void sendRequest()
{
	static bool requestOpenResult;

	if (request.readyState() == readyStateUnsent || request.readyState() == readyStateDone)
	{
		requestOpenResult = request.open("GET", (GET_ServerAddress + GET_Location).c_str());

		if (requestOpenResult)
		{
			// Only send() if open() returns true, or crash
			request.send();
		}
		else
		{
			Serial.println("Can't send bad request");
		}
	}
	else
	{
		Serial.println("Can't send request");
	}
}

void requestCB(void *optParm, AsyncHTTPRequest *request, int readyState)
{
	(void) optParm;

	if (readyState == readyStateDone)
	{
		AHTTP_LOGDEBUG(F("\n**************************************"));
		AHTTP_LOGDEBUG1(F("Response Code = "), request->responseHTTPString());

		if (request->responseHTTPcode() == 200)
		{
			Serial.println(F("\n**************************************"));
			Serial.println(request->responseText());
			Serial.println(F("**************************************"));
		}
	}
}

void setup()
{
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

	Serial.print("\nStart AsyncWebClientRepeating_STM32 on ");
	Serial.println(BOARD_NAME);
	Serial.println(ASYNC_HTTP_REQUEST_GENERIC_VERSION);

#if defined(ASYNC_HTTP_REQUEST_GENERIC_VERSION_MIN)

	if (ASYNC_HTTP_REQUEST_GENERIC_VERSION_INT < ASYNC_HTTP_REQUEST_GENERIC_VERSION_MIN)
	{
		Serial.print("Warning. Must use this example on Version equal or later than : ");
		Serial.println(ASYNC_HTTP_REQUEST_GENERIC_VERSION_MIN_TARGET);
	}

#endif
	// start the ethernet connection and the server
	// Use random mac
	uint16_t index = millis() % NUMBER_OF_MAC;

	// Use Static IP
	//Ethernet.begin(mac[index], ip);
	// Use DHCP dynamic IP and random mac
	Ethernet.begin(mac[index]);

	Serial.print(F("AsyncHTTPRequest @ IP : "));
	Serial.println(Ethernet.localIP());
	Serial.println();

	request.setDebug(false);

	request.onReadyStateChange(requestCB);
	sendHTTPRequest.start(); //start the ticker

	// Send first request now
	sendRequest();
}

void loop()
{
	sendHTTPRequest.update();
}
