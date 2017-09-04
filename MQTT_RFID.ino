/* wiring the SPI to ESP8266 (weemos D1 mini)
RST     = 5
SDA(SS) = 4 
MOSI    = 13
MISO    = 12
SCK     = 14
GND     = GND
3.3V    = 3.3V
*/

#include "SPI.h"
#include "MFRC522.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient wifiClient;

#define SS_PIN 4
#define RST_PIN 5

char msg[50];

// Network data.
const char* ssid = "ENTER AP";
const char* password = "ENTER PASS";
const char* server = "ENTER SERVER ADDRESS";
char* topic = "/rfid";
String strID="";


MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

void callback(char* topic, byte* message, unsigned int length){}
PubSubClient client(server, 1883, callback, wifiClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("RFID_Lab")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

String rfidCard(){
  strID="";
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
  return "";
  for (byte i = 0; i < 4; i++) {
    strID += (rfid.uid.uidByte[i] < 0x10 ? "0" : "") + String(rfid.uid.uidByte[i], HEX) + (i!=3 ? ":" : "");
  }
  strID.toUpperCase();
return strID;
}


void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  setup_wifi(); 
  client.setServer(server, 1883);
}

void loop() {
  delay(800);
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if (client.connected()){
    String rfidCardValue = rfidCard();
    if (rfidCardValue != ""){
      Serial.println(rfidCardValue);
      if (client.publish(topic, (char*) rfidCardValue.c_str())) {
      Serial.println("Publish ok");
      }   
    }
  }
}
