#include <WiFi.h> // Load WiFi library
#include <FS.h>
#include <SPIFFS.h>
#include <vector>


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

void preAuth(const std::vector<String>& SSIDList) {
  // Open auth.txt in read mode
  File file = SPIFFS.open("/auth.txt", "r");

  if (!file) {
    Serial.println("Failed to open file for reading!");
    return;
  }
  
  Serial.println("checking for saved credentials...");

  // Read auth file line by line
  while (file.available()) {
    String currentSSID = file.readStringUntil('\n');
    currentSSID.trim(); // Remove leading and trailing whitespace

    auto it = std:: find(SSIDList.begin(), SSIDList.end(), currentSSID);
    if (it != SSIDList.end()){
      // Matching SSID found, return next line as the password
      if (file.available()) {
        String password = file.readStringUntil('\n');
        password.trim();
        Serial.print("Match found for SSID: " + currentSSID);
        Serial.println(" | pass: " + password);
        break;
      }
    }
  }
  file.close();
}











/* void WiFiConnect(void){
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
 */
/* void saveCredentials(const char *ssid, const char *password) {
  File file = SPIFFS.open("/auth.txt","w");
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  file.println(ssid);
  file.println(password);

  file.close();
  Serial.println("Credentials saved to /preauth.txt");
} */

void WiFiInit() {

  std::vector<String> localSSIDs = WiFiScan();
  preAuth(localSSIDs);
}