#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "your ssid";
const char* password = "your wifi password";

// MQTT Broker
const char* mqtt_server = "your local IP";
const int mqtt_port = 1883;
const char* topic = "esp32/beacons";

WiFiClient espClient;
PubSubClient client(espClient);

BLEScan* pBLEScan;

// MAC addresses of 6 beacons
BLEAddress beacon1("--ENTER THE MAC ADDRESS OF BEACON--");
BLEAddress beacon2("--ENTER THE MAC ADDRESS OF BEACON--");
BLEAddress beacon3("--ENTER THE MAC ADDRESS OF BEACON--");
BLEAddress beacon4("--ENTER THE MAC ADDRESS OF BEACON--");
BLEAddress beacon5("--ENTER THE MAC ADDRESS OF BEACON--");
BLEAddress beacon6("--ENTER THE MAC ADDRESS OF BEACON--");

int rssi1 = -100;
int rssi2 = -100;
int rssi3 = -100;
int rssi4 = -100;
int rssi5 = -100;
int rssi6 = -100;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    BLEAddress addr = advertisedDevice.getAddress();

    if (addr.equals(beacon1)) {
      rssi1 = advertisedDevice.getRSSI();
    } else if (addr.equals(beacon2)) {
      rssi2 = advertisedDevice.getRSSI();
    } else if (addr.equals(beacon3)) {
      rssi3 = advertisedDevice.getRSSI();
    } else if (addr.equals(beacon4)) {
      rssi4 = advertisedDevice.getRSSI();
    } else if (addr.equals(beacon5)) {
      rssi5 = advertisedDevice.getRSSI();
    } else if (addr.equals(beacon6)) {
      rssi6 = advertisedDevice.getRSSI();
    }
  }
};

void setup_wifi() {
  delay(100);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
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
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(50);
  pBLEScan->setWindow(49);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  rssi1 = -100;
  rssi2 = -100;
  rssi3 = -100;
  rssi4 = -100;
  rssi5 = -100;
  rssi6 = -100;

  pBLEScan->start(2, false);

  // Create JSON string
  String json = "{";
  json += "\"beacon1\":" + String(rssi1) + ",";
  json += "\"beacon2\":" + String(rssi2) + ",";
  json += "\"beacon3\":" + String(rssi3) + ",";
  json += "\"beacon4\":" + String(rssi4) + ",";
  json += "\"beacon5\":" + String(rssi5) + ",";
  json += "\"beacon6\":" + String(rssi6);
  json += "}";

  client.publish(topic, json.c_str());
  Serial.println("Published RSSI data: " + json);

  delay(2000);
}
