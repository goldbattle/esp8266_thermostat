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
    EEPROM.begin(16);

    // Replace our values in the cache
    EEPROM.put(0, desired_mode);
    EEPROM.put(4, desired_device);
    EEPROM.put(8, desired_temp);
    EEPROM.put(12, desired_fan);

    // Actually commit our ram to eeprom
    EEPROM.commit();

}


void clear_eeprom() {

    // Create a cache in our ram
    EEPROM.begin(16);

    // write a 0 to all 512 bytes of the EEPROM
    for (int i = 0; i < 16; i++) {
        EEPROM.write(i, 0);
    }

    // Done commit it
    EEPROM.end();

}


void load_from_eeprom() {

    // Create a cache in our ram
    EEPROM.begin(16);

    // Load it from our eeprom
    EEPROM.get(0, desired_mode);
    EEPROM.get(4, desired_device);
    EEPROM.get(8, desired_temp);
    EEPROM.get(12, desired_fan);

    // Handle the case where this device doesn't have these in memory
    if(desired_mode < 0 || desired_mode > 3) desired_mode = 0;
    if(desired_device < 0 || desired_device > MAX_CLIENTS) desired_device = 0;
    if(desired_temp==-1) desired_temp = 70;
    if(desired_fan < 0 || desired_fan > 1) desired_fan = false;

    // Print what we loaded
    Serial.println("EEPROM: loaded the following data");
    Serial.println("desired_mode = "+String(desired_mode));
    Serial.println("desired_device = "+String(desired_device));
    Serial.println("desired_temp = "+String(desired_temp));
    Serial.println("desired_fan = "+String(desired_fan));

    // We are done with the eeprom
    EEPROM.end();

}



