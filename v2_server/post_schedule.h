


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
