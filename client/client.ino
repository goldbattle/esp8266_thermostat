

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DHTesp.h>



//=================================================
//=================================================

const char* ssid     = "xxxxxxxx";
const char* password = "xxxxxxxx";
const String device  = "office";
const String server  = "192.168.69.4";

//=================================================
//=================================================

// Poll frequency of the sensors (seconds)
#define DEVICE_POLL_FREQ 30

// Initialize DHT sensor 
// NOTE: This is a custom library that works for the esp8266
// NOTE: the adafruit library does not work and starts returning nans
#define DHTPIN  2
DHTesp dht;


// Values read from sensor
float humidity = -1;
float temperature = -1;
String status = "Unknown";


/// CALIBRATION DATA
// baseline => 49% 75F
// upstairs => 42% 83F
// downstairs => 41% 83F
// basement => 41% 83F
float offset_humidity = -7.5;
float offset_temperature = -8.0;

 
void setup(void)
{

  // You can open the Arduino IDE Serial Monitor window to see what the code is doing
  // Serial connection from ESP-01 via 3.3v console cable
  Serial.begin(115200);
  
  // Initialize temperature sensor
  dht.setup(DHTPIN, DHTesp::DHT22);

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
  Serial.println("DHT Weather Reading Server");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


}
 
void loop(void) {

  // Reading temperature and humidity
  // Note that this can take a lot of time and is blocking...
  humidity = dht.getHumidity();
  temperature = dht.getTemperature();
  status = dht.getStatusString();

  // Convert from Celsius to Fahrenheit
  temperature = dht.toFahrenheit(temperature);

  // Add our offsets
  humidity += offset_humidity;
  temperature += offset_temperature;

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor (humidity)!");
    humidity = -1;
  }
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor (temperature)!");
    temperature = -1;
  }

  // Check WiFi connection status
  if(WiFi.status() == WL_CONNECTED) {

    // Declare object of class HTTPClient
    HTTPClient http;

    // Setup the destination and content type
    http.begin("http://"+server+"/reading");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Send the pose request
    Serial.println(device+","+humidity+","+temperature+","+status);
    int httpCode = http.POST("device="+device+"&humidity="+humidity+"&temperature="+temperature+"&status="+status);
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



