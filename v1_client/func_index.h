




void render_index(AsyncWebServerRequest *request) {

    // Render the page
    String html_data = "<!DOCTYPE HTML><html><head>";
    html_data += "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css\" crossorigin=\"anonymous\">";
    html_data += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\">";
    html_data += "</head><body>";

    // Status of the system
    html_data += "<h2 class=\"text-center\" style=\"margin-top:2rem;margin-bottom:2rem;\">ESP8266 DHT Client</h2>";

    // Print the data that we have recorded
    html_data += "<hr/>";
    html_data += "<p class=\"text-center\">";
    for(int i=MAX_CALIB-1; i>=0; i--) {
        html_data += "calib "+String(calib_hum[i])+"% / "+String(calib_temp[i])+" F -> ";
        html_data += "meas "+String(meas_hum[i])+"% / "+String(meas_temp[i])+" F<br/>";
    }
    html_data += "</p>";

    // Print the current estimated values
    html_data += "<p class=\"text-center\">";
    float m,b,r;
    linreg(MAX_CALIB,meas_hum,calib_hum,m,b,r);
    html_data += "humidity: m="+String(m)+" b="+String(b)+" r="+String(r)+"<br/>";
    linreg(MAX_CALIB,meas_temp,calib_temp,m,b,r);
    html_data += "temperature: m="+String(m)+" b="+String(b)+" r="+String(r)+"<br/>";
    html_data += "</p><hr/>";

    // Render our temp selector
    html_data += "<form  action=\"/\" method=\"POST\" style=\"margin:1rem 2rem;\">";

    // Current RAW values
    html_data += "<div class=\"form-group\">";
    html_data += "<label>Raw Sensor Temperature</label>";
    html_data += "<input class=\"form-control\" type=\"number\" step=\"0.01\" name=\"m_temp\" value=\""+String(temperature)+"\">";
    html_data += "</div>";
    html_data += "<div class=\"form-group\">";
    html_data += "<label>Raw Sensor Humidity</label>";
    html_data += "<input class=\"form-control\" type=\"number\" step=\"0.01\" name=\"m_hum\" value=\""+String(humidity)+"\">";
    html_data += "</div>";

    // Current calibrated values
    linreg(MAX_CALIB,meas_hum,calib_hum,m,b,r);
    float humidity_edited = m*humidity+b;
    linreg(MAX_CALIB,meas_temp,calib_temp,m,b,r);
    float temperature_edited = m*temperature+b;
    html_data += "<div class=\"form-group\">";
    html_data += "<label>Adjusted Sensor Temperature</label>";
    html_data += "<input class=\"form-control\" type=\"number\" value=\""+String(temperature_edited)+"\" disabled>";
    html_data += "</div>";
    html_data += "<div class=\"form-group\">";
    html_data += "<label>Adjusted Sensor Humidity</label>";
    html_data += "<input class=\"form-control\" type=\"number\" value=\""+String(humidity_edited)+"\" disabled>";
    html_data += "</div>";

    // user input temp
    html_data += "<div class=\"form-group\">";
    html_data += "<label>Calibration Temperature</label>";
    html_data += "<input class=\"form-control\" type=\"number\" step=\"0.01\" name=\"c_temp\" value=\""+String(temperature_edited)+"\">";
    html_data += "</div>";
    html_data += "<div class=\"form-group\">";
    html_data += "<label>Calibration Humidity</label>";
    html_data += "<input class=\"form-control\" type=\"number\" step=\"0.01\" name=\"c_hum\" value=\""+String(humidity_edited)+"\">";
    html_data += "</div>";

    // Form submit button
    html_data += "<button type=\"submit\" class=\"btn btn-primary btn-lg btn-block\">Add Calibration</button>";
    html_data += "</form>";
    html_data += "<hr/>";
    html_data += "</body></html>";

    // Finally return the request
    request->send(200, "text/html", html_data.c_str());


}






