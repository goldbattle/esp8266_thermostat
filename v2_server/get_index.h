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


String print_schedule_hour_row(int hour, bool bold) {
  
  // Convert time to 12 hour time
  int hour_ampm = hour;
  if(hour == 0) {
    hour_ampm = 12;
  } else if(hour > 12) {
    hour_ampm = hour - 12;
  }

  // 0am midnight to 11am
  String ampm = (hour < 12)? "am" : "pm";

  // Time
  String style = (!bold)? "" : "class=\"table-active\"";
  String html_data = "<tr "+style+">";
  html_data += "<td scope=\"row\">"+String(hour_ampm)+" "+ampm+"</td>";

  // Previous day temp
  //html_data += "<td>0.00&degF &uarr;, 0.00&degF &darr;<br/>0% &uarr;, 0% &darr;</td>";

  // Sensor
  html_data += "<td>";
  html_data += "<select class=\"form-control form-control-sm\" name=\"device_"+String(hour)+"\">";
  for(int i=0; i<NUM_SENSORS; i++) {
    html_data += "<option value=\""+String(i)+"\" "+((schedule_control[hour].desired_device==i)?String("selected"):String(""));
    html_data += ">"+String(config.name_sensors[i])+((last_payload.sensors[i].has_value)?" (online)":" (offline)")+"</option>";
  }
  html_data += "</select>";
  html_data += "</td>";
  
  // Mode
  html_data += "<td>";
  html_data += "<select class=\"form-control form-control-sm\" name=\"mode_"+String(hour)+"\">";
  html_data += "<option value=\"0\" "+((schedule_control[hour].desired_mode==0)?String("selected"):String(""))+">off</option>";
  html_data += "<option value=\"1\" "+((schedule_control[hour].desired_mode==1)?String("selected"):String(""))+">cooling</option>";
  html_data += "<option value=\"2\" "+((schedule_control[hour].desired_mode==2)?String("selected"):String(""))+">heating</option>";
  html_data += "</select>";
  html_data += "</td>";

  // Desired temp
  html_data += "<td><input class=\"form-control form-control-sm\" type=\"number\" name=\"temp_"+String(hour)+"\" value=\""+String(schedule_control[hour].desired_temp)+"\"/></td>";

  // Fan
  html_data += "<td>";
  String checked = (schedule_control[hour].desired_fan)? "checked" : "";
  html_data += "<div class=\"form-check\"><input type=\"checkbox\" class=\"form-check-input\" name=\"fan_"+String(hour)+"\" "+checked+"/></div>";
  html_data += "</select>";
  html_data += "</td>";

  // Finally return
  return html_data;

}


void render_index() {

  // Create our response object
  // https://gist.github.com/romanseidl/4d6ac8c247a4768417037b1a00b3bb9f
  BufferedResponse response(server, "text/html", 200, 1024);

  // Get current time
  time_t timestamp_now = time(nullptr);
  struct tm *timestamp_local = localtime(&timestamp_now);

  // Calculate time difference
  int time_diff = (int)difftime(timestamp_now,last_payload_time);
  int seconds = (time_diff % 60);
  int minutes = (time_diff % 3600) / 60;
  int hours = (time_diff % 86400) / 3600;

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
  //response.write("<meta http-equiv=\"refresh\" content=\"120\">");
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
  response.write("<li class=\"nav-item active\"><a class=\"nav-link\" href=\"/\">Home</a></li>");
  response.write("<li class=\"nav-item\"><a class=\"nav-link\" href=\"/statistics/\">Statistics</a></li>");
  response.write("<li class=\"nav-item\"><a class=\"nav-link\" href=\"/settings/\">Settings</a></li>");
  response.write("</ul></div></nav>");
  
  
  //==========================================
  //==========================================

  // Count how many sensors are online
  int ct_online = 0;
  for(int i=0; i<NUM_SENSORS; i++) {
    ct_online += (int)last_payload.sensors[i].has_value;
  }

  // Status of the system
  response.write("<div class=\"alert alert-"+((status_err)?String("danger"):String("primary"))+" text-center\" style=\"margin:2rem 2rem;\">");
  response.write(status);
  response.write("</div>");

  // Current status of the system
  if(ct_online > 1) {
    response.write("<table class=\"table table-condensed table-sm\" style=\"text-align:center;\">");
    response.write("<thead><tr>");
    response.write("<th scope=\"col\">Sensor</th>");
    response.write("<th scope=\"col\">Temperature</th>");
    response.write("<th scope=\"col\">Humidity</th>");
    response.write("</tr><tbody>");
    for(int i=0; i<NUM_SENSORS; i++) {
      if(!last_payload.sensors[i].has_value)
        continue;
      response.write("<tr>");
      response.write("<td>"+String(config.name_sensors[i])+"</td>");
      float temp_f = (last_payload.sensors[i].temp/10.0) * 9.0/5.0 + 32;
      float temp_f_max = (stats_curr24.payload_max.sensors[i].temp/10.0) * 9.0/5.0 + 32;
      float temp_f_min = (stats_curr24.payload_min.sensors[i].temp/10.0) * 9.0/5.0 + 32;
      response.write("<td>"+String(temp_f)+"&degF <br/>"+String(temp_f_max)+"&uarr;, "+String(temp_f_min)+"&darr;</td>");
      response.write("<td>"+String(last_payload.sensors[i].humid)+"% <br/>");
      response.write(String(stats_curr24.payload_max.sensors[i].humid)+"&uarr;, "+String(stats_curr24.payload_min.sensors[i].humid)+"&darr;</td>");
      response.write("</tr>");
    }
    response.write("</tbody></table>");
    response.write("<hr/>");
  }
  
  //==========================================
  //==========================================

  // Render our temp selector
  response.write("<h2 class=\"text-center\" style=\"margin-top:2rem;margin-bottom:2rem;\">System Override</h2>");
  response.write("<form  action=\"/override/\" method=\"POST\" style=\"margin:1rem 2rem;\">");
  response.write("<div class=\"form-group\">");
  response.write("<label>Desired Temp</label>");
  response.write("<input class=\"form-control form-control-sm\" type=\"number\" name=\"temp\" value=\""+String(override_control.desired_temp)+"\">");
  response.write("</div>");

  // Render what device we will reference
  response.write("<div class=\"form-group\">");
  response.write("<label>Reference Device</label>");
  response.write("<select class=\"form-control form-control-sm\" name=\"device\">");
  for(int i=0; i<NUM_SENSORS; i++) {
    response.write("<option value=\""+String(i)+"\" "+((override_control.desired_device==i)?String("selected"):String("")));
    response.write(">"+String(config.name_sensors[i])+((last_payload.sensors[i].has_value)?" (online)":" (offline)")+"</option>");
  }
  response.write("</select>");
  response.write("</div>");

  // Render device mode
  response.write("<div class=\"form-group\">");
  response.write("<label>System Mode</label>");
  response.write("<select class=\"form-control form-control-sm\" name=\"mode\">");
  response.write("<option value=\"0\" "+((override_control.desired_mode==0)?String("selected"):String(""))+">off</option>");
  response.write("<option value=\"1\" "+((override_control.desired_mode==1)?String("selected"):String(""))+">cooling</option>");
  response.write("<option value=\"2\" "+((override_control.desired_mode==2)?String("selected"):String(""))+">heating</option>");
  response.write("</select>");
  response.write("</div>");

  // Render fan mode
  response.write("<div class=\"form-group\">");
  response.write("<label>Fan Mode</label>");
  response.write("<select class=\"form-control form-control-sm\" name=\"fan\">");
  response.write("<option value=\"0\" "+((!override_control.desired_fan)?String("selected"):String(""))+">off</option>");
  response.write("<option value=\"1\" "+((override_control.desired_fan)?String("selected"):String(""))+">on</option>");
  response.write("</select>");
  response.write("</div>");

  // Form submit button
  if((int)difftime(timestamp_now,override_time) < 3600) {
    response.write("<button type=\"submit\" class=\"btn btn-danger btn-lg btn-block\">Disable Override</button>");
  } else {
    response.write("<button type=\"submit\" class=\"btn btn-primary btn-lg btn-block\">Override for 1hr</button>");
  }
  response.write("</form>");
  response.write("<hr/>");

  //==========================================
  //==========================================

  // Our programadic schedule
  response.write("<h2 class=\"text-center\" style=\"margin-top:2rem;margin-bottom:2rem;\">Programmed Schedule</h2>");
  response.write("<form  action=\"/schedule/\" method=\"POST\">");
  response.write("<table class=\"table table-condensed table-sm\" style=\"text-align:center;\">");
  response.write("<thead><tr>");
  response.write("<th scope=\"col\" width=\"20%\">Time</th>");
  //response.write("<th scope=\"col\">Prev. Day</th>");
  response.write("<th scope=\"col\" width=\"40%\">Sensor</th>");
  response.write("<th scope=\"col\" width=\"25%\">Mode</th>");
  response.write("<th scope=\"col\" width=\"10%\">Temp</th>");
  response.write("<th scope=\"col\" width=\"5%\">Fan</th>");
  response.write("</tr><tbody>");

  // Append a row for each hour of the day
  for(int i=0; i<24; i++) {
    response.write(print_schedule_hour_row(i, (timestamp_local->tm_hour == i)));
  }

  // End the table
  response.write("</tbody></table>");
  
  // Form submit button
  response.write("<div style=\"margin:1rem 2rem;\"><button type=\"submit\" class=\"btn btn-primary btn-lg btn-block\">Save Schedule</button></div>");
  response.write("</form>");

  //==========================================
  //==========================================

  // nice footer of generation time
  response.write("<div class=\"text-muted\" style=\"text-align:center;margin-top:2em;\">Page generated at " + String(asctime(timestamp_local)) + "</div>");
  
  // End the html document
  response.write("</body></html>"); 


}
