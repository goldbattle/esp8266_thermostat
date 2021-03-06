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


void handle_update(AsyncWebServerRequest *request) {

    // Loop through all parameters
    int params = request->params();
    for(int i=0; i<params; i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->name()=="temp") desired_temp = p->value().toInt();
        if(p->name()=="device") desired_device = p->value().toInt();
        if(p->name()=="mode") desired_mode = p->value().toInt();
        if(p->name()=="fan") desired_fan = (bool)p->value().toInt();
        Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
    }

    // Update our pins
    handle_pin_updates();

    // Save to disk / eeprom
    save_to_eeprom();

    // Redirect back to homepage
    request->redirect("/");

}














