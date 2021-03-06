
/**
 * This is the data for a single sensor.
 * We also have if the data is valid and loaded.
 * TEMP: in units 10*celsius
 * HUMID: % humidity from 0 to 100
 */
typedef struct sensor {
  bool has_value = false;
  short int temp = 0;
  short int humid = 0;
};


/**
 * This is the primary data structure for all information from the sensor.
 * Everything that we can read will try to be loaded into this structure.
 */
#define NUM_SENSORS 6
typedef struct payload {

  // indoor gw1000 internal
  // sensor1 ... sensor8
  sensor sensors[NUM_SENSORS];
  
};


/**
 * Statistics for the control pin activations.
 * In this case, we wish to count the number of minutes each one is enabled.
 * We also have the max and min sensor values for the current day
 */
typedef struct daily_stats {
  
  // max sensor values for today
  payload payload_max;
  
  // min sensor values for today
  payload payload_min;

  // Day of the month, month, and years since 1900
  short int day = 0;
  short int month = 0;
  short int year_since_1900 = 0;

  // current pin minute count
  short int cool_mins = 0;
  short int heat_mins = 0;
  short int fan_mins = 0;

  // function to reset the min max values
  void reset() {
    cool_mins = 0;
    heat_mins = 0;
    fan_mins = 0;
    for(int i=0; i<NUM_SENSORS; i++) {
      payload_max.sensors[i].temp = -1e3;
      payload_max.sensors[i].humid = -1e3;
      payload_min.sensors[i].temp = 1e3;
      payload_min.sensors[i].humid = 1e3;
    }
  }

  // update with newest values
  void update_minmax(payload last_payload) {
    for(int i=0; i<NUM_SENSORS; i++) {
      if(!last_payload.sensors[i].has_value)
        continue;
      payload_max.sensors[i].temp = max(payload_max.sensors[i].temp,last_payload.sensors[i].temp);
      payload_max.sensors[i].humid = max(payload_max.sensors[i].humid,last_payload.sensors[i].humid);
      payload_min.sensors[i].temp = min(payload_min.sensors[i].temp,last_payload.sensors[i].temp);
      payload_min.sensors[i].humid = min(payload_min.sensors[i].humid,last_payload.sensors[i].humid);
    }
  }

};



/**
 * This has what the system control should be performing.
 * MODE: 0:off, 1:cool, 2:heat
 * DEVICE: what id for the device we will use
 * TEMP: what temp we want to achieve
 * FAN: if the fan should be forced on
 */
typedef struct control {  
  short int desired_mode = 0; // 0:off, 1:cool, 2:heat
  short int desired_device = 0; // what device
  short int desired_temp = 70;
  bool desired_fan = false;
};
