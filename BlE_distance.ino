#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <math.h>  // for pow()

BLEScan* pBLEScan;
BLEAddress beaconAddress("51:00:24:10:00:40"); // Replace with your beacon's MAC

float calculateDistance(int rssi) {
  int txPower = -64;     // Assumed RSSI at 1 meter
  float n = 2.0;         // Environmental factor
  return pow(10.0, ((txPower - rssi) / (10 * n)));
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getAddress().equals(beaconAddress)) {
      int rssi = advertisedDevice.getRSSI();
      float distance = calculateDistance(rssi);
      Serial.print("Found Beacon with RSSI: ");
      Serial.print(rssi);
      Serial.print(" | Estimated Distance: ");
      Serial.print(distance, 2);  // 2 decimal places
      Serial.println(" meters");
    }
  }
};

void setup() {
  Serial.begin(115200);
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(50);
  pBLEScan->setWindow(49);
}

void loop() {
  pBLEScan->start(4, false);  // Scan for 2 seconds
}
