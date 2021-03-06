


void render_index(AsyncWebServerRequest *request) {

    // Get current time
    time_t timestamp_now = time(nullptr);

    // Render the page
    String html_data = "<!DOCTYPE HTML><html><head>";
    html_data += "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css\" crossorigin=\"anonymous\">";
    html_data += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\">";
    html_data += "<meta http-equiv=\"refresh\" content=\"60\">";
    html_data += "<style>";
    // html_data += "html { display: inline-block; margin: 0px auto; text-align: center; }";
    // html_data += "h2 { font-size: 2.0rem; }";
    // html_data += "p { font-size: 1.5rem; margin-bottom: 1.0rem; }";
    html_data += ".graphtemp { stroke:green; stroke-width: 1; fill: none; }";
    html_data += ".graphumid { stroke:blue; stroke-width: 1; fill: none; }";
    html_data += ".graphline { stroke:black; stroke-width: 0.5; fill: none; }";
    html_data += "</style>";
    html_data += "</head><body>";

    // Status of the system
    html_data += "<h2 class=\"text-center\" style=\"margin-top:2rem;margin-bottom:2rem;\">ESP8266 Thermostat</h2>";
    html_data += "<div class=\"alert alert-"+((status_err)?String("danger"):String("info"))+" text-center\" style=\"margin:1rem 2rem;\">"+status+"</div>";

    // Render our temp selector
    html_data += "<form  action=\"/\" method=\"POST\" style=\"margin:1rem 2rem;\">";
    html_data += "<div class=\"form-group\">";
    html_data += "<label>Desired Temp</label>";
    html_data += "<input class=\"form-control\" type=\"number\" name=\"temp\" value=\""+String(desired_temp)+"\">";
    html_data += "</div>";

    // Render what device we will reference
    html_data += "<div class=\"form-group\">";
    html_data += "<label>Reference Device</label>";
    html_data += "<select class=\"form-control\" name=\"device\">";
    for(int i=0; i<MAX_CLIENTS; i++) {
        html_data += "<option value=\""+String(i)+"\" "+((desired_device==i)?String("selected"):String(""))+">"+devices[i]+"</option>";
    }
    html_data += "</select>";
    html_data += "</div>";

    // Render device mode
    html_data += "<div class=\"form-group\">";
    html_data += "<label>System Mode</label>";
    html_data += "<select class=\"form-control\" name=\"mode\">";
    html_data += "<option value=\"0\" "+((desired_mode==0)?String("selected"):String(""))+">off</option>";
    html_data += "<option value=\"1\" "+((desired_mode==1)?String("selected"):String(""))+">cooling</option>";
    html_data += "<option value=\"2\" "+((desired_mode==2)?String("selected"):String(""))+">heating</option>";
    html_data += "</select>";
    html_data += "</div>";

    // Render fan mode
    html_data += "<div class=\"form-group\">";
    html_data += "<label>Fan Mode</label>";
    html_data += "<select class=\"form-control\" name=\"fan\">";
    html_data += "<option value=\"0\" "+((!desired_fan)?String("selected"):String(""))+">off</option>";
    html_data += "<option value=\"1\" "+((desired_fan)?String("selected"):String(""))+">on</option>";
    html_data += "</select>";
    html_data += "</div>";

    // Form submit button
    html_data += "<button type=\"submit\" class=\"btn btn-primary btn-lg btn-block\">Update</button>";
    html_data += "</form>";
    html_data += "<hr/>";


    // Render for each client
    for(int i=0; i<MAX_CLIENTS; i++) {

        // Calculate time difference
        int time_diff = (int)difftime(timestamp_now,devices_db[i].timestamp);
        int seconds = (time_diff % 60);
        int minutes = (time_diff % 3600) / 60;
        int hours = (time_diff % 86400) / 3600;

        // Display current status
        html_data += "<p class=\"text-center\">";
        html_data += "<a href=\"http://"+String(devices_db[i].ipaddress)+"/\">"+devices[i]+"</a>: ";
        html_data += String(devices_db[i].status)+" "+String(hours)+" hrs, "+String(minutes)+" min, "+String(seconds)+" sec old<br/>";
        html_data += String(devices_db[i].temperature)+" &#176;F - "+String(devices_db[i].humidity)+" %<br/>";

        // Compute axis min and max values
        int t_min = 99999;
        int t_max = 0;
        int h_min = 100;
        int h_max = 0;
        for(int j=0; j<12; j++) {
            double temp = devices_db[i].hist_temperature[j];
            double hum = devices_db[i].hist_humidity[j];
            if(ceil(temp) > t_max) t_max = ceil(temp);
            if(floor(temp) <= t_min) t_min = floor(temp);
            if(ceil(hum) > h_max) h_max = ceil(hum);
            if(floor(hum) <= h_min) h_min = floor(hum);
        }
        int t_mid = (t_min + t_max)/2;
        int h_mid = (h_min + h_max)/2;
        
        // Display history
        html_data += "<svg width=\"85%\" viewBox=\"0 0 454 150\" preserveAspectRatio=\"xMidYMin\" class=\"graph text-center\">";
        html_data += "<g><text x=\"70\" y=\"120\">-24</text><text x=\"118\" y=\"120\">-21</text><text x=\"166\" y=\"120\">-18</text><text x=\"214\" y=\"120\">-15</text><text x=\"262\" y=\"120\">-12</text><text x=\"310\" y=\"120\">-9</text><text x=\"358\" y=\"120\">-6</text><text x=\"406\" y=\"120\">-3</text><text x=\"454\" y=\"120\"></text></g>";
        html_data += "<g><text x=\"0\" y=\"15\">"+String(t_max)+"&#176;F/"+String(h_max)+"%</text><text x=\"0\" y=\"50\">"+String(t_mid)+"&#176;F/"+String(h_mid)+"%</text><text x=\"0\" y=\"100\">"+String(t_min)+"&#176;F/"+String(h_min)+"%</text></g>";
        html_data += "<g><text x=\"45%\" y=\"140\">Timestamp (hr)</text></g>";

        // Temperature transform calculation
        // highest deg=>0, lowest deg=>100
        html_data += "<polyline class=\"graphtemp\" points=\"";
        for(int j=0; j<12; j++) {
            float m = (abs(t_min-t_max)<1e-5)? 0 : 100.0/(t_min - t_max);
            float y_point = m*devices_db[i].hist_temperature[j]-(m*t_max);
            float x_point = (454.0-70.0)/(11.0-0.0)*j+70.0;
            html_data += String(x_point)+","+String(y_point)+" ";
        }
        html_data += "\"/>";

        // Humidity transform calculation
        // highest rh=>0, lowest rh=>100
        html_data += "<polyline class=\"graphumid\" points=\"";
        for(int j=0; j<12; j++) {
            double m = (abs(h_min-h_max)<1e-5)? 0 : 100.0/(h_min - h_max); 
            double y_point = m*devices_db[i].hist_humidity[j]-(m*h_max);
            double x_point = (454.0-70.0)/(11.0-0.0)*j+70.0;
            html_data += String(x_point)+","+String(y_point)+" ";
        }
        html_data += "\"/>";

        // Nice axis lines
        html_data += "<polyline class=\"graphline\" points=\"68,102  454,102\"/>";
        html_data += "<polyline class=\"graphline\" points=\"68,0  68,102\"/>";
        html_data += "<polyline class=\"graphline\" points=\"68,0  454,0\"/>";
        html_data += "<polyline class=\"graphline\" points=\"454,0  454,102\"/>";
        html_data += "</svg></p>";

        // Nice separator
        if(i+1 < MAX_CLIENTS) {
            html_data += "<hr/>";
        }

    }
    html_data += "</body></html>";

    // Finally return the request
    request->send(200, "text/html", html_data.c_str());


}














