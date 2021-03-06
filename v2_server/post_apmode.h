


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
