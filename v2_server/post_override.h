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



void post_override() {

  // Loop through all parameters
  for(int p=0; p<server.args(); p++){
    if(server.argName(p)=="temp") override_control.desired_temp = server.arg(p).toInt();
    if(server.argName(p)=="device") override_control.desired_device = server.arg(p).toInt();
    if(server.argName(p)=="mode") override_control.desired_mode = server.arg(p).toInt();
    if(server.argName(p)=="fan") override_control.desired_fan = (bool)server.arg(p).toInt();
    //Serial.printf("POST[%s]: %s\n", server.argName(p).c_str(), server.arg(p).c_str());
  }

  // Check if we have not passed an hour of our override
  // If so, then we want to "undo" the override
  time_t timestamp_now = time(nullptr);
  if((int)difftime(timestamp_now,override_time) < 3600) {
    override_time = 0;
  } else {
    override_time = time(nullptr);
  }
  
  // Update our pins
  handle_pin_updates();

  // Redirect back to homepage
  server.sendHeader("Location","/");
  server.send(303);

}
