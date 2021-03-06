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



void post_schedule() {

  // The fan will not be posted if set to false
  // Thus loop through and default them
  for(int i=0; i<24; i++){
    schedule_control[i].desired_fan = false;
  }

  // Loop through all parameters
  for(int p=0; p<server.args(); p++) {
    for(int i=0; i<24; i++){
      if(server.argName(p)=="temp_"+String(i)) schedule_control[i].desired_temp = server.arg(p).toInt();
      if(server.argName(p)=="device_"+String(i)) schedule_control[i].desired_device = server.arg(p).toInt();
      if(server.argName(p)=="mode_"+String(i)) schedule_control[i].desired_mode = server.arg(p).toInt();
      if(server.argName(p)=="fan_"+String(i)) schedule_control[i].desired_fan = true;
    }
    //Serial.printf("POST[%s]: %s\n", server.argName(p).c_str(), server.arg(p).c_str());
  }

  // Update our pins
  handle_pin_updates();

  // Save to disk / eeprom
  save_to_eeprom();

  // Redirect back to homepage
  server.sendHeader("Location","/");
  server.send(303);
  
}
