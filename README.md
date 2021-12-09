# Thermometer and humidity display
This project displays the temperature and humidity readings from a DHT22 sensor
on a 128x64 OLED display. What makes this project different from similar
projects is a progress bar at the bottom of the display showing you how long
until the readings are refreshed.

## Used hardware
- 1x ESP32 WROOM 4Mb Devkit V1
- 1x DHT-22 sensor
- 1x OLED display, 128 by 64 pixels, I2C compatible

## Required libraries
The following libraries are required to run the project:
- [DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library)
- [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor)
- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)

## Setup
The components are wired up as follows:

### DHT-22 sensor
The DHT-22 sensor has its data pin connected to pin 4 on the ESP32 board.

### OLED display
The SLC pin of the display is connected to pin 21 on the ESP32 board. The SDA pin of the display is connected to pin 22 on the ESP32 board.

# License
This project is released under the [MIT](https://choosealicense.com/licenses/mit/) license.