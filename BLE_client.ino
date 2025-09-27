#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>

BLEScan* pBLEScan;
BLEAddress beaconAddress("--ENTER THE MAC ADDRESS OF BEACON--"); // Replace with your beacon's MAC address

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getAddress().equals(beaconAddress)) {
      Serial.print("Found Beacon with RSSI: ");
      Serial.println(advertisedDevice.getRSSI());
    }
  }
};

void setup() {
  Serial.begin(115200);
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  
  
  pBLEScan->setInterval(50);  // Reduce time between scans (default ~1000ms)
  pBLEScan->setWindow(49);    // Increase scan duration (default ~50ms)
}

void loop() {
  pBLEScan->start(2, false);  // Scan every 2 seconds instead of 5
}
