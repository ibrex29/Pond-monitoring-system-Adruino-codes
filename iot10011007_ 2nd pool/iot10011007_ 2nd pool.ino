#include "DHT.h"
#define DHTPIN D1
#define DHTTYPE DHT11

#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

DHT dht(DHTPIN, DHTTYPE);

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "aa"
#define WIFI_PASSWORD "1234567890"

// Insert Firebase project API Key
#define API_KEY "AIzaSyAlpmQJ4DUnUqOMo5CqszMlfe781elVT8I"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "readdata-4764a-default-rtdb.firebaseio.com/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

//unsigned long sendDataPrevMillis = 0;
//int count = 0;
bool signupOK = false;

void setup(){
  pinMode(DHTPIN, INPUT);
  dht.begin();
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){
  


 delay(1000);

  int sensorValue = analogRead(A0);
  float voltage =sensorValue * (5.0 / 1024.0);
  float ntu = (-1120.4) * sq(voltage) + (5742.3 * voltage ) - 4353.8;
if (Firebase.ready() && signupOK ) {
    
    if (Firebase.RTDB.setFloat(&fbdo, "/iot/pond2/turbidity",ntu)){
//      Serial.println("PASSED");
       Serial.print("turbidity: ");
      Serial.println(ntu);
      
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
}

  float h = dht.readHumidity();

  float t = dht.readTemperature();
  
  if (Firebase.ready() && signupOK ) {
    
    if (Firebase.RTDB.setFloat(&fbdo, "/iot/pond2/humidity",h)){
//      Serial.println("PASSED");
       Serial.print("Humidity: ");
       Serial.println(h);
      
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    
    // Write an Float number on the database path test/float
    if (Firebase.RTDB.setFloat(&fbdo, "/iot/pond2/temperature", t)){
//      Serial.println("PASSED");
       Serial.print("Temperature: ");
       Serial.println(t);
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
  Serial.println("______________________________");
}
