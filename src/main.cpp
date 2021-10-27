#include <Arduino.h>
#include <WiFi.h>
#include "time.h"
#include <U8g2lib.h>
#include <sstream>

// OLED constructor
// Please check on the link below and according to the display driver on your OLED change the line below:
// https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, SCL, SDA); 

// change the SSID and password according to your WiFi network
const char* ssid      = "yourssid";
const char* password  = "yourpassword";

const char* ntpServer = "pool.ntp.org";
// Change the data below according to your timezone and daylight offset (in seconds)
const long  gmtOffset_sec = 19800;	// Time zone of IST in seconds
const long 	daylightOffset_sec = 0;

/*
	NOTE: I have really low C++ experience so I can't really find a way to concanate strings with integers
	as I have to convert integers into strings and then convert them into char* as the drawStr method
	expects it. So I don't think the way I implemented it is an efficient solution but it works.
*/
std::string curr_time;

std::string IntToString(int a)
{
    std::ostringstream temp;
    temp << a;
    return temp.str();
}

void displayLocalTime() {
	struct tm timeinfo;
	if (!getLocalTime(&timeinfo)) {
		Serial.println("Failed to obtain time");
		return;
	}

	curr_time = IntToString(timeinfo.tm_hour) + ": " + IntToString(timeinfo.tm_min) + ": " + IntToString(timeinfo.tm_sec);
	u8g2.clearBuffer();
	u8g2.setFont(u8g2_font_ncenB14_tr);
	u8g2.drawStr(0, 18, "Time:");
	u8g2.drawStr(10, 50, curr_time.c_str());
	u8g2.sendBuffer();

	Serial.println(curr_time.c_str());
}

void setup() {
	Serial.begin(115200);

	u8g2.begin();

	// Connect to WiFi
	Serial.printf("Connecting to %s", ssid);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("  CONNECTED");

	// get the time from internet
	configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
	displayLocalTime();

	// disconnect the WiFi as it's no longer required
	WiFi.disconnect(true);
	WiFi.mode(WIFI_OFF);
}

void loop() {
	displayLocalTime();
	delay(900);
}
