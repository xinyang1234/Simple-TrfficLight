#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "your_SSID"
#define WIFI_PASSWORD "your_password"

// Insert Firebase project API Key
#define API_KEY "your_API_key"

// Insert RTDB URL
#define DATABASE_URL "your_data_firebass"

// Define the pin where your push button is connected
#define BUTTON_PIN 2  // GPIO pin for button on ESP32

// Initialize Firebase
FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200);

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  timeClient.begin();
  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback;  //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Set up the push button
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("BUTTON is PRESSED!");
    sendDataToFirebase();
    delay(1000);
  }
  // Serial.println("Loop completed");
}

void sendDataToFirebase() {
  // Generate a random string (for example purposes)
  timeClient.update();
  String currentTime = timeClient.getFormattedTime();
  String randomString = "Lampu penyeberangan Jalan Ir Hendra Bermasalah !! ";
  // Set the data in Firebase
  if (Firebase.RTDB.setString(&fbdo, "test/string", randomString)) {
    Serial.println("Data sent to Firebase");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
    if (Firebase.RTDB.setString(&fbdo, "test/time", currentTime)) {
      Serial.println("Data sent to Firebase");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
  } else {
    Serial.println("Error sending data to Firebase");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}
