#include <WiFi.h> // Load WiFi library
#include "wifi_module.hpp"


const String ssid = "";
const String password = "";


// Scan for local wifi networks - terminal prints out list ordered by signal strength
#define MAX_NETWORKS 10

WiFiScanResult WiFiScan(void) {
  static WiFiNetwork topNetworks[MAX_NETWORKS];
  int suitableNetworks = 0;

  // Clear the array before each scan
  memset(topNetworks, 0, sizeof(topNetworks));

  // Remember total number of found networks, regardless of suitability
  int totalNetworks = WiFi.scanNetworks();

  // Handle the event of negative or 0 networks being detected
  if (totalNetworks <= 0) {
    if (totalNetworks < 0) {
      Serial.println("Error scanning networks");
    }
    else {
      Serial.println("No networks found");
    }
    WiFiScanResult errorResult;
    errorResult.networks = nullptr;
    errorResult.numNetworks = 0;
    return errorResult;
  }

  // Handle any real number of networks being detected
  else {
    Serial.print("Networks found: ");
    Serial.println(totalNetworks);

    // Sort networks by signal strength (RSSI)
    for (int i = 0; i < min(MAX_NETWORKS, totalNetworks); i++) {
      String SSID = WiFi.SSID(i);
      int RSSI = WiFi.RSSI(i);
      
      // Ignore networks with poor signal strength (RSSI) + print how many networks were ignored
      if (RSSI < -80) {
        Serial.print("+");
        Serial.print(String(totalNetworks - i));
        Serial.println(" networks ignored due to poor signal strength");
        break;
      }

      // Print each suitable networks SSID along with RSSI + write the SSID to the corresponding array index of topSSIDs
      else {
        Serial.print(i + 1);
        Serial.print(" - SSID: ");
        Serial.print(SSID);
        Serial.print(" | Strength: ");
        Serial.println(RSSI);

        topNetworks[i].SSID = SSID;
        topNetworks[i].RSSI = RSSI;

        suitableNetworks++;
      }
    }
  }

  Serial.println("Top Networks:");
  for (int i = 0; i < suitableNetworks; i++) {
    Serial.print("SSID: ");
    Serial.print(topNetworks[i].SSID);
    Serial.print(" | RSSI: ");
    Serial.println(topNetworks[i].RSSI);
  }

  WiFiScanResult result;
  result.networks = topNetworks;
  result.numNetworks = suitableNetworks;
  // Print contents of array topSSIDs for troubleshooting + return array topSSIDs
  return result;
}


void WiFiConnect(void){
  Serial.println("WifiSetup starting...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Attempting connection...");
  }
  if (WiFi.status() == WL_CONNECTED) {
    //reserved for cred save after successful connection
  }
  Serial.print("Connected successfully to network: ");
  Serial.println(ssid);
}


void WiFiMain() {
  WiFiScan();
}