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



void post_settings() {

  // Create a copy of our config object
  // If we fail for any errors, then we will not overwrite
  config_data config_tmp = config;
  settings_status_err = false;
  settings_status = "";

  // Loop through all parameters
  for(int p=0; p<server.args(); p++){
    // general config
    if(server.argName(p)=="name") server.arg(p).toCharArray(config_tmp.name_thermostat,sizeof(config_tmp.name_thermostat)-1);
    if(server.argName(p)=="ssid") server.arg(p).toCharArray(config_tmp.net_ssid,sizeof(config_tmp.net_ssid)-1);
    if(server.argName(p)=="password") server.arg(p).toCharArray(config_tmp.net_pass,sizeof(config_tmp.net_pass)-1);
    if(server.argName(p)=="gw1000ip") server.arg(p).toCharArray(config_tmp.gw1000_host,sizeof(config_tmp.gw1000_host)-1);
    if(server.argName(p)=="gw1000port") config_tmp.gw1000_port = server.arg(p).toInt();
    if(server.argName(p)=="freqpoll") config_tmp.freq_device_poll = server.arg(p).toInt();
    if(server.argName(p)=="freqcontrol") config_tmp.freq_control = server.arg(p).toInt();
    if(server.argName(p)=="sliding") config_tmp.sliding_window_size = server.arg(p).toInt();
    if(server.argName(p)=="hysteresis") config_tmp.hysteresis = server.arg(p).toFloat();
    // device names
    for(int i=0; i<NUM_SENSORS; i++){
      if(server.argName(p)=="device_"+String(i)) server.arg(p).toCharArray(config_tmp.name_sensors[i],sizeof(config_tmp.name_sensors[i])-1);
    }
    // debug 
    Serial.printf("POST[%s]: %s\n", server.argName(p).c_str(), server.arg(p).c_str());
  }


  // double check our num bounds
  if(config_tmp.gw1000_port < 1) {
    settings_status += "GW1000 port needs to be a positive number<br/>";
    settings_status_err = true;
  }
  if(config_tmp.freq_device_poll < 1) {
    settings_status += "Device poll frequency positive non-zero number<br/>";
    settings_status_err = true;
  }
  if(config_tmp.freq_control < 1) {
    settings_status += "Control frequency positive non-zero number<br/>";
    settings_status_err = true;
  }
  if(config_tmp.sliding_window_size < 1 || config_tmp.sliding_window_size > MAX_TEMP_SLIDING_WINDOW) {
    settings_status += "Sliding window size needs to within the range 0 < n < "+String(MAX_TEMP_SLIDING_WINDOW)+"<br/>";
    settings_status_err = true;
  }
  if(config_tmp.hysteresis < 0.0) {
    settings_status += "Hysteresis should be greater than or equal to zero<br/>";
    settings_status_err = true;
  }

  // If no errors, then these settings are good!
  if(!settings_status_err) {

    // Restart if network has changed
    bool do_restart = (
      String(config_tmp.net_ssid) != String(config.net_ssid)
      || String(config_tmp.net_pass) != String(config.net_pass)
    );

    // update the config
    config = config_tmp;
   
    // Save to disk / eeprom
    save_to_eeprom();

    // Restart!
    if(do_restart) {
      ESP.restart();
    }
    
  }

  // Redirect back to homepage
  server.sendHeader("Location","/settings/");
  server.send(303);

}
