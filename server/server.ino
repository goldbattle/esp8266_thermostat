
#include <Arduino.h>
#include <time.h>
#include <Hash.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
//#include <GDBStub.h>


//=================================================
//=================================================

const char* ssid     = "xxxxxxxx";
const char* password = "xxxxxxxx";
const char* device   = "thermostat";

// Static IP address config
IPAddress staticIP(192, 168, 69, 4);  //ESP static ip
IPAddress gateway(192, 168, 69, 1);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255, 255, 255, 0);   //Subnet mask
IPAddress dns(192, 168, 69, 1);       //DNS

// Our system status
bool fan_on = false;
bool cool_on = false;
bool heat_on = false;
int desired_mode = 0; // 0:off, 1:cool, 2:heat
int desired_device = 0; // what device
int desired_temp = 70;
bool desired_fan = false;
bool status_err = false;
String status = "system is starting up...<br/>";

// Pins our relays are set too
#define PIN_FAN 12
#define PIN_COOL 14
#define PIN_HEAT 16

// Known floor name devices
#define MAX_CLIENTS 4
String devices[MAX_CLIENTS] = {"upstairs", "office", "downstairs", "basement"};

// Poll frequency of the sensors (seconds)
#define DEVICE_POLL_FREQ 30

//=================================================
//=================================================


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


/// Struct that contains our sensor values and history
typedef struct {
    // If we have initialized this sensor
    bool initialized;
    // Current sensor values
    time_t timestamp;
    String device;
    float humidity;
    float temperature;
    String status;
    // Rolling average
    float hist_avg_humidity[10];
    float hist_avg_temperature[10];
    // Historical sensor values
    time_t hist_timestamp;
    float hist_humidity[48];
    float hist_temperature[48];
} device_record;

// Our database structure
device_record devices_db[MAX_CLIENTS];

//=================================================
//=================================================


// Include our handle functions
#include "func_storage.h"
#include "func_pins.h"
#include "func_index.h"
#include "func_reading.h"
#include "func_update.h"


//=================================================
//=================================================


void setup() {

    // Serialport for debugging purposes
    Serial.begin(115200);
    //gdbstub_init();

    // Enable the PIN modes for our ouputs
    pinMode(PIN_FAN, OUTPUT);
    pinMode(PIN_COOL, OUTPUT);
    pinMode(PIN_HEAT, OUTPUT);

    // DHCP Hostname (useful for finding device for static lease)
    WiFi.hostname(device);

    // Connect to Wi-Fi with our static ip
    WiFi.config(staticIP, subnet, gateway, dns);
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println(".");
    }

    // Connect to ntp to get our current time
    configTime(3*3600, 0, "pool.ntp.org", "time.nist.gov");
    Serial.println("Waiting for time");
    while (!time(nullptr)) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("");

    // Print ESP8266 Local IP Address
    Serial.println(WiFi.localIP());

    // Load from our eeprom
    load_from_eeprom();
    //clear_eeprom();

    // Loop through all database and set them to not be init'ed
    for(int i=0; i<MAX_CLIENTS; i++) {
        devices_db[i].initialized = false;
        for(int j=0; j<10; j++) {
            devices_db[i].hist_avg_humidity[j] = 0;
            devices_db[i].hist_avg_temperature[j] = 0;
        }
        for(int j=0; j<48; j++) {
            devices_db[i].hist_humidity[j] = 0;
            devices_db[i].hist_temperature[j] = 0;
        }
    }

    // On startup set the pins to what they where based on eeprom
    //handle_pin_updates();

    // Route for root / web page
    server.on("/", HTTP_GET, render_index);
    server.on("/", HTTP_POST, handle_update);

    // Route for getting post data from our other units
    server.on("/reading", HTTP_POST, handle_reading);

    // Sleep for a sec
    delay(1000);

    // Start server
    server.begin();

}




void loop() {

    // Update our pins
    handle_pin_updates();

    // Sleep for a little so we don't spam the pins
    delay(20000);

}






