/****************************************************************************************************************************
  AsyncHTTPRequest_ESP_WiFiManager.ino - Dead simple AsyncHTTPRequest for ESP8266, ESP32 and currently STM32 with built-in LAN8742A Ethernet

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
//************************************************************************************************************
//
// There are scores of ways to use AsyncHTTPRequest.  The important thing to keep in mind is that
// it is asynchronous and just like in JavaScript, everything is event driven.  You will have some
// reason to initiate an asynchronous HTTP request in your program, but then sending the request
// headers and payload, gathering the response headers and any payload, and processing
// of that response, can (and probably should) all be done asynchronously.
//
// In this example, a Ticker function is setup to fire every 300 seconds to initiate a request.
// Everything is handled in AsyncHTTPRequest without blocking.
// The callback onReadyStateChange is made progressively and like most JS scripts, we look for
// readyState == 4 (complete) here.  At that time the response is retrieved and printed.
//
// Note that there is no code in loop().  A code entered into loop would run oblivious to
// the ongoing HTTP requests.  The Ticker could be removed and periodic calls to sendRequest()
// could be made in loop(), resulting in the same asynchronous handling.
//
// For demo purposes, debug is turned on for handling of the first request.  These are the
// events that are being handled in AsyncHTTPRequest.  They all begin with Debug(nnn) where
// nnn is the elapsed time in milliseconds since the transaction was started.
//
//*************************************************************************************************************

// Important note: This code is intended to run with the ESPAsync_WiFiManager library version 1.4.1+.

#if !( defined(ESP8266) ||  defined(ESP32) )
	#error This code is intended to run on the ESP8266 or ESP32 platform! Please check your Tools->Board setting.
#endif

#define ESP_ASYNC_WIFIMANAGER_VERSION_MIN_TARGET          "ESPAsync_WiFiManager v1.15.0"
#define ESP_ASYNC_WIFIMANAGER_VERSION_MIN                 1015000

#define ASYNC_HTTP_REQUEST_GENERIC_VERSION_MIN_TARGET      "AsyncHTTPRequest_Generic v1.10.2"
#define ASYNC_HTTP_REQUEST_GENERIC_VERSION_MIN             1010002

// Uncomment for certain HTTP site to optimize
//#define NOT_SEND_HEADER_AFTER_CONNECTED        true

// Level from 0-4
#define ASYNC_HTTP_DEBUG_PORT     Serial

#define _ASYNC_HTTP_LOGLEVEL_     1
#define _WIFIMGR_LOGLEVEL_        1

// 300s = 5 minutes to not flooding, 60s in testing
#define HTTP_REQUEST_INTERVAL     60  //300

//Ported to ESP32
#ifdef ESP32
	#include <esp_wifi.h>
	#include <WiFi.h>
	#include <WiFiClient.h>

	// From v1.1.1
	#include <WiFiMulti.h>
	WiFiMulti wifiMulti;

	// LittleFS has higher priority than SPIFFS
	#define USE_LITTLEFS    true
	#define USE_SPIFFS      false

	#if USE_LITTLEFS
		// Use LittleFS
		#include "FS.h"

		// The library will be depreciated after being merged to future major Arduino esp32 core release 2.x
		// At that time, just remove this library inclusion
		#if ( defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 2) )
			#warning Using ESP32 Core 1.0.6 or 2.0.0+ and core LittleFS library
			// The library has been merged into esp32 core from release 1.0.6
			#include <LittleFS.h>             // https://github.com/espressif/arduino-esp32/tree/master/libraries/LittleFS

			//#define FileFS        LittleFS
			//#define FS_Name       "LittleFS"
			FS* filesystem =      &LittleFS;
			#define FileFS        LittleFS
		#else
			#warning Using ESP32 Core 1.0.5-. You must install LITTLEFS library
			// The library has been merged into esp32 core from release 1.0.6
			#include <LITTLEFS.h>             // https://github.com/lorol/LITTLEFS

			//#define FileFS        LITTLEFS
			FS* filesystem =      &LITTLEFS;
			#define FileFS        LITTLEFS
		#endif

		#define FS_Name       "LittleFS"
	#elif USE_SPIFFS
		#include <SPIFFS.h>
		FS* filesystem =      &SPIFFS;
		#define FileFS        SPIFFS
		#define FS_Name       "SPIFFS"
	#else
		// +Use FFat
		#include <FFat.h>
		FS* filesystem =      &FFat;
		#define FileFS        FFat
		#define FS_Name       "FFat"
	#endif
	//////

	#define LED_BUILTIN       2
	#define LED_ON            HIGH
	#define LED_OFF           LOW

#else

	#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
	//needed for library
	#include <DNSServer.h>

	// From v1.1.1
	#include <ESP8266WiFiMulti.h>
	ESP8266WiFiMulti wifiMulti;

	#define USE_LITTLEFS      true

	#if USE_LITTLEFS
		#include <LittleFS.h>
		FS* filesystem =      &LittleFS;
		#define FileFS        LittleFS
		#define FS_Name       "LittleFS"
	#else
		FS* filesystem =      &SPIFFS;
		#define FileFS        SPIFFS
		#define FS_Name       "SPIFFS"
	#endif
	//////

	#define ESP_getChipId()   (ESP.getChipId())

	#define LED_ON      LOW
	#define LED_OFF     HIGH
#endif

// SSID and PW for your Router
String Router_SSID;
String Router_Pass;

// From v1.1.1
// You only need to format the filesystem once
//#define FORMAT_FILESYSTEM       true
#define FORMAT_FILESYSTEM         false

#define MIN_AP_PASSWORD_SIZE    8

#define SSID_MAX_LEN            32
//From v1.0.10, WPA2 passwords can be up to 63 characters long.
#define PASS_MAX_LEN            64

typedef struct
{
	char wifi_ssid[SSID_MAX_LEN];
	char wifi_pw  [PASS_MAX_LEN];
}  WiFi_Credentials;

typedef struct
{
	String wifi_ssid;
	String wifi_pw;
}  WiFi_Credentials_String;

#define NUM_WIFI_CREDENTIALS      2

typedef struct
{
	WiFi_Credentials  WiFi_Creds [NUM_WIFI_CREDENTIALS];
} WM_Config;

WM_Config         WM_config;

#define  CONFIG_FILENAME              F("/wifi_cred.dat")
//////

// Indicates whether ESP has WiFi credentials saved from previous session, or double reset detected
bool initialConfig = false;

// Use false if you don't like to display Available Pages in Information Page of Config Portal
// Comment out or use true to display Available Pages in Information Page of Config Portal
// Must be placed before #include <ESP_WiFiManager.h>
#define USE_AVAILABLE_PAGES     true

// From v1.0.10 to permit disable/enable StaticIP configuration in Config Portal from sketch. Valid only if DHCP is used.
// You'll loose the feature of dynamically changing from DHCP to static IP, or vice versa
// You have to explicitly specify false to disable the feature.
//#define USE_STATIC_IP_CONFIG_IN_CP          false

// Use false to disable NTP config. Advisable when using Cellphone, Tablet to access Config Portal.
// See Issue 23: On Android phone ConfigPortal is unresponsive (https://github.com/khoih-prog/ESP_WiFiManager/issues/23)
#define USE_ESP_WIFIMANAGER_NTP     false

// Use true to enable CloudFlare NTP service. System can hang if you don't have Internet access while accessing CloudFlare
// See Issue #21: CloudFlare link in the default portal (https://github.com/khoih-prog/ESP_WiFiManager/issues/21)
#define USE_CLOUDFLARE_NTP          false

// New in v1.0.11
#define USING_CORS_FEATURE          true
//////

// Use USE_DHCP_IP == true for dynamic DHCP IP, false to use static IP which you have to change accordingly to your network
#if (defined(USE_STATIC_IP_CONFIG_IN_CP) && !USE_STATIC_IP_CONFIG_IN_CP)
	// Force DHCP to be true
	#if defined(USE_DHCP_IP)
		#undef USE_DHCP_IP
	#endif
	#define USE_DHCP_IP     true
#else
	// You can select DHCP or Static IP here
	#define USE_DHCP_IP     true
	//#define USE_DHCP_IP     false
#endif

#if ( USE_DHCP_IP || ( defined(USE_STATIC_IP_CONFIG_IN_CP) && !USE_STATIC_IP_CONFIG_IN_CP ) )
	// Use DHCP
	#warning Using DHCP IP
	IPAddress stationIP   = IPAddress(0, 0, 0, 0);
	IPAddress gatewayIP   = IPAddress(192, 168, 2, 1);
	IPAddress netMask     = IPAddress(255, 255, 255, 0);
#else
	// Use static IP
	#warning Using static IP

	#ifdef ESP32
		IPAddress stationIP   = IPAddress(192, 168, 2, 232);
	#else
		IPAddress stationIP   = IPAddress(192, 168, 2, 186);
	#endif

	IPAddress gatewayIP   = IPAddress(192, 168, 2, 1);
	IPAddress netMask     = IPAddress(255, 255, 255, 0);
#endif

#define USE_CONFIGURABLE_DNS      true

IPAddress dns1IP      = gatewayIP;
IPAddress dns2IP      = IPAddress(8, 8, 8, 8);

#define USE_CUSTOM_AP_IP          false

IPAddress APStaticIP  = IPAddress(192, 168, 100, 1);
IPAddress APStaticGW  = IPAddress(192, 168, 100, 1);
IPAddress APStaticSN  = IPAddress(255, 255, 255, 0);

#include <ESPAsync_WiFiManager.h>              //https://github.com/khoih-prog/ESPAsync_WiFiManager

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
//#include <ESPAsync_WiFiManager-Impl.h>         //https://github.com/khoih-prog/ESPAsync_WiFiManager

#define HTTP_PORT           80

//AsyncWebServer webServer(HTTP_PORT);
//DNSServer dnsServer;

#include <AsyncHTTPRequest_Generic.h>             // https://github.com/khoih-prog/AsyncHTTPRequest_Generic

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
//#include <AsyncHTTPRequest_Impl_Generic.h>        // https://github.com/khoih-prog/AsyncHTTPRequest_Generic

#include <Ticker.h>

AsyncHTTPRequest request;
Ticker ticker;

///////////////////////////////////////////
// New in v1.4.0
/******************************************
   // Defined in ESPAsync_WiFiManager.h
  typedef struct
  {
  IPAddress _ap_static_ip;
  IPAddress _ap_static_gw;
  IPAddress _ap_static_sn;

  }  WiFi_AP_IPConfig;

  typedef struct
  {
  IPAddress _sta_static_ip;
  IPAddress _sta_static_gw;
  IPAddress _sta_static_sn;
  #if USE_CONFIGURABLE_DNS
  IPAddress _sta_static_dns1;
  IPAddress _sta_static_dns2;
  #endif
  }  WiFi_STA_IPConfig;
******************************************/

WiFi_AP_IPConfig  WM_AP_IPconfig;
WiFi_STA_IPConfig WM_STA_IPconfig;

void initAPIPConfigStruct(WiFi_AP_IPConfig &in_WM_AP_IPconfig)
{
	in_WM_AP_IPconfig._ap_static_ip   = APStaticIP;
	in_WM_AP_IPconfig._ap_static_gw   = APStaticGW;
	in_WM_AP_IPconfig._ap_static_sn   = APStaticSN;
}

void initSTAIPConfigStruct(WiFi_STA_IPConfig &in_WM_STA_IPconfig)
{
	in_WM_STA_IPconfig._sta_static_ip   = stationIP;
	in_WM_STA_IPconfig._sta_static_gw   = gatewayIP;
	in_WM_STA_IPconfig._sta_static_sn   = netMask;
#if USE_CONFIGURABLE_DNS
	in_WM_STA_IPconfig._sta_static_dns1 = dns1IP;
	in_WM_STA_IPconfig._sta_static_dns2 = dns2IP;
#endif
}

void displayIPConfigStruct(WiFi_STA_IPConfig in_WM_STA_IPconfig)
{
	LOGERROR3(F("stationIP ="), in_WM_STA_IPconfig._sta_static_ip, F(", gatewayIP ="), in_WM_STA_IPconfig._sta_static_gw);
	LOGERROR1(F("netMask ="), in_WM_STA_IPconfig._sta_static_sn);
#if USE_CONFIGURABLE_DNS
	LOGERROR3(F("dns1IP ="), in_WM_STA_IPconfig._sta_static_dns1, F(", dns2IP ="), in_WM_STA_IPconfig._sta_static_dns2);
#endif
}

void configWiFi(WiFi_STA_IPConfig in_WM_STA_IPconfig)
{
#if USE_CONFIGURABLE_DNS
	// Set static IP, Gateway, Subnetmask, DNS1 and DNS2. New in v1.0.5
	WiFi.config(in_WM_STA_IPconfig._sta_static_ip, in_WM_STA_IPconfig._sta_static_gw, in_WM_STA_IPconfig._sta_static_sn, in_WM_STA_IPconfig._sta_static_dns1, in_WM_STA_IPconfig._sta_static_dns2);
#else
	// Set static IP, Gateway, Subnetmask, Use auto DNS1 and DNS2.
	WiFi.config(in_WM_STA_IPconfig._sta_static_ip, in_WM_STA_IPconfig._sta_static_gw, in_WM_STA_IPconfig._sta_static_sn);
#endif
}

///////////////////////////////////////////

uint8_t connectMultiWiFi()
{
#if ESP32
	// For ESP32, this better be 0 to shorten the connect time.
	// For ESP32-S2, must be > 500
#if ( ARDUINO_ESP32S2_DEV || ARDUINO_FEATHERS2 || ARDUINO_PROS2 || ARDUINO_MICROS2 )
#define WIFI_MULTI_1ST_CONNECT_WAITING_MS           500L
#else
	// For ESP32 core v1.0.6, must be >= 500
#define WIFI_MULTI_1ST_CONNECT_WAITING_MS           800L
#endif
#else
	// For ESP8266, this better be 2200 to enable connect the 1st time
#define WIFI_MULTI_1ST_CONNECT_WAITING_MS             2200L
#endif

#define WIFI_MULTI_CONNECT_WAITING_MS                   100L

	uint8_t status;

	LOGERROR(F("ConnectMultiWiFi with :"));

	if ( (Router_SSID != "") && (Router_Pass != "") )
	{
		LOGERROR3(F("* Flash-stored Router_SSID = "), Router_SSID, F(", Router_Pass = "), Router_Pass );
	}

	for (uint8_t i = 0; i < NUM_WIFI_CREDENTIALS; i++)
	{
		// Don't permit NULL SSID and password len < MIN_AP_PASSWORD_SIZE (8)
		if ( (String(WM_config.WiFi_Creds[i].wifi_ssid) != "") && (strlen(WM_config.WiFi_Creds[i].wifi_pw) >= MIN_AP_PASSWORD_SIZE) )
		{
			LOGERROR3(F("* Additional SSID = "), WM_config.WiFi_Creds[i].wifi_ssid, F(", PW = "), WM_config.WiFi_Creds[i].wifi_pw );
		}
	}

	LOGERROR(F("Connecting MultiWifi..."));

	WiFi.mode(WIFI_STA);

#if !USE_DHCP_IP
	// New in v1.4.0
	configWiFi(WM_STA_IPconfig);
	//////
#endif

	int i = 0;
	status = wifiMulti.run();

	delay(WIFI_MULTI_1ST_CONNECT_WAITING_MS);

	while ( ( i++ < 20 ) && ( status != WL_CONNECTED ) )
	{
		status = wifiMulti.run();

		if ( status == WL_CONNECTED )
			break;
		else
			delay(WIFI_MULTI_CONNECT_WAITING_MS);
	}

	if ( status == WL_CONNECTED )
	{
		LOGERROR1(F("WiFi connected after time: "), i);
		LOGERROR3(F("SSID:"), WiFi.SSID(), F(",RSSI="), WiFi.RSSI());
		LOGERROR3(F("Channel:"), WiFi.channel(), F(",IP address:"), WiFi.localIP() );
	}
	else
		LOGERROR(F("WiFi not connected"));

	return status;
}

void heartBeatPrint()
{
	static int num = 1;

	if (WiFi.status() == WL_CONNECTED)
		Serial.print(F("H"));        // H means connected to WiFi
	else
		Serial.print(F("F"));        // F means not connected to WiFi

	if (num == 80)
	{
		Serial.println();
		num = 1;
	}
	else if (num++ % 10 == 0)
	{
		Serial.print(F(" "));
	}
}

void check_WiFi()
{
	if ( (WiFi.status() != WL_CONNECTED) )
	{
		Serial.println(F("\nWiFi lost. Call connectMultiWiFi in loop"));
		connectMultiWiFi();
	}
}

void check_status()
{
	static ulong checkstatus_timeout  = 0;
	static ulong checkwifi_timeout    = 0;

	static ulong current_millis;

#define WIFICHECK_INTERVAL    1000L
#define HEARTBEAT_INTERVAL    10000L

	current_millis = millis();

	// Check WiFi every WIFICHECK_INTERVAL (1) seconds.
	if ((current_millis > checkwifi_timeout) || (checkwifi_timeout == 0))
	{
		check_WiFi();
		checkwifi_timeout = current_millis + WIFICHECK_INTERVAL;
	}

	// Print hearbeat every HEARTBEAT_INTERVAL (10) seconds.
	if ((current_millis > checkstatus_timeout) || (checkstatus_timeout == 0))
	{
		heartBeatPrint();
		checkstatus_timeout = current_millis + HEARTBEAT_INTERVAL;
	}
}

void loadConfigData()
{
	File file = FileFS.open(CONFIG_FILENAME, "r");
	LOGERROR(F("LoadWiFiCfgFile "));

	memset(&WM_config,       0, sizeof(WM_config));

	// New in v1.4.0
	//memset(&WM_STA_IPconfig, 0, sizeof(WM_STA_IPconfig));
	// New in v1.8.2
	initSTAIPConfigStruct(WM_STA_IPconfig);
	//////

	if (file)
	{
		file.readBytes((char *) &WM_config,   sizeof(WM_config));

		// New in v1.4.0
		file.readBytes((char *) &WM_STA_IPconfig, sizeof(WM_STA_IPconfig));
		//////

		file.close();
		LOGERROR(F("OK"));

		// New in v1.4.0
		displayIPConfigStruct(WM_STA_IPconfig);
		//////
	}
	else
	{
		LOGERROR(F("failed"));
	}
}

void saveConfigData()
{
	File file = FileFS.open(CONFIG_FILENAME, "w");
	LOGERROR(F("SaveWiFiCfgFile "));

	if (file)
	{
		file.write((uint8_t*) &WM_config,   sizeof(WM_config));

		// New in v1.4.0
		file.write((uint8_t*) &WM_STA_IPconfig, sizeof(WM_STA_IPconfig));
		//////

		file.close();
		LOGERROR(F("OK"));
	}
	else
	{
		LOGERROR(F("failed"));
	}
}

void sendRequest()
{
	static bool requestOpenResult;

	if (request.readyState() == readyStateUnsent || request.readyState() == readyStateDone)
	{
		//requestOpenResult = request.open("GET", "http://worldtimeapi.org/api/timezone/Europe/London.txt");
		requestOpenResult = request.open("GET", "http://worldtimeapi.org/api/timezone/America/Toronto.txt");

		if (requestOpenResult)
		{
			// Only send() if open() returns true, or crash
			request.send();
		}
		else
		{
			Serial.println(F("Can't send bad request"));
		}
	}
	else
	{
		Serial.println(F("Can't send request"));
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
	// put your setup code here, to run once:
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

	delay(200);

	Serial.print(F("\nStarting AsyncHTTPRequest_ESP_WiFiManager using "));
	Serial.print(FS_Name);
	Serial.print(F(" on "));
	Serial.println(ARDUINO_BOARD);
	Serial.println(ESP_ASYNC_WIFIMANAGER_VERSION);
	Serial.println(ASYNC_HTTP_REQUEST_GENERIC_VERSION);

#if defined(ESP_ASYNC_WIFIMANAGER_VERSION_INT)

	if (ESP_ASYNC_WIFIMANAGER_VERSION_INT < ESP_ASYNC_WIFIMANAGER_VERSION_MIN)
	{
		Serial.print(F("Warning. Must use this example on Version later than : "));
		Serial.println(ESP_ASYNC_WIFIMANAGER_VERSION_MIN_TARGET);
	}

#endif

#if defined(ASYNC_HTTP_REQUEST_GENERIC_VERSION_MIN)

	if (ASYNC_HTTP_REQUEST_GENERIC_VERSION_INT < ASYNC_HTTP_REQUEST_GENERIC_VERSION_MIN)
	{
		Serial.print(F("Warning. Must use this example on Version equal or later than : "));
		Serial.println(ASYNC_HTTP_REQUEST_GENERIC_VERSION_MIN_TARGET);
	}

#endif

	if (FORMAT_FILESYSTEM)
		FileFS.format();

	// Format FileFS if not yet
#ifdef ESP32

	if (!FileFS.begin(true))
#else
	if (!FileFS.begin())
#endif
	{
		Serial.print(FS_Name);
		Serial.println(F(" failed! AutoFormatting."));

#ifdef ESP8266
		FileFS.format();
#endif
	}

	unsigned long startedAt = millis();

	// New in v1.4.0
	initAPIPConfigStruct(WM_AP_IPconfig);
	initSTAIPConfigStruct(WM_STA_IPconfig);
	//////

	//Local intialization. Once its business is done, there is no need to keep it around
	// Use this to default DHCP hostname to ESP8266-XXXXXX or ESP32-XXXXXX
	//ESPAsync_WiFiManager ESPAsync_wifiManager(&webServer, &dnsServer);
	// Use this to personalize DHCP hostname (RFC952 conformed)
	AsyncWebServer webServer(HTTP_PORT);

	//ESPAsync_WiFiManager ESPAsync_wifiManager(&webServer, &dnsServer, "AutoConnectAP");
#if ( USING_ESP32_S2 || USING_ESP32_C3 )
	ESPAsync_WiFiManager ESPAsync_wifiManager(&webServer, NULL, "AutoConnectAP");
#else
	AsyncDNSServer dnsServer;

	ESPAsync_WiFiManager ESPAsync_wifiManager(&webServer, &dnsServer, "AutoConnectAP");
#endif

	//ESPAsync_wifiManager.setDebugOutput(true);

	//reset settings - for testing
	//ESPAsync_wifiManager.resetSettings();

#if USE_CUSTOM_AP_IP
	//set custom ip for portal
	// New in v1.4.0
	ESPAsync_wifiManager.setAPStaticIPConfig(WM_AP_IPconfig);
	//////
#endif

	ESPAsync_wifiManager.setMinimumSignalQuality(-1);

	// From v1.0.10 only
	// Set config portal channel, default = 1. Use 0 => random channel from 1-13
	ESPAsync_wifiManager.setConfigPortalChannel(0);
	//////

#if !USE_DHCP_IP
	// Set (static IP, Gateway, Subnetmask, DNS1 and DNS2) or (IP, Gateway, Subnetmask). New in v1.0.5
	// New in v1.4.0
	ESPAsync_wifiManager.setSTAStaticIPConfig(WM_STA_IPconfig);
	//////
#endif

	// New from v1.1.1
#if USING_CORS_FEATURE
	ESPAsync_wifiManager.setCORSHeader("Your Access-Control-Allow-Origin");
#endif

	// We can't use WiFi.SSID() in ESP32 as it's only valid after connected.
	// SSID and Password stored in ESP32 wifi_ap_record_t and wifi_config_t are also cleared in reboot
	// Have to create a new function to store in EEPROM/SPIFFS for this purpose
	Router_SSID = ESPAsync_wifiManager.WiFi_SSID();
	Router_Pass = ESPAsync_wifiManager.WiFi_Pass();

	//Remove this line if you do not want to see WiFi password printed
	Serial.print(F("Stored: SSID = "));
	Serial.print(Router_SSID);
	Serial.print(F(", Pass = "));
	Serial.println(Router_Pass);

	if (Router_SSID != "")
	{
		ESPAsync_wifiManager.setConfigPortalTimeout(120); //If no access point name has been previously entered disable timeout.
		Serial.println(F("Got stored Credentials. Timeout 120s"));
	}
	else
	{
		Serial.println(F("No stored Credentials. No timeout"));
	}

	String chipID = String(ESP_getChipId(), HEX);
	chipID.toUpperCase();

	// SSID and PW for Config Portal
	String AP_SSID = "ESP_" + chipID + "_AutoConnectAP";
	String AP_PASS = "MyESP_" + chipID;

	// From v1.1.0, Don't permit NULL password
	if ( (Router_SSID == "") || (Router_Pass == "") )
	{
		Serial.println(F("We haven't got any access point credentials, so get them now"));

		initialConfig = true;

		Serial.print(F("Starting configuration portal @ "));

#if USE_CUSTOM_AP_IP
		Serial.print(APStaticIP);
#else
		Serial.print(F("192.168.4.1"));
#endif

		Serial.print(F(", SSID = "));
		Serial.print(AP_SSID);
		Serial.print(F(", PASS = "));
		Serial.println(AP_PASS);

		// Starts an access point
		//if (!ESPAsync_wifiManager.startConfigPortal((const char *) ssid.c_str(), password))
		if ( !ESPAsync_wifiManager.startConfigPortal(AP_SSID.c_str(), AP_PASS.c_str()) )
			Serial.println(F("Not connected to WiFi but continuing anyway."));
		else
			Serial.println(F("WiFi connected...yeey :)"));

		// Stored  for later usage, from v1.1.0, but clear first
		memset(&WM_config, 0, sizeof(WM_config));

		for (uint8_t i = 0; i < NUM_WIFI_CREDENTIALS; i++)
		{
			String tempSSID = ESPAsync_wifiManager.getSSID(i);
			String tempPW   = ESPAsync_wifiManager.getPW(i);

			if (strlen(tempSSID.c_str()) < sizeof(WM_config.WiFi_Creds[i].wifi_ssid) - 1)
				strcpy(WM_config.WiFi_Creds[i].wifi_ssid, tempSSID.c_str());
			else
				strncpy(WM_config.WiFi_Creds[i].wifi_ssid, tempSSID.c_str(), sizeof(WM_config.WiFi_Creds[i].wifi_ssid) - 1);

			if (strlen(tempPW.c_str()) < sizeof(WM_config.WiFi_Creds[i].wifi_pw) - 1)
				strcpy(WM_config.WiFi_Creds[i].wifi_pw, tempPW.c_str());
			else
				strncpy(WM_config.WiFi_Creds[i].wifi_pw, tempPW.c_str(), sizeof(WM_config.WiFi_Creds[i].wifi_pw) - 1);

			// Don't permit NULL SSID and password len < MIN_AP_PASSWORD_SIZE (8)
			if ( (String(WM_config.WiFi_Creds[i].wifi_ssid) != "") && (strlen(WM_config.WiFi_Creds[i].wifi_pw) >= MIN_AP_PASSWORD_SIZE) )
			{
				LOGERROR3(F("* Add SSID = "), WM_config.WiFi_Creds[i].wifi_ssid, F(", PW = "), WM_config.WiFi_Creds[i].wifi_pw );
				wifiMulti.addAP(WM_config.WiFi_Creds[i].wifi_ssid, WM_config.WiFi_Creds[i].wifi_pw);
			}
		}

		// New in v1.4.0
		ESPAsync_wifiManager.getSTAStaticIPConfig(WM_STA_IPconfig);
		displayIPConfigStruct(WM_STA_IPconfig);
		//////

		saveConfigData();
	}
	else
	{
		wifiMulti.addAP(Router_SSID.c_str(), Router_Pass.c_str());
	}

	startedAt = millis();

	if (!initialConfig)
	{
		// Load stored data, the addAP ready for MultiWiFi reconnection
		loadConfigData();

		for (uint8_t i = 0; i < NUM_WIFI_CREDENTIALS; i++)
		{
			// Don't permit NULL SSID and password len < MIN_AP_PASSWORD_SIZE (8)
			if ( (String(WM_config.WiFi_Creds[i].wifi_ssid) != "") && (strlen(WM_config.WiFi_Creds[i].wifi_pw) >= MIN_AP_PASSWORD_SIZE) )
			{
				LOGERROR3(F("* Add SSID = "), WM_config.WiFi_Creds[i].wifi_ssid, F(", PW = "), WM_config.WiFi_Creds[i].wifi_pw );
				wifiMulti.addAP(WM_config.WiFi_Creds[i].wifi_ssid, WM_config.WiFi_Creds[i].wifi_pw);
			}
		}

		if ( WiFi.status() != WL_CONNECTED )
		{
			Serial.println(F("ConnectMultiWiFi in setup"));

			connectMultiWiFi();
		}
	}

	Serial.print(F("After waiting "));
	Serial.print((float) (millis() - startedAt) / 1000L);
	Serial.print(F(" secs more in setup(), connection result is "));

	if (WiFi.status() == WL_CONNECTED)
	{
		Serial.print(F("connected. Local IP: "));
		Serial.println(WiFi.localIP());
	}
	else
		Serial.println(ESPAsync_wifiManager.getStatus(WiFi.status()));

	request.setDebug(false);

	request.onReadyStateChange(requestCB);
	ticker.attach(HTTP_REQUEST_INTERVAL, sendRequest);

	// Send first request now
	sendRequest();
}

void loop()
{
	// put your main code here, to run repeatedly
	check_status();
}
