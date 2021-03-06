

void render_settings() {

  // Create our response object
  // https://gist.github.com/romanseidl/4d6ac8c247a4768417037b1a00b3bb9f
  BufferedResponse response(server, "text/html", 200, 1024);

  // Get current time
  time_t timestamp_now = time(nullptr);
  struct tm *timestamp_local = localtime(&timestamp_now);

  // Render the page (use dark theme if it is the night)
  response.write("<!DOCTYPE HTML><html><head>");
  bool is_dark_out = (timestamp_local->tm_hour > 17 || timestamp_local->tm_hour < 6);
  if(is_dark_out) {
    response.write("<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootswatch/4.5.0/cyborg/bootstrap.min.css\" crossorigin=\"anonymous\">");
  } else {
    response.write("<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootswatch/4.5.0/sandstone/bootstrap.min.css\" crossorigin=\"anonymous\">");
  }
  response.write("<script src=\"https://code.jquery.com/jquery-3.5.1.slim.min.js\"></script>");
  response.write("<script src=\"https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js\"></script>");
  response.write("<script src=\"https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/js/bootstrap.min.js\"></script>");
  response.write("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\">");
  response.write("</head><body style=\"margin-bottom:4rem;max-width:600px;margin:auto;\">");

  // nav bar
  if(is_dark_out) {
    response.write("<nav class=\"navbar navbar-expand-lg navbar-dark bg-dark\">");
  } else {
    response.write("<nav class=\"navbar navbar-expand-lg navbar-dark bg-primary\">");
  }
  response.write("<a class=\"navbar-brand\" href=\"/\">"+String(config.name_thermostat)+" Thermostat</a>");
  response.write("<button class=\"navbar-toggler\" type=\"button\" data-toggle=\"collapse\" data-target=\"#navbar\" aria-controls=\"navbar\">");
  response.write("<span class=\"navbar-toggler-icon\"></span>");
  response.write("</button>");
  response.write("<div class=\"collapse navbar-collapse\" id=\"navbar\">");
  response.write("<ul class=\"navbar-nav mr-auto mt-2 mt-lg-0\">");
  response.write("<li class=\"nav-item\"><a class=\"nav-link\" href=\"/\">Home</a></li>");
  response.write("<li class=\"nav-item\"><a class=\"nav-link\" href=\"/statistics/\">Statistics</a></li>");
  response.write("<li class=\"nav-item active\"><a class=\"nav-link\" href=\"/settings/\">Settings</a></li>");
  response.write("</ul></div></nav>");
  
  //==========================================
  //==========================================
  
  // Error status of the system
  if(settings_status_err) {
    response.write("<div class=\"alert alert-danger text-center\" style=\"margin:2rem 2rem;\">");
    response.write(settings_status);
    response.write("</div>");
    settings_status_err = false;
  }

  // Render our temp selector
  response.write("<h2 class=\"text-center\" style=\"margin-top:2rem;margin-bottom:2rem;\">Configuration</h2>");
  response.write("<form action=\"/settings/\" method=\"POST\" style=\"margin:1rem 2rem;\">");

  // Device name
  response.write("<div class=\"form-group\">");
  response.write("<label>Device Name</label>");
  response.write("<input class=\"form-control form-control-sm\" type=\"text\" name=\"name\" value=\""+String(config.name_thermostat)+"\">");
  response.write("</div>");

  // Network SSID
  response.write("<div class=\"form-group\">");
  response.write("<label>Network SSID</label>");
  response.write("<input class=\"form-control form-control-sm\" type=\"text\" name=\"ssid\" value=\""+String(config.net_ssid)+"\">");
  response.write("</div>");

  // Network password
  response.write("<div class=\"form-group\">");
  response.write("<label>Network Password</label>");
  response.write("<input class=\"form-control form-control-sm\" type=\"password\" name=\"password\" value=\""+String(config.net_pass)+"\">");
  response.write("</div>");

  // Port for our gw1000 gateway
  response.write("<div class=\"form-group\">");
  response.write("<label>GW1000 Port</label>");
  response.write("<input class=\"form-control form-control-sm\" type=\"number\" name=\"gw1000port\" value=\""+String(config.gw1000_port)+"\">");
  response.write("</div>");

  // Hostname for our gw1000 gateway
  response.write("<div class=\"form-group\">");
  response.write("<label>GW1000 Hostname</label>");
  response.write("<input class=\"form-control form-control-sm\" type=\"text\" name=\"gw1000ip\" value=\""+String(config.gw1000_host)+"\">");
  response.write("</div>");

  // Port for our gw1000 gateway
  response.write("<div class=\"form-group\">");
  response.write("<label>GW1000 Port</label>");
  response.write("<input class=\"form-control form-control-sm\" type=\"number\" name=\"gw1000port\" value=\""+String(config.gw1000_port)+"\">");
  response.write("</div>");

  // Poll frequency (minutes)
  response.write("<div class=\"form-group\">");
  response.write("<label>Sensor Poll Frequency (minutes)</label>");
  response.write("<input class=\"form-control form-control-sm\" type=\"number\" name=\"freqpoll\" value=\""+String(config.freq_device_poll)+"\">");
  response.write("</div>");

  // Control frequency (minutes)
  response.write("<div class=\"form-group\">");
  response.write("<label>Control Frequency (minutes)</label>");
  response.write("<input class=\"form-control form-control-sm\" type=\"number\" name=\"freqcontrol\" value=\""+String(config.freq_control)+"\">");
  response.write("</div>");

  // Temp Sliding Window Size
  response.write("<div class=\"form-group\">");
  response.write("<label>Temperature Sliding Window Size</label>");
  response.write("<input class=\"form-control form-control-sm\" type=\"number\" name=\"sliding\" value=\""+String(config.sliding_window_size)+"\">");
  response.write("</div>");

  // Temp Hysteresis
  response.write("<div class=\"form-group\">");
  response.write("<label>Temperature Hysteresis</label>");
  response.write("<input class=\"form-control form-control-sm\" type=\"number\" step=\"0.01\" name=\"hysteresis\" value=\""+String(config.hysteresis)+"\">");
  response.write("</div>");

  // Submit button
  response.write("<button type=\"submit\" class=\"btn btn-primary btn-lg btn-block\">Save Settings</button>");

  // Loop through each device and have a form for their name
  response.write("<h2 class=\"text-center\" style=\"margin-top:2rem;margin-bottom:2rem;\">Display Names</h2>");
  for(int i=0; i<NUM_SENSORS; i++) {
    response.write("<div class=\"form-group\">");
    response.write("<label>Device "+String(i)+" Display Name</label>");
    response.write("<input class=\"form-control form-control-sm\" type=\"text\" name=\"device_"+String(i)+"\" value=\""+String(config.name_sensors[i])+"\">");
    response.write("</div>");
  }

  // Submit button
  response.write("<button type=\"submit\" class=\"btn btn-primary btn-lg btn-block\">Save Settings</button>");
  response.write("</form>");

  //==========================================
  //==========================================

  // nice footer of generation time
  response.write("<div class=\"text-muted\" style=\"text-align:center;margin-top:2em;\">Page generated at " + String(asctime(timestamp_local)) + "</div>");
  
  // End the html document
  response.write("</body></html>"); 

}
