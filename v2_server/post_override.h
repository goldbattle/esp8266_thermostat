


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
