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
  
  // Get current time
  time_t timestamp_now = time(nullptr);
  struct tm *timestamp_local = localtime(&timestamp_now);

  // Get the total minutes we have had the unit on for
  int on_minutes = (int)(difftime(timestamp_now,last_control_time) / 60.0);

  // Append to our statistics object
  if(fan_on) stats_curr24.fan_mins += on_minutes;
  if(heat_on) stats_curr24.heat_mins += on_minutes;
  if(cool_on) stats_curr24.cool_mins += on_minutes;

  // Record the last time we did pin control
  last_control_time = timestamp_now;

  // Record the last control signal
  bool fan_on_last = fan_on;
  bool heat_on_last = heat_on;
  bool cool_on_last = cool_on;

  //===================================================
  //===================================================

  // Reset our status message
  status = "";
  status_err = false;
  fan_on = false;
  heat_on = false;
  cool_on = false;

  // Check if we have not passed an hour of our override
  control current_control;
  if((int)difftime(timestamp_now,override_time) < 3600) {
    current_control = override_control;
    status += "override has been active for "+String((int)(difftime(timestamp_now,override_time)/60.0))+" minutes<br/>";
  } else {
    current_control = schedule_control[timestamp_local->tm_hour];
  }
  
  // Get the current temp of the system
  float c_temp = (last_payload.sensors[current_control.desired_device].temp/10.0) * 9.0/5.0 + 32;
  bool sensor_online = last_payload.sensors[current_control.desired_device].has_value;
  
  // Sum the total average temp over our sliding window
  float c_temp_sum = c_temp;
  if(sensor_online) {

    // Do the summing
    for(int i=0; i<last_control_temp_ct; i++) {
      c_temp_sum += last_control_temp[i];
    }
    c_temp_sum /= (last_control_temp_ct+1);

    // increase the count if we need to
    if(last_control_temp_ct < config.sliding_window_size) {
      last_control_temp_ct++;
    }
  
    // "Slide" the window
    for(int i=last_control_temp_ct-1; i>0; i--) {
      last_control_temp[i] = last_control_temp[i-1];
    }
    last_control_temp[0] = c_temp;
    
  }

  // If we are cooling, then check if we should use it
  if(current_control.desired_mode==0) {
      status += "system mode if set to off, doing nothing<br/>";
  }
  
  // If we are cooling, then check if we should use it
  if(current_control.desired_mode==1 && c_temp_sum > current_control.desired_temp-((int)cool_on_last)*config.hysteresis) {
    status += "current temp of "+String(c_temp_sum)+"&degF > "+String(current_control.desired_temp);
    status += (cool_on_last)? "-"+String(round(config.hysteresis))+"&degF<br/>" : "&degF<br/>";
    status += "turning cooling on and fan to move air<br/>";
    cool_on = true;
    fan_on = true;
  } else if(current_control.desired_mode==1) {
    status += "current temp of "+String(c_temp_sum)+"&degF < "+String(current_control.desired_temp)+"&degF<br/>";
    status += "no need to turn on cooling<br/>";
  }

  // If we are heating, then check if we should use it
  if(current_control.desired_mode==2 && c_temp_sum < current_control.desired_temp+((int)heat_on_last)*config.hysteresis) {
    status += "current temp of "+String(c_temp_sum)+"&degF < "+String(current_control.desired_temp);
    status += (heat_on_last)? "+"+String(round(config.hysteresis))+"&degF<br/>" : "&degF<br/>";
    status += "turning heating on and fan to move air<br/>";
    heat_on = true;
    fan_on = true;
  } else if(current_control.desired_mode==2) {
    status += "current temp of "+String(c_temp_sum)+"&degF > "+String(current_control.desired_temp)+"&degF<br/>";
    status += "no need to turn on heating<br/>";
  }

  // Enable the fan if the user specified and not is already
  if(current_control.desired_fan && !fan_on) {
    status += "user has forced the fan to be on.<br/>";
    fan_on = true;
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
  int time_diff = (int)difftime(timestamp_now,last_payload_time);
  if(!sensor_online || time_diff>120*config.freq_device_poll) {
    int seconds = (time_diff % 60);
    int minutes = (time_diff % 3600) / 60;
    int hours = (time_diff % 86400) / 3600;
    status = "ERROR: have not got any sensor readings from <b>"+String(config.name_sensors[current_control.desired_device])+"</b>.<br/>";
    status += "ERROR: last checked for data "+String(hours)+" hrs, "+String(minutes)+" min, "+String(seconds)+" sec ago.<br/>";
    status += "ERROR: the system will be turned off till we get a new reading.<br/>";
    heat_on = false;
    cool_on = false;
    fan_on = false;
    status_err = true;
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
