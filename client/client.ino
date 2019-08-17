
#include <time.h>
#include <Hash.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <DHTesp.h>


//=================================================
//=================================================

const char* ssid     = "xxxxxxxx";
const char* password = "xxxxxxxx";
const String device  = "upstairs";
const String server  = "thermostat.local.lan";

//=================================================
//=================================================

// Create AsyncWebServer object on port 80
AsyncWebServer website(80);

// Poll frequency of the sensors (seconds)
#define DEVICE_POLL_FREQ 30

// Initialize DHT sensor 
// NOTE: This is a custom library that works for the esp8266
// NOTE: the adafruit library does not work and starts returning nans
#define DHTPIN  2
DHTesp dht;

// Current value of our sensor
float humidity = 0.0;
float temperature = 0.0;

/// CALIBRATION DATA!!
#define MAX_CALIB 5
float calib_hum[MAX_CALIB];
float calib_temp[MAX_CALIB];
float meas_hum[MAX_CALIB];
float meas_temp[MAX_CALIB];

//=================================================
//=================================================


// Include our handle functions
#include "linreg.h"
#include "func_index.h"
#include "func_storage.h"
#include "func_reading.h"


//=================================================
//=================================================

 
void setup(void)
{

  // You can open the Arduino IDE Serial Monitor window to see what the code is doing
  // Serial connection from ESP-01 via 3.3v console cable
  Serial.begin(115200);
  
  // Initialize temperature sensor
  dht.setup(DHTPIN, DHTesp::DHT22);

  // Load from our eeprom
  load_from_eeprom();
  //clear_eeprom();

  // DHCP Hostname (useful for finding device for static lease)
  WiFi.hostname(device);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\rWorking to connect");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  // Print debug info
  Serial.println("DHT Sensor Client");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Route for root / web page
  website.on("/", HTTP_GET, render_index);
  website.on("/", HTTP_POST, handle_reading);

  // Sleep for a sec
  delay(1000);

  // Start server
  website.begin();


}
 
void loop(void) {

  // Reading temperature and humidity
  // Note that this can take a lot of time and is blocking...
  // Values read from sensor
  humidity = dht.getHumidity();
  temperature = dht.getTemperature();
  String status = dht.getStatusString();

  // Convert from Celsius to Fahrenheit
  temperature = dht.toFahrenheit(temperature);

  // Add our offsets calculated from the linear regression
  float m,b,r;
  linreg(MAX_CALIB,meas_hum,calib_hum,m,b,r);
  float humidity_edited = m*humidity+b;
  linreg(MAX_CALIB,meas_temp,calib_temp,m,b,r);
  float temperature_edited = m*temperature+b;

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor (humidity)!");
    humidity = -1;
    humidity_edited = -1;
  }
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor (temperature)!");
    temperature = -1;
    temperature_edited = -1;
  }

  // Check WiFi connection status
  if(WiFi.status() == WL_CONNECTED) {

    // Declare object of class HTTPClient
    HTTPClient http;

    // Setup the destination and content type
    http.begin("http://"+server+"/reading");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Send the pose request
    Serial.println(device+","+humidity+","+temperature+","+status+","+WiFi.localIP().toString());
    int httpCode = http.POST("device="+device+"&humidity="+humidity_edited+"&temperature="+temperature_edited+"&humidity_raw="+humidity+"&temperature_raw="+temperature+"&status="+status+"&ip="+WiFi.localIP().toString());
    Serial.println("http response: "+httpCode);

    // Could get a return payload...
    //String payload = http.getString();
    //Serial.println(payload);

    // Close connection
    http.end();

  } else{
    Serial.println("Error in WiFi connection");
  }

  // Send a request as fast as our pool frequency
  delay(1000*DEVICE_POLL_FREQ);

}



