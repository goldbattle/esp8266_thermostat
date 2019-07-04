


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
        Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
    }

    // Loop through our struct arr and see if it is found
    bool found = false;
    for(int i=0; i<MAX_CLIENTS; i++) {
        if(devices[i].equals(temp.device)) {
            // Skip this if our status is not "OK"
            devices_db[i].status = temp.status;
            if(temp.status != "OK")
                continue;
            // If we are a fresh device, init the history with the current temp
            if(!devices_db[i].initialized) {
                for(int j=0; j<10; j++) {
                    devices_db[i].hist_avg_humidity[j] = temp.humidity;
                    devices_db[i].hist_avg_temperature[j] = temp.temperature;
                }
                for(int j=0; j<48; j++) {
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
            double increment_sec = 1800;
            //double increment_sec = 10;
            if(difftime(devices_db[i].timestamp,devices_db[i].hist_timestamp)>increment_sec) {
                // Debug print
                Serial.println("SHIFTING HIST!! => "+String(difftime(devices_db[i].timestamp,devices_db[i].hist_timestamp)));
                // loop and shift forward
                for(int j=1; j<48; j++) {
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
                devices_db[i].hist_humidity[47] = c_hum;
                devices_db[i].hist_temperature[47] = c_temp;
                devices_db[i].hist_timestamp = devices_db[i].timestamp;
            }
        }
    }

}














