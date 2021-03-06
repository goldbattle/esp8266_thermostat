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

    // Our new calibration values
    float meas_tempr = 0.0;
    float meas_humid = 0.0;
    float calib_tempr = 0.0;
    float calib_humid = 0.0;

    // Loop through all parameters
    int params = request->params();
    for(int i=0; i<params; i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->name()=="m_temp") meas_tempr = p->value().toFloat();
        if(p->name()=="m_hum") meas_humid = p->value().toFloat();
        if(p->name()=="c_temp") calib_tempr = p->value().toFloat();
        if(p->name()=="c_hum") calib_humid = p->value().toFloat();
        Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
    }
    
    // Append to our history of calibration values
    for(int j=1; j<MAX_CALIB; j++) {
        calib_temp[j-1] = calib_temp[j];
        calib_hum[j-1] = calib_hum[j];
        meas_temp[j-1] = meas_temp[j];
        meas_hum[j-1] = meas_hum[j];
    }
    calib_temp[MAX_CALIB-1] = calib_tempr;
    calib_hum[MAX_CALIB-1] = calib_humid;
    meas_temp[MAX_CALIB-1] = meas_tempr;
    meas_hum[MAX_CALIB-1] = meas_humid;

    // Re-calculate our current best fit line
    save_to_eeprom();

    // Redirect back to homepage
    request->redirect("/");

}














