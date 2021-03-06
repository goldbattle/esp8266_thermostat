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



void handle_pin_updates() {

    // Reset our status message
    status = "";
    status_err = false;
    fan_on = false;
    heat_on = false;
    cool_on = false;

    // Get the current temp of the system (use the average value)
    float c_hum = 0;
    float c_temp = 0;
    for(int j=0; j<10; j++) {
        c_hum += devices_db[desired_device].hist_avg_humidity[j];
        c_temp += devices_db[desired_device].hist_avg_temperature[j];
    }
    c_hum /= 10;
    c_temp /= 10;

    // If we are cooling, then check if we should use it
    if(desired_mode==0) {
        status += "system mode if set to off, doing nothing<br/>";
    }
    
    // If we are cooling, then check if we should use it
    if(desired_mode==1 && c_temp > desired_temp) {
        status += "current temp of "+String(c_temp)+" > "+String(desired_temp)+"<br/>";
        status += "current humidity is "+String(c_hum)+" %<br/>";
        status += "turning cooling on and fan to move air<br/>";
        cool_on = true;
        fan_on = true;
    } else if(desired_mode==1) {
        status += "current temp of "+String(c_temp)+" < "+String(desired_temp)+"<br/>";
        status += "current humidity is "+String(c_hum)+" %<br/>";
        status += "no need to turn on cooling<br/>";
    }

    // If we are heating, then check if we should use it
    if(desired_mode==2 && c_temp < desired_temp) {
        status += "current temp of "+String(c_temp)+" < "+String(desired_temp)+"<br/>";
        status += "current humidity is "+String(c_hum)+" %<br/>";
        status += "turning heating on and fan to move air<br/>";
        heat_on = true;
        fan_on = true;
    } else if(desired_mode==2) {
        status += "current temp of "+String(c_temp)+" > "+String(desired_temp)+"<br/>";
        status += "current humidity is "+String(c_hum)+" %<br/>";
        status += "no need to turn on heating<br/>";
    }

    // Assert that both our heat and cooling are not on at the same time
    if(heat_on && cool_on) {
        status += "ERROR: both heating and cooling is enabled.<br/>";
        status += "ERROR: will not turn either settings on.<br/>";
        heat_on = false;
        cool_on = false;
        fan_on = false;
        status_err = true;
    }

    // The final check is to see if this data is recent or not
    // If we have not got any information from the device in the past few minutes, then we should turn off
    time_t time_curr = time(nullptr);
    int time_diff = (int)difftime(time_curr,devices_db[desired_device].timestamp);
    if(time_diff>2*DEVICE_POLL_FREQ) {
        int seconds = (time_diff % 60);
        int minutes = (time_diff % 3600) / 60;
        int hours = (time_diff % 86400) / 3600;
        status = "ERROR: have not got any sensor readings from device <b>"+devices[desired_device]+"</b>.<br/>";
        status += "ERROR: last reading was from "+String(hours)+" hrs, "+String(minutes)+" min, "+String(seconds)+" sec ago.<br/>";
        status += "ERROR: the system will be turned off till we get a new reading.<br/>";
        heat_on = false;
        cool_on = false;
        fan_on = false;
        status_err = true;
    }

    // Enable the fan if the user specified and not is already
    if(desired_fan && !fan_on) {
        status += "user has forced fan to be on.<br/>";
        fan_on = true;
    }

    // If we should turn on cooling
    if(heat_on) digitalWrite(PIN_HEAT,HIGH);
    else digitalWrite(PIN_HEAT,LOW);

    // If we should turn on cooling
    if(cool_on) digitalWrite(PIN_COOL,HIGH);
    else digitalWrite(PIN_COOL,LOW);
 
    // If we should put the fan on, then do that
    if(fan_on) digitalWrite(PIN_FAN,HIGH);
    else digitalWrite(PIN_FAN,LOW);

}

