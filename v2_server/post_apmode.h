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



void post_apmode() {

  // Loop through all parameters
  for(int p=0; p<server.args(); p++){
    // config
    if(server.argName(p)=="ssid") server.arg(p).toCharArray(config.net_ssid,sizeof(config.net_ssid)-1);
    if(server.argName(p)=="password") server.arg(p).toCharArray(config.net_pass,sizeof(config.net_pass)-1);
    // debug 
    Serial.printf("POST[%s]: %s\n", server.argName(p).c_str(), server.arg(p).c_str());
  }

  // Save to disk / eeprom
  save_to_eeprom();

  // Restart!
  ESP.restart();

}
