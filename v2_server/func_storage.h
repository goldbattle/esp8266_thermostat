
int eeprom_op_size_required() {
  int size_required = sizeof(is_ok);
  size_required += sizeof(config);
  size_required += sizeof(schedule_control);
  size_required += sizeof(not_reset);
  size_required += sizeof(stats_curr24);
  size_required += sizeof(stats_daily);
  return size_required;

}


void save_to_eeprom() {

  // Debug size of the data structures
  //Serial.println("sensor = "+String(sizeof(sensor)));
  //Serial.println("payload = "+String(sizeof(payload)));
  //Serial.println("daily_stats = "+String(sizeof(daily_stats)));
  //Serial.println("control = "+String(sizeof(control)));
  //Serial.println("var stats_daily = "+String(sizeof(stats_daily)));
  //Serial.println("var schedule_control = "+String(sizeof(schedule_control)));
  int size_required = eeprom_op_size_required();

  // Create a cache in our ram
  EEPROM.begin(size_required);

  // Replace our values in the cache
  int curr_addres = 0;
  EEPROM.put(curr_addres, is_ok);
  curr_addres += sizeof(is_ok);
  EEPROM.put(curr_addres, config);
  curr_addres += sizeof(config);
  EEPROM.put(curr_addres, schedule_control);
  curr_addres += sizeof(schedule_control);
  EEPROM.put(curr_addres, not_reset);
  curr_addres += sizeof(not_reset);
  EEPROM.put(curr_addres, stats_curr24);
  curr_addres += sizeof(stats_curr24);
  EEPROM.put(curr_addres, stats_daily);
  curr_addres += sizeof(stats_daily);

  // Actually commit our ram to eeprom
  EEPROM.commit();
  Serial.println("EEPROM: saved "+String(curr_addres)+" of "+String(SPI_FLASH_SEC_SIZE)+" possible bytes");

}


void clear_eeprom() {

  // Calculate size we need
  int size_required = eeprom_op_size_required();

  // Create a cache in our ram
  EEPROM.begin(size_required);

  // write a 0 to all 512 bytes of the EEPROM
  for (int i = 0; i < size_required; i++) {
      EEPROM.write(i, 0);
  }

  // Done commit it
  EEPROM.end();

}


void load_from_eeprom() {

  // Calculate size we need
  int size_required = eeprom_op_size_required();
  Serial.println("EEPROM: loading "+String(size_required)+" of "+String(SPI_FLASH_SEC_SIZE)+" possible bytes");

  // Create a cache in our ram
  EEPROM.begin(size_required);

  // Load it from our eeprom
  int curr_addres = 0;
  EEPROM.get(curr_addres, is_ok);
  curr_addres += sizeof(is_ok);

  // If we have loaded ok, then we can load the config
  // Otherwise we should just load the default config file
  if(is_ok[0] == 'o' && is_ok[1] == 'k') {
    EEPROM.get(curr_addres, config);
    curr_addres += sizeof(config);
    EEPROM.get(curr_addres, schedule_control);
    curr_addres += sizeof(schedule_control);
    EEPROM.get(curr_addres, not_reset);
    curr_addres += sizeof(not_reset);
    EEPROM.get(curr_addres, stats_curr24);
    curr_addres += sizeof(stats_curr24);
    EEPROM.get(curr_addres, stats_daily);
    curr_addres += sizeof(stats_daily);
  } else {
    is_ok[0] = 'o';
    is_ok[1] = 'k';
    clear_eeprom();
    save_to_eeprom();
  }

  // We are done with the eeprom
  EEPROM.end();

}
