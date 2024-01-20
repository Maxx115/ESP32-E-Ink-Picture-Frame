#include <WiFi.h> // Load WiFi library


const String ssid = "";
const String password = "";


// Scan for local wifi networks - terminal prints out list ordered by signal strength
#define MAX_NETWORKS 10
String* WiFiScan(void) {
  static String topSSIDs[MAX_NETWORKS];
  int suitableNetworks = 0;
  // Remember total number of found networks, regardless of suitability
  int numNetworks = WiFi.scanNetworks();
  // Handle an unexpected negative number of networks returned
  if (numNetworks < 0) {
    Serial.println("Error scanning networks");
  }
  // Handle the event of 0 networks being detected
  else if (numNetworks == 0) {
    Serial.println("No networks found");
  }
  // Handle any real number of networks being detected
  else {
    Serial.print("Networks found: ");
    Serial.println(numNetworks);

    // Sort networks by signal strength (RSSI)
    for (int i = 0; i < min(MAX_NETWORKS, numNetworks); i++) {
      String SSID = WiFi.SSID(i);
      int RSSI = WiFi.RSSI(i);
      
      // Ignore networks with poor signal strength (RSSI) + print how many networks were ignored
      if (RSSI < -80) {
        Serial.print("+");
        Serial.print(String(numNetworks - i));
        Serial.println(" other networks ignored due to poor signal strenth");
        break;
      }
      // Print each suitable networks SSID along with RSSI + write the SSID to the corresponding array index of topSSIDs
      else {
        Serial.print(i + 1);
        Serial.print(" - SSID: ");
        Serial.print(SSID);
        Serial.print(" | Strength: ");
        Serial.println(RSSI);

        topSSIDs[i] = SSID;
        suitableNetworks++;
      }
    }
  }
  // Print contents of array topSSIDs for troubleshooting + return array topSSIDs
  Serial.print("Returned SSIDs: ");
  for (int i = 0; i < suitableNetworks; i++) {
    Serial.print(String(i) + "[" + topSSIDs[i] + "] ");
  }
  return topSSIDs;
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