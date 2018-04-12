# MQTT_RFID_esp8266
Like stated in the .ino file this project uses weemos D1 mini board based on esp8266 controller.
To program ESP8266 from Arduino IDE and to use the code best option is to use this setup:
https://github.com/esp8266/Arduino

Also, as can be seen from the .ino file, this code is intended for RFID readers based on MFRC522 chips.
This project uses MQTT to send/receive data (PubSubClient.h library) and needs to have an MQTT broker running somewhere accessible (laptop, router, raspberry pi or a server).
Wiring the RFID's SPI to ESP8266 (weemos D1 mini)
RST     = 5
SDA(SS) = 4 
MOSI    = 13
MISO    = 12
SCK     = 14
GND     = GND
3.3V    = 3.3V
