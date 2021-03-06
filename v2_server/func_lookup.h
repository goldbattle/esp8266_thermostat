#define ITEM_INTEMP 0x01        //Indoor Temperature ( 2
#define ITEM_OUTTEMP 0x02       //Outdoor Temperature ( 2
#define ITEM_DEWPOINT 0x03      //Dew point ( 2
#define ITEM_WINDCHILL 0x04     //Wind chill ( 2
#define ITEM_HEATINDEX 0x05     //Heat index ( 2
#define ITEM_INHUMI 0x06        //Indoor Humidity (%) 1
#define ITEM_OUTHUMI 0x07       //Outdoor Humidity (%) 1
#define ITEM_ABSBARO 0x08       //Absolutely Barometric (hpa) 2
#define ITEM_RELBARO 0x09       //Relative Barometric (hpa) 2
#define ITEM_WINDDIRECTION 0x0A //Wind Direction (360°) 2
#define ITEM_WINDSPEED 0x0B     //Wind Speed (m/s) 2
#define ITEM_GUSTSPEED 0x0C     //Gust Speed (m/s) 2
#define ITEM_RAINEVENT 0x0D     //Rain Event (mm) 2
#define ITEM_RAINRATE 0x0E      //Rain Rate (mm/h) 2
#define ITEM_RAINHOUR 0x0F      //Rain hour (mm) 2
#define ITEM_RAINDAY 0x10       //Rain Day (mm) 2
#define ITEM_RAINWEEK 0x11      //Rain Week (mm) 2
#define ITEM_RAINMONTH 0x12     //Rain Month (mm) 4
#define ITEM_RAINYEAR 0x13      //Rain Year (mm) 4
#define ITEM_RAINTOTALS 0x14    //Rain Totals (mm) 4
#define ITEM_LIGHT 0x15         //Light (lux) 4
#define ITEM_UV 0x16            //UV (uW/m2) 2
#define ITEM_UVI 0x17           //UVI (0 15 index) 1
#define ITEM_TIME 0x18          //Date and time 6
#define ITEM_DAYLWINDMAX 0X19   //Day max wind(m/s) 2

#define ITEM_TEMP1 0x1A         //Temperature 1( 2
#define ITEM_TEMP2 0x1B         //Temperature 2( 2
#define ITEM_TEMP3 0x1C         //Temperature 3( 2
#define ITEM_TEMP4 0x1D         //Temperature 4( 2
#define ITEM_TEMP5 0x1E         //Temperature 5( 2
#define ITEM_TEMP6 0x1F         //Temperature 6( 2
#define ITEM_TEMP7 0x20         //Temperature 7( 2
#define ITEM_TEMP8 0x21         //Temperature 8( 2
#define ITEM_HUMI1 0x22         //Humidity 1, 0 100% 1
#define ITEM_HUMI2 0x23         //Humidity 2, 0 100% 1
#define ITEM_HUMI3 0x24         //Humidity 3, 0 100% 1
#define ITEM_HUMI4 0x25         //Humidity 4, 0 100% 1
#define ITEM_HUMI5 0x26         //Humidity 5, 0 100% 1
#define ITEM_HUMI6 0x27         //Humidity 6, 0 100% 1
#define ITEM_HUMI7 0x28         //Humidity 7, 0 100% 1
#define ITEM_HUMI8 0x29         //Humidity 8, 0 100% 1

#define ITEM_PM25_CH1 0x2A       //PM2.5 Air Quality Sensor(μg/m3) 2
#define ITEM_SOILTEMP1 0x2B      //Soil Temperature 2
#define ITEM_SOILMOISTURE1 0x2C  //Soil Moisture(%) 1
#define ITEM_SOILTEMP2 0x2D      //Soil Temperature( 2
#define ITEM_SOILMOISTURE2 0x2E  //Soil Moisture(%) 1
#define ITEM_SOILTEMP3 0x2F      //Soil Temperature( 2
#define ITEM_SOILMOISTURE3 0x30  //Soil Moisture(%) 1
#define ITEM_SOILTEMP4 0x31      //Soil Temperature( 2
#define ITEM_SOILMOISTURE4 0x32  //Soil Moisture(%) 1
#define ITEM_SOILTEMP5 0x33      //Soil Temperature( 2
#define ITEM_SOILMOISTURE5 0x34  //Soil Moisture(%) 1
#define ITEM_SOILTEMP6 0x35      //Soil Temperature( 2
#define ITEM_SOILMOISTURE6 0x36  //Soil Moisture(%) 1
#define ITEM_SOILTEMP7 0x37      //Soil Temperature( 2
#define ITEM_SOILMOISTURE7 0x38  //Soil Moisture(%) 1
#define ITEM_SOILTEMP8 0x39      //Soil Temperature( 2
#define ITEM_SOILMOISTURE8 0x3A  //Soil Moisture(%) 1
#define ITEM_SOILTEMP9 0x3B      //Soil Temperature( 2
#define ITEM_SOILMOISTURE9 0x3C  //Soil Moisture(%) 1
#define ITEM_SOILTEMP10 0x3D      //Soil Temperature( 2
#define ITEM_SOILMOISTURE10 0x3E//Soil Moisture(%) 1
#define ITEM_SOILTEMP11 0x3F      //Soil Temperature( 2
#define ITEM_SOILMOISTURE11 0x40  //Soil Moisture(%) 1
#define ITEM_SOILTEMP12 0x41      //Soil Temperature( 2
#define ITEM_SOILMOISTURE12 0x42  //Soil Moisture(%) 1
#define ITEM_SOILTEMP13 0x43      //Soil Temperature( 2
#define ITEM_SOILMOISTURE13 0x44  //Soil Moisture(%) 1
#define ITEM_SOILTEMP14 0x45      //Soil Temperature( 2
#define ITEM_SOILMOISTURE14 0x46  //Soil Moisture(%) 1
#define ITEM_SOILTEMP15 0x47      //Soil Temperature( 2
#define ITEM_SOILMOISTURE15 0x48  //Soil Moisture(%) 1
#define ITEM_SOILTEMP16 0x49      //Soil Temperature( 2
#define ITEM_SOILMOISTURE16 0x4A  //Soil Moisture(%) 1

#define ITEM_LOWBATT 0x4C          //All sensor lowbatt 16 char 16
#define ITEM_PM25_24HAVG1 0x4D     //for pm25_ch1 2
#define ITEM_PM25_24HAVG2 0x4E     //for pm25_ch2 2
#define ITEM_PM25_24HAVG3 0x4F     //for pm25_ch3 2
#define ITEM_PM25_24HAVG4 0x50     //for pm25_ch4 2
#define ITEM_PM25_CH2 0x51         //PM2.5 Air Quality Sensor(μg/m3) 2
#define ITEM_PM25_CH3 0x52         //PM2.5 Air Quality Sensor(μg/m3) 2
#define ITEM_PM25_CH4 0x53         //PM2.5 Air Quality Sensor(μg/m3) 2
#define ITEM_LEAK_CH1 0x58         //for Leak_ch1 1
#define ITEM_LEAK_CH2 0x59         //for Leak_ch2 1
#define ITEM_LEAK_CH3 0x5A         //for Leak_ch3 1
#define ITEM_LEAK_CH4 0x5B         //for Leak_ch4 1
#define ITEM_LIGHTNING 0x60        //Lightning distance 1~40KM 1
#define ITEM_LIGHTNING_TIME 0x61   //Lightning detection time (UTC) 4
#define ITEM_LIGHTNING_POWER 0x62  //Strikes per day4

#define ITEM_TF_USR1 0x63          //Temperature( 2
#define ITEM_TF_USR2 0x64          //Temperature( 2
#define ITEM_TF_USR3 0x65          //Temperature( 2
#define ITEM_TF_USR4 0x66          //Temperature( 2
#define ITEM_TF_USR5 0x67          //Temperature( 2
#define ITEM_TF_USR6 0x68          //Temperature( 2
#define ITEM_TF_USR7 0x69          //Temperature( 2
#define ITEM_TF_USR8 0x6A          //Temperature( 2
#define ITEM_TF_BATT 0x6B          //tf temperature batt 8


/**
 * This will query our gw1000 sensor and try to get the latest data
 * We will return the current payload for the system to use.
 * Fastest poll rate should be no more then 60 seconds.
 */
void get_latest_payload(const char* host, const int port) {

  // Reset all to be "offline"
  for(int i=0; i<NUM_SENSORS; i++) {
    last_payload.sensors[i].has_value = false;
    last_payload.sensors[i].temp = 0;
    last_payload.sensors[i].humid = 0;
  }
  
  // try to connect
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("unable to connect to server:");
    Serial.println("\t- host = "+String(host));
    Serial.println("\t- port = "+String(port));
    return;
  }

  // Send the command CMD_GW1000_LIVE_DATA to the local network device
  // HEADER (2 bytes), CMD (1 byte), SIZE (1 byte), DATA (n bytes), CHECKSUM (1 byte)
  // HEADER = always fixed with 0xffff as value
  // SIZE = total size from CMD to CHECKSUM
  // CHECKSUM = CMD + SIZE + DATA1 + DATA2 + ... + DATAn
  uint8_t cmd[] = {0xff,0xff,0x27,0x03,0x2a};
  client.write(cmd, 5);

  // Read the response resulting bytes of live data
  char buf[1024];
  int sz = client.readBytes(buf, 1024);
  for(int i=0; i<sz; i++){

    // skip the first 5 which are the fixed header, command, and size
    if(i < 5) {
      continue;
    }

    // indoor gw1000 internal
    if(buf[i] == ITEM_INTEMP && (i+2) < sz) {
      last_payload.sensors[0].has_value = true;
      last_payload.sensors[0].temp = ((buf[i+1] & 0xff) << 8) | (buf[i+2] & 0xff);
      //Serial.printf("(found temp0 %d)\n",last_payload.sensor_0.temp);
      i += 2;
    } else if(buf[i] == ITEM_INHUMI && (i+1) < sz) {
      last_payload.sensors[0].has_value = true;
      last_payload.sensors[0].humid = buf[i+1];
      //Serial.printf("(found humid0 %d)\n",last_payload.sensor_0.humid);
      i += 1;
    }
    
    // multi-sensors
    else if(buf[i] == ITEM_TEMP1 && (i+2) < sz) {
      last_payload.sensors[1].has_value = true;
      last_payload.sensors[1].temp = ((buf[i+1] & 0xff) << 8) | (buf[i+2] & 0xff);
      //Serial.printf("(found temp1 %d)\n",last_payload.sensor_1.temp);
      i += 2;
    } else if(buf[i] == ITEM_HUMI1 && (i+1) < sz) {
      last_payload.sensors[1].has_value = true;
      last_payload.sensors[1].humid = buf[i+1];
      //Serial.printf("(found humid1 %d)\n",last_payload.sensor_1.humid);
      i += 1;
    } else if(buf[i] == ITEM_TEMP2 && (i+2) < sz) {
      last_payload.sensors[2].has_value = true;
      last_payload.sensors[2].temp = ((buf[i+1] & 0xff) << 8) | (buf[i+2] & 0xff);
      //Serial.printf("(found temp2 %d)\n",last_payload.sensor_2.temp);
      i += 2;
    } else if(buf[i] == ITEM_HUMI2 && (i+1) < sz) {
      last_payload.sensors[2].has_value = true;
      last_payload.sensors[2].humid = buf[i+1];
      //Serial.printf("(found humid2 %d)\n",last_payload.sensor_2.humid);
      i += 1;
    } else if(buf[i] == ITEM_TEMP3 && (i+2) < sz) {
      last_payload.sensors[3].has_value = true;
      last_payload.sensors[3].temp = ((buf[i+1] & 0xff) << 8) | (buf[i+2] & 0xff);
      //Serial.printf("(found temp3 %d)\n",last_payload.sensor_3.temp);
      i += 2;
    } else if(buf[i] == ITEM_HUMI3 && (i+1) < sz) {
      last_payload.sensors[3].has_value = true;
      last_payload.sensors[3].humid = buf[i+1];
      //Serial.printf("(found humid3 %d)\n",last_payload.sensor_3.humid);
      i += 1;
    } else if(buf[i] == ITEM_TEMP4 && (i+2) < sz) {
      last_payload.sensors[4].has_value = true;
      last_payload.sensors[4].temp = ((buf[i+1] & 0xff) << 8) | (buf[i+2] & 0xff);
      //Serial.printf("(found temp4 %d)\n",last_payload.sensor_4.temp);
      i += 2;
    } else if(buf[i] == ITEM_HUMI4 && (i+1) < sz) {
      last_payload.sensors[4].has_value = true;
      last_payload.sensors[4].humid = buf[i+1];
      //Serial.printf("(found humid4 %d)\n",last_payload.sensor_4.humid);
      i += 1;
    } else if(buf[i] == ITEM_TEMP5 && (i+2) < sz) {
      last_payload.sensors[5].has_value = true;
      last_payload.sensors[5].temp = ((buf[i+1] & 0xff) << 8) | (buf[i+2] & 0xff);
      i += 2;
    } else if(buf[i] == ITEM_HUMI5 && (i+1) < sz) {
      last_payload.sensors[5].has_value = true;
      last_payload.sensors[5].humid = buf[i+1];
      //Serial.printf("(found humid5 %d)\n",last_payload.sensor_5.humid);
      i += 1;
    } else if(buf[i] == ITEM_TEMP6 && (i+2) < sz) {
      last_payload.sensors[6].has_value = true;
      last_payload.sensors[6].temp = ((buf[i+1] & 0xff) << 8) | (buf[i+2] & 0xff);
      //Serial.printf("(found temp6 %d)\n",last_payload.sensor_6.temp);
      i += 2;
    } else if(buf[i] == ITEM_HUMI6 && (i+1) < sz) {
      last_payload.sensors[6].has_value = true;
      last_payload.sensors[6].humid = buf[i+1];
      //Serial.printf("(found humid6 %d)\n",last_payload.sensor_6.humid);
      i += 1;
    } else if(buf[i] == ITEM_TEMP7 && (i+2) < sz) {
      last_payload.sensors[7].has_value = true;
      last_payload.sensors[7].temp = ((buf[i+1] & 0xff) << 8) | (buf[i+2] & 0xff);
      //Serial.printf("(found temp7 %d)\n",last_payload.sensor_7.temp);
      i += 2;
    } else if(buf[i] == ITEM_HUMI7 && (i+1) < sz) {
      last_payload.sensors[7].has_value = true;
      last_payload.sensors[7].humid = buf[i+1];
      //Serial.printf("(found humid7 %d)\n",last_payload.sensor_7.humid);
      i += 1;
    } else if(buf[i] == ITEM_TEMP8 && (i+2) < sz) {
      last_payload.sensors[8].has_value = true;
      last_payload.sensors[8].temp = ((buf[i+1] & 0xff) << 8) | (buf[i+2] & 0xff);
      //Serial.printf("(found temp8 %d)\n",last_payload.sensor_8.temp);
      i += 2;
    } else if(buf[i] == ITEM_HUMI5 && (i+1) < sz) {
      last_payload.sensors[8].has_value = true;
      last_payload.sensors[8].humid = buf[i+1];
      //Serial.printf("(found humid8 %d)\n",last_payload.sensor_8.humid);
      i += 1;
    }
    
    // Ignore some that we don't care about
    else if ((buf[i] == ITEM_ABSBARO && (i+2) < sz) || (buf[i] == ITEM_RELBARO && (i+2) < sz)) {
      i += 2;
    }
    else if (buf[i] == ITEM_LOWBATT && (i+16) < sz) {
      i += 16;
    }
    
    // Else error to the user
    else {
      Serial.printf("unknown raw byte -> %02x\n",buf[i]);
    }    

  }

  // time we recorded this data
  last_payload_time = time(nullptr);
  
}
