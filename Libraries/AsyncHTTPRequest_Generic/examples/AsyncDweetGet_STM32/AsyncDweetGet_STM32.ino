/****************************************************************************************************************************
  AsyncDweetGET_STM32.ino - Dead simple AsyncHTTPRequest for ESP8266, ESP32 and currently STM32 with built-in LAN8742A Ethernet

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

/**
  Connects to dweet.io once every 1 minutes, sends a GET request and a request body.

  Shows how to use Strings to assemble path and parse content from response.
  dweet.io expects: https://dweet.io/dweet/for/thingName

  For more on dweet.io, see https://dweet.io/play/
 * */

#include "defines.h"

// Select a test server address
const char GET_ServerAddress[] = "dweet.io";

// use your own thing name here
String dweetName = "/dweet/for/currentSecond?second=";

#define ASYNC_HTTP_REQUEST_GENERIC_VERSION_MIN_TARGET      "AsyncHTTPRequest_Generic v1.10.2"
#define ASYNC_HTTP_REQUEST_GENERIC_VERSION_MIN             1010002

// 600s = 10 minutes to not flooding, 60s in testing
#define HTTP_REQUEST_INTERVAL_MS     60000  //600000

// Uncomment for certain HTTP site to optimize
//#define NOT_SEND_HEADER_AFTER_CONNECTED        true

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include <AsyncHTTPRequest_Generic.h>             // https://github.com/khoih-prog/AsyncHTTPRequest_Generic

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include <AsyncHTTPRequest_Impl_Generic.h>        // https://github.com/khoih-prog/AsyncHTTPRequest_Generic

#include <Ticker.h>                             // https://github.com/sstaub/Ticker

AsyncHTTPRequest request;

void sendRequest(void);

// Repeat forever, millis() resolution
Ticker sendHTTPRequest(sendRequest, HTTP_REQUEST_INTERVAL_MS, 0, MILLIS);

void sendRequest(void)
{
	static bool requestOpenResult;

	if (request.readyState() == readyStateUnsent || request.readyState() == readyStateDone)
	{
		requestOpenResult = request.open("GET", (GET_ServerAddress + dweetName + String(millis() / 1000)).c_str() );

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

void parseResponse(String responseText)
{
	/*
	  Typical response is:
	  {"this":"succeeded",
	  "by":"getting",
	  "the":"dweets",
	  "with":[{"thing":"my-thing-name",
	    "created":"2016-02-16T05:10:36.589Z",
	    "content":{"sensorValue":456}}]}

	  You want "content": numberValue
	*/
	// now parse the response looking for "content":
	int labelStart = responseText.indexOf("content\":");
	// find the first { after "content":
	int contentStart = responseText.indexOf("{", labelStart);
	// find the following } and get what's between the braces:
	int contentEnd = responseText.indexOf("}", labelStart);
	String content = responseText.substring(contentStart + 1, contentEnd);

	Serial.println(content);

	// now get the value after the colon, and convert to an int:
	int valueStart = content.indexOf(":");
	String valueString = content.substring(valueStart + 1);
	int number = valueString.toInt();

	Serial.print("Value string: ");
	Serial.println(valueString);
	Serial.print("Actual value: ");
	Serial.println(number);
}

void requestCB(void* optParm, AsyncHTTPRequest* request, int readyState)
{
	(void) optParm;

	if (readyState == readyStateDone)
	{
		Serial.println();
		AHTTP_LOGDEBUG(F("**************************************"));
		AHTTP_LOGDEBUG1(F("Response Code = "), request->responseHTTPString());

		if (request->responseHTTPcode() == 200)
		{
			String responseText = request->responseText();

			Serial.println("\n**************************************");
			//Serial.println(request->responseText());
			Serial.println(responseText);
			Serial.println("**************************************");

			parseResponse(responseText);

			request->setDebug(false);
		}
		else
		{
			AHTTP_LOGERROR(F("Response error"));
		}
	}
}

void setup(void)
{
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

	Serial.print("\nStart AsyncDweetGET_STM32 on ");
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
	sendHTTPRequest.start(); //start the ticker.
}

void loop(void)
{
	sendHTTPRequest.update();
}
