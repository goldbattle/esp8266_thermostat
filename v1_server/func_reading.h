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



void handle_reading(AsyncWebServerRequest *request) {

    // Our new data record
    device_record temp;
    temp.timestamp = time(nullptr);

    // Loop through all parameters
    int params = request->params();
    for(int i=0; i<params; i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->name()=="device") temp.device = p->value();
        if(p->name()=="humidity") temp.humidity = p->value().toFloat();
        if(p->name()=="temperature") temp.temperature = p->value().toFloat();
        if(p->name()=="status") temp.status = p->value();
        if(p->name()=="ip") temp.ipaddress = p->value();
        Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
    }

    // Loop through our struct arr and see if it is found
    bool found = false;
    for(int i=0; i<MAX_CLIENTS; i++) {
        if(devices[i].equals(temp.device)) {
            // Skip this if our status is not "OK"
            devices_db[i].status = temp.status;
            devices_db[i].ipaddress = temp.ipaddress;
            if(temp.status != "OK")
                continue;
            if(isnan(temp.humidity) || isnan(temp.temperature))
                continue;
            // If we are a fresh device, init the history with the current temp
            if(!devices_db[i].initialized) {
                for(int j=0; j<10; j++) {
                    devices_db[i].hist_avg_humidity[j] = temp.humidity;
                    devices_db[i].hist_avg_temperature[j] = temp.temperature;
                }
                for(int j=0; j<12; j++) {
                    devices_db[i].hist_humidity[j] = temp.humidity;
                    devices_db[i].hist_temperature[j] = temp.temperature;
                }
                devices_db[i].initialized = true;
            }
            // update current value
            devices_db[i].timestamp = temp.timestamp;
            devices_db[i].device = temp.device;
            devices_db[i].humidity = temp.humidity;
            devices_db[i].temperature = temp.temperature;
            found = true;
            // Update our sliding window of values
            for(int j=1; j<10; j++) {
                devices_db[i].hist_avg_humidity[j-1] = devices_db[i].hist_avg_humidity[j];
                devices_db[i].hist_avg_temperature[j-1] = devices_db[i].hist_avg_temperature[j];
            }
            devices_db[i].hist_avg_humidity[9] = devices_db[i].humidity;
            devices_db[i].hist_avg_temperature[9] = devices_db[i].temperature;
            // check if we should append our historical times
            double increment_sec = 7200;
            if(difftime(devices_db[i].timestamp,devices_db[i].hist_timestamp)>increment_sec) {
                // Debug print
                Serial.println("SHIFTING HIST!! => "+String(difftime(devices_db[i].timestamp,devices_db[i].hist_timestamp)));
                // loop and shift forward
                for(int j=1; j<12; j++) {
                    devices_db[i].hist_humidity[j-1] = devices_db[i].hist_humidity[j];
                    devices_db[i].hist_temperature[j-1] = devices_db[i].hist_temperature[j];
                }
                // Calculate the current average temp
                float c_hum = 0;
                float c_temp = 0;
                for(int j=0; j<10; j++) {
                    c_hum += devices_db[i].hist_avg_humidity[j];
                    c_temp += devices_db[i].hist_avg_temperature[j];
                }
                c_hum /= 10;
                c_temp /= 10;
                // Set the newest to be the current temp
                devices_db[i].hist_humidity[12-1] = c_hum;
                devices_db[i].hist_temperature[12-1] = c_temp;
                devices_db[i].hist_timestamp = devices_db[i].timestamp;
            }
        }
    }

}














