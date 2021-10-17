#include <Arduino.h>
#include <WiFi.h>
#include "time.h"
#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>
#include "Adafruit_SH1106.h"

const char* ssid      = "yourssid";
const char* password  = "yourpassword";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;	// Time zone of IST in seconds
const long 	daylightOffset_sec = 0;


Adafruit_SH1106 display(SDA, SCL);  // Using default SDA and SCL pins of ESP32.

void displayLocalTime() {
	struct tm timeinfo;
	if (!getLocalTime(&timeinfo)) {
		Serial.println("Failed to obtain time");

        display.clearDisplay();
        display.setCursor(0,28);
        display.println("Failed to obtain time");
        display.display();
		return;
	}

	display.clearDisplay();
	display.setCursor(0,28);
	display.println(&timeinfo, "%A, %B, %d, %Y, %H: %M: %S");
	display.display();
	Serial.println(&timeinfo, "%A, %B, %d, %Y, %H: %M: %S");
}

void setup() {
	Serial.begin(115200);

	display.begin(SH1106_SWITCHCAPVCC, 0x3C);

	display.clearDisplay();

	display.setTextSize(1);
	display.setTextColor(WHITE);

	// Connect to WiFi
	Serial.printf("Connecting to %s", ssid);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("  CONNECTED");

	// init and get the time
	configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
	displayLocalTime();

	// disconnect the WiFi as it's no longer required
	WiFi.disconnect(true);
	WiFi.mode(WIFI_OFF);
}

void loop() {
	delay(900);
	displayLocalTime();
}
