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



