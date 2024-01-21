#ifndef WiFiMAIN
#define WIFIMAIN

void WiFiMain(void);

// Structure to hold information about suitable Wi-Fi networks
struct WiFiNetwork {
  String SSID;
  int RSSI;
};

// Structure to hold the result of a Wi-Fi scan
struct WiFiScanResult {
  WiFiNetwork* networks;  // Array of suitable Wi-Fi networks
  int numNetworks;        // Number of entries in the array networks
};

WiFiScanResult WiFiScan(void);

#endif //WIFIMAIN