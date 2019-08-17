



void save_to_eeprom() {

    // Create a cache in our ram
    EEPROM.begin(4*sizeof(float)*MAX_CALIB);
    
    // Save to the eeprom
    int n = 0;
    for(int i=0; i<MAX_CALIB; i++) {
        EEPROM.put(n+0*sizeof(float), calib_hum[i]);
        EEPROM.put(n+1*sizeof(float), calib_temp[i]);
        EEPROM.put(n+2*sizeof(float), meas_hum[i]);
        EEPROM.put(n+3*sizeof(float), meas_temp[i]);
        n += 4*sizeof(float);
    }

    // Actually commit our ram to eeprom
    EEPROM.commit();

}


void clear_eeprom() {

    // Create a cache in our ram
    EEPROM.begin(4*sizeof(float)*MAX_CALIB);

    // write a 0 to all 512 bytes of the EEPROM
    for (int i = 0; i < 16; i++) {
        EEPROM.write(i, 0);
    }

    // Done commit it
    EEPROM.end();

}


void load_from_eeprom() {

    // Create a cache in our ram
    EEPROM.begin(4*sizeof(float)*MAX_CALIB);

    // Load it from our eeprom
    int n = 0;
    for(int i=0; i<MAX_CALIB; i++) {
        EEPROM.get(n+0*sizeof(float), calib_hum[i]);
        EEPROM.get(n+1*sizeof(float), calib_temp[i]);
        EEPROM.get(n+2*sizeof(float), meas_hum[i]);
        EEPROM.get(n+3*sizeof(float), meas_temp[i]);
        n += 4*sizeof(float);
    }

    // Print what we loaded
    Serial.println("EEPROM: loaded the following data");
    for(int i=0; i<MAX_CALIB; i++) {
        Serial.print(String(calib_temp[i])+",");
    }
    Serial.print("done loading");


    // We are done with the eeprom
    EEPROM.end();

}



