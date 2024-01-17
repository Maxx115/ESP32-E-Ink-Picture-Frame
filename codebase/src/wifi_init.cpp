#include <WiFi.h> // Load WiFi library
#include <FS.h>
#include <SPIFFS.h>
#include <vector>

const String ssid = "";
const String password = "";

// Scan for local wifi networks - terminal prints out list ordered by signal strength
std::vector<String> WiFiScan(void) {
  std::vector<String> SSIDList;

  int numNetworks = WiFi.scanNetworks();
  if (numNetworks == 0) {
    Serial.println("No networks found");
  }
  else {
    Serial.print("Networks found: ");
    Serial.println(numNetworks);

    for (int i = 0; i < numNetworks; i++) {
      Serial.print(i+1);
      Serial.print(" - SSID: ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" | Strength: ");
      Serial.println(WiFi.RSSI(i));
      SSIDList.push_back(WiFi.SSID(i));
    }
  }
  return SSIDList;
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


void WiFiInit() {
  std::vector<String> localSSIDs = WiFiScan();
}