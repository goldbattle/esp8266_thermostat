

void render_statistics() {

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
  response.write("<li class=\"nav-item active\"><a class=\"nav-link\" href=\"/statistics/\">Statistics</a></li>");
  response.write("<li class=\"nav-item\"><a class=\"nav-link\" href=\"/settings/\">Settings</a></li>");
  response.write("</ul></div></nav>");
  
  //==========================================
  //==========================================

  response.write("<h2 class=\"text-center\" style=\"margin-top:2rem;margin-bottom:2rem;\">System Statistics</h2>");
  response.write("<table class=\"table table-condensed table-sm\" style=\"text-align:center;\">");
  response.write("<thead><tr>");
  response.write("<th scope=\"col\">Date</th>");
  response.write("<th scope=\"col\">Temp (T"+String(NUM_SENSORS-1)+")</th>");
  response.write("<th scope=\"col\">Hum (T"+String(NUM_SENSORS-1)+")</th>");
  response.write("<th scope=\"col\">Cooling</th>");
  response.write("<th scope=\"col\">Heating</th>");
  response.write("<th scope=\"col\">Fan</th>");
  response.write("</tr><tbody>");

  // Add the current day
  response.write("<tr>");
  response.write("<td>"+String(stats_curr24.month)+"/"+String(stats_curr24.day)+"/"+String(stats_curr24.year_since_1900+1900)+"</td>");
  float temp_f_max = (stats_curr24.payload_max.sensors[NUM_SENSORS-1].temp/10.0) * 9.0/5.0 + 32;
  float temp_f_min = (stats_curr24.payload_min.sensors[NUM_SENSORS-1].temp/10.0) * 9.0/5.0 + 32;
  response.write("<td>"+String(temp_f_max)+"&uarr;, "+String(temp_f_min)+"&darr;</td>");
  response.write("<td>"+String(stats_curr24.payload_max.sensors[NUM_SENSORS-1].humid)+"&uarr;, "+String(stats_curr24.payload_min.sensors[NUM_SENSORS-1].humid)+"&darr;</td>");
  response.write("<td>"+String(stats_curr24.cool_mins)+"</td>");
  response.write("<td>"+String(stats_curr24.heat_mins)+"</td>");
  response.write("<td>"+String(stats_curr24.fan_mins)+"</td>");
  response.write("</tr>");

  // Add the older days
  for(int i=0; i<sizeof(stats_daily)/sizeof(stats_daily[0]); i++) {
    response.write("<tr>");
    response.write("<td>"+String(stats_daily[i].month)+"/"+String(stats_daily[i].day)+"/"+String(stats_daily[i].year_since_1900+1900)+"</td>");
    float temp_f_max = (stats_daily[i].payload_max.sensors[NUM_SENSORS-1].temp/10.0) * 9.0/5.0 + 32;
    float temp_f_min = (stats_daily[i].payload_min.sensors[NUM_SENSORS-1].temp/10.0) * 9.0/5.0 + 32;
    response.write("<td>"+String(temp_f_max)+"&uarr;, "+String(temp_f_min)+"&darr;</td>");
    response.write("<td>"+String(stats_daily[i].payload_max.sensors[NUM_SENSORS-1].humid)+"&uarr;, "+String(stats_daily[i].payload_min.sensors[NUM_SENSORS-1].humid)+"&darr;</td>");
    response.write("<td>"+String(stats_daily[i].cool_mins)+"</td>");
    response.write("<td>"+String(stats_daily[i].heat_mins)+"</td>");
    response.write("<td>"+String(stats_daily[i].fan_mins)+"</td>");
    response.write("</tr>");
  }

  // End the table
  response.write("</tbody></table>");
  
 

  //==========================================
  //==========================================

  // nice footer of generation time
  response.write("<div class=\"text-muted\" style=\"text-align:center;margin-top:2em;\">Page generated at " + String(asctime(timestamp_local)) + "</div>");
  
  // End the html document
  response.write("</body></html>"); 

}
