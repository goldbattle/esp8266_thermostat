/*
 * ESP8266 Thermostat System
 * Copyright (C) 2021 Patrick Geneva
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <Arduino.h>
#include <time.h>
#include <EEPROM.h>
#include <Hash.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>



//=================================================
//=================================================

// Pins our relays are set too
#define PIN_FAN 12
#define PIN_COOL 2
#define PIN_HEAT 16


//=================================================
//=================================================

// Load our datatypes
#include "datatypes.h"

// Our system status
bool fan_on = false;
bool cool_on = false;
bool heat_on = false;
bool status_err = false;
String status = "system is starting up...<br/>";

// The current override information and schedule
time_t last_control_time;
time_t override_time;
control override_control;
control schedule_control[24];

// Create webserver object on port 80
ESP8266WebServer server(80);

// Last payload from the sensor
time_t last_poll_time;
time_t last_payload_time;
payload last_payload;

// Sliding window of historical temps
#define MAX_TEMP_SLIDING_WINDOW 50
int last_control_temp_ct = 0;
float last_control_temp[MAX_TEMP_SLIDING_WINDOW];

// Max min values over the last 24 hours
bool not_reset = true;
daily_stats stats_curr24;

// Our daily statistic objects
daily_stats stats_daily[24];


//=================================================
//=================================================

// if we are "ok" and have loaded
char is_ok[2];

// If we are in access point or are connected to the wifi
// Try to use 8.8.8.8 so android phones listen to our DNS server
int wifi_max_tries = 30;
bool ap_active = false;
IPAddress ap_ip(8, 8, 8, 8);
IPAddress ap_netmask(255, 255, 255, 0);
const byte ap_dns_port = 53;
DNSServer ap_dns_server;

// Main config data for our system
struct config_data {

  // what network we will connect too
  char net_ssid[32] = "";
  char net_pass[32] = "";

  // gw1000 polling information
  char gw1000_host[32] = "192.168.1.162";
  unsigned int gw1000_port = 45000;

  // poll frequencies (minutes)
  unsigned char freq_device_poll = 1;
  unsigned char freq_control = 1;

  // window size to temperatures over
  unsigned char sliding_window_size = 10;

  // device name (used for title and host name)
  char name_thermostat[32] = "ESP8266";

  // display name for each sensor
  char name_sensors[NUM_SENSORS][32];

  // hysteresis to heat / cool above
  float hysteresis = 1.0;
  
};
config_data config;

// Error information for the settings page
bool settings_status_err = false;
String settings_status = "";


//=================================================
//=================================================


// General utility functions
#include "BufferedResponse.h"
#include "func_lookup.h"
#include "func_storage.h"
#include "func_pins.h"

// Include our handle functions
#include "get_index.h"
#include "get_stats.h"
#include "get_settings.h"
#include "get_notfound.h"
#include "get_apmode.h"
#include "post_override.h"
#include "post_schedule.h"
#include "post_settings.h"
#include "post_apmode.h"

//=================================================
//=================================================

void reconnect() {  
  
  // DHCP Hostname (useful for finding device for static lease)
  String macaddr = WiFi.softAPmacAddress();
  macaddr.replace(":","");
  String hostname = "ESP_"+macaddr.substring(0,5);
  WiFi.hostname(hostname);

  // Connect to Wi-Fi with our static ip
  WiFi.begin(config.net_ssid, config.net_pass);
  Serial.println("Connecting to WiFi");
  int num_tries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    num_tries++;
    if(num_tries > wifi_max_tries) {
      Serial.println("");
      Serial.println("ERROR: Unable to connect to wifi (enabling AP mode)");
      Serial.println("ERROR: \t- "+String(config.net_ssid));
      Serial.println("ERROR: \t- "+WiFi.softAPmacAddress());
      ap_active = true;
      WiFi.softAPConfig(ap_ip, ap_ip, ap_netmask);
      WiFi.softAP(hostname);
      return;
    }
  }
  Serial.println("");

  // Configure our timezone
  // https://remotemonitoringsystems.ca/time-zone-abbreviations.php
  // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
  int timezone = 5;
  char *tz_info = "EST5EDT,M3.2.0,M11.1.0";
  //configTime(timezone*3600, 0, "pool.ntp.org", "time.nist.gov");
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  setenv("TZ", tz_info, 1);

  // Connect to ntp to get our current time
  Serial.println("Waiting for time");
  int i = 0;
  while (time(nullptr) < 1000000000ul) {
    delay(500);
    Serial.print(".");
    i++;
  }
  Serial.println("");

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

}



void setup() {

  // Serialport for debugging purposes
  Serial.begin(115200);

  // Enable the PIN modes for our ouputs
  pinMode(PIN_FAN, OUTPUT);
  pinMode(PIN_COOL, OUTPUT);
  pinMode(PIN_HEAT, OUTPUT);

  // Default the device names
  for(int i=0; i<NUM_SENSORS; i++) {
    sprintf(config.name_sensors[i], "Sensor %d", i);
  }

  // Load config and stats from our eeprom
  load_from_eeprom();

  // Connect to the wifi
  reconnect();

  // If we have connected to our wifi, then we are good
  if(!ap_active) {
  
    // Init our max and min values for the system
    stats_curr24.reset();
    time_t timestamp_now = time(nullptr);
    struct tm *timestamp_local = localtime(&timestamp_now);
    stats_curr24.day = timestamp_local->tm_mday;
    stats_curr24.month = timestamp_local->tm_mon;
    stats_curr24.year_since_1900 = timestamp_local->tm_year;

    // Load the first set of sensor data
    last_poll_time = time(nullptr);
    last_payload_time = time(nullptr);
    get_latest_payload(config.gw1000_host, config.gw1000_port);
    stats_curr24.update_minmax(last_payload);

    // On startup set the pins to what they where based on eeprom
    last_control_time = time(nullptr);
    handle_pin_updates();

    // Route for root / web page
    server.on("/", HTTP_GET, render_index);
    server.on("/statistics/", HTTP_GET, render_statistics);
    server.on("/settings/", HTTP_GET, render_settings);

    // Post from forms
    server.on("/override/", HTTP_POST, post_override);
    server.on("/schedule/", HTTP_POST, post_schedule);
    server.on("/settings/", HTTP_POST, post_settings);

    // Start server
    server.begin();

  } else {

    // Route for root / web page
    server.on("/", HTTP_GET, render_apmode);
    server.on("/generate_204", HTTP_GET, render_apmode); // Android captive portal.
    server.on("/fwlink", HTTP_GET, render_apmode); // Microsoft captive portal.
    server.on("/save_ap/", HTTP_POST, post_apmode);
    server.onNotFound(render_notfound);

    // Start server
    ap_dns_server.start(ap_dns_port, "*", ap_ip);
    server.begin();

  }


}


void loop() {

  // If we are in AP mode, then we should just process requests
  // Also we should process DNS requests here too!
  if(ap_active) {
    
    // Handle requests
    ap_dns_server.processNextRequest();
    server.handleClient();

    // Still try to connect to the wifi (maybe service just dropped...)
    if (WiFi.status() == WL_CONNECTED) {
      ESP.restart();
    }
    return;

  }

  // Main control logic updates at a lower rate
  time_t timestamp_now = time(nullptr);
  int time_diff = (int)difftime(timestamp_now,last_poll_time);
  if(time_diff > config.freq_device_poll*60) {

    // Get the latest live data from the sensor
    get_latest_payload(config.gw1000_host, config.gw1000_port);

    // Update our poll time
    last_poll_time = timestamp_now;

    // Reset our max and min values if it is near midnight
    // 1) if it is 12pm and we have not reset yet, then reset it
    // 2) if it is not 1am, then we can say that we have not reset yet
    struct tm *timestamp_local = localtime(&timestamp_now);
    stats_curr24.day = timestamp_local->tm_mday;
    stats_curr24.month = timestamp_local->tm_mon;
    stats_curr24.year_since_1900 = timestamp_local->tm_year;
    if(not_reset && timestamp_local->tm_hour == 23) {
      // move all old stats forward and append
      for(int i=sizeof(stats_daily)/sizeof(stats_daily[0])-1; i>0; i--)
        stats_daily[i] = stats_daily[i-1];
      stats_daily[0] = stats_curr24;
      // actually reset the current data      
      stats_curr24.reset();
      not_reset = false;
      // save everything to file
      save_to_eeprom();
    }
    if(!not_reset && timestamp_local->tm_hour != 23) {
      not_reset = true;
    }

    // Update the max and min values
    stats_curr24.update_minmax(last_payload);
    
  }

  // Update our pins changes and updates
  timestamp_now = time(nullptr);
  time_diff = (int)difftime(timestamp_now,last_control_time);
  if(time_diff > config.freq_control*60) {
    handle_pin_updates();
  }

  // Handle any http traffic clients
  server.handleClient();

  // If we have lost internet, try to reconnect
  if (WiFi.status() != WL_CONNECTED) {
    status = "ERROR: Unable to connect to the wireless network!!!<br/>";
    status += "ERROR: Trying to connect to <b>"+String(config.net_ssid)+"</b>!!<br/>";
    status += "ERROR: You can update the SSID in the Settings page.";
    status_err = true;
    reconnect();
  }

}
