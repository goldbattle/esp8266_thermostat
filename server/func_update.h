


void handle_update(AsyncWebServerRequest *request) {

    // Loop through all parameters
    int params = request->params();
    for(int i=0; i<params; i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->name()=="temp") desired_temp = p->value().toInt();
        if(p->name()=="device") desired_device = p->value().toInt();
        if(p->name()=="mode") desired_mode = p->value().toInt();
        if(p->name()=="fan") desired_fan = (bool)p->value().toInt();
        Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
    }

    // Update our pins
    handle_pin_updates();

    // Save to disk / eeprom
    save_to_eeprom();

    // Redirect back to homepage
    request->redirect("/");

}














