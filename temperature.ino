/* =============================================================================

REQUIRES the following Arduino libraries:
- DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
- Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
- Adafruit SSD1306, https://github.com/adafruit/Adafruit_SSD1306
- Adafruit GFX Library, https://github.com/adafruit/Adafruit-GFX-Library

============================================================================= */

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <SPI.h>
#include <Wire.h>

/* -------------------------------------------------------------------------- */

#define DHT_PIN 4
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

/* -------------------------------------------------------------------------- */

#define SCREEN_ADDRESS 0x3C
#define SCREEN_HEIGHT 64
#define SCREEN_RESET -1
#define SCREEN_WIDTH 128

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET);

/* -------------------------------------------------------------------------- */

const int progressBarHeight = 1;

/**
 * The number of milliseconds between updates of the temperature and humidity.
 * The value should NOT be less than 2000 for the DHT22 sensor.
 */
const int readingUpdateInterval = 30000;

/**
 * Given the width of the screen and the reading update interval we can
 * calculate the number of milliseconds each pixel represents. This becomes the
 * delay for the loop so the progress bar will update smoothly.
 */
const int displayRefreshRate = readingUpdateInterval / SCREEN_WIDTH;

/**
 * Based on the desired interval between sensor readings and how long a loop
 * will wait for the next iteration we can determine the number of iterations we
 * need to skip before refreshing the sensor readings.
 */
const int maxTickCount = readingUpdateInterval / displayRefreshRate;

int tickCount = 0;

/* ========================================================================== */

void setup() {
	Serial.begin(115200);

	if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
		Serial.println(F("Unable to initialize screen."));
		// Unable to initialize the screen, stop executing the program.
		for(;;);
	}

	display.setTextColor(SSD1306_WHITE);
	dht.begin();
}

void loop() {
	if (tickCount == 0) {
		updateReadings();
	} else {
		updateProgress();
	}

	// Increase the tick count by one. When the tick count reaches the max
	// tick count, tick count will automatically reset to 0.
	tickCount = (tickCount + 1) % maxTickCount;

	delay(displayRefreshRate);
}

/* ========================================================================== */

void updateProgress() {
	const float percentageDone = tickCount / (float)maxTickCount;
	const int widthDone = display.width() * percentageDone;
	const int widthToDo = display.width() - widthDone;
	const int startRow = display.height() - progressBarHeight;

	// This will draw a white bar representing how long to go till the sensor
	// readings will be updated.
	display.drawRect(0, startRow, widthToDo, progressBarHeight, SSD1306_WHITE);
	// The will draw a black bar starting at the end of the white bar all the
	// way to the end of the display. It represents the time already passed
	// since the last sensor readings update.
	display.drawRect(widthToDo, startRow, widthDone, progressBarHeight, SSD1306_BLACK);

	display.display();
}

void updateReadings() {
	float humidity = dht.readHumidity();
	float temperature = dht.readTemperature();

	display.clearDisplay();
	updateTemperatureReading(temperature);
	updateHumidityReading(humidity);
	display.display();
}

void updateHumidityReading(float humidity) {
	// The humidity reading will be displayed in the second half of the screen,
	// the cursor will have to be moved to the proper position.
	display.setCursor(0,32);
	// Print the heading for the reading
	display.setTextSize(1);
	display.print(F("Humidity"));

	// Move the cursor a little lower than a println would've done so there is
	// a little more vertical space between the heading and the value.
	display.setCursor(0,42);
	display.setTextSize(2);
	if (isnan(humidity)) {
		display.print(F("--.-%"));
	} else {
		display.print(humidity, 1);
		display.print(F("%"));
	}
}

void updateTemperatureReading(float temperature) {
	// Print the heading for the reading
	display.setTextSize(1);
	display.setCursor(0,0);
	display.print(F("Temperature"));

	// Move the cursor a little lower than a println would've done so there is
	// a little more vertical space between the heading and the value.
	display.setCursor(0,10);
	display.setTextSize(2);
	if (isnan(temperature)) {
		display.print(F("--.-"));
	} else {
		display.print(temperature, 1);
	}

	// This will draw a small degrees symbol, by drawing it at text size 1 it
	// looks like superscript compared to the larger text around it.
	display.setTextSize(1);
	display.write(167);

	// Set the text size again to 2 so the Celsius symbol has the same text size
	// as the temperature.
	display.setTextSize(2);
	display.print(F("C"));
}
