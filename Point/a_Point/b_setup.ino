void setup() {
  Serial.begin(115200);

  butt1.setDebounce(50);  // настройка антидребезга (по умолчанию 80 мс)
  butt1.setTimeout(300);  // настройка таймаута на удержание (по умолчанию 500 мс)

  EEPROM.begin(512);

  mem1.begin(0, 'a');
  mem2.begin(mem1.nextAddr(), 'a');

  butt1.setType(HIGH_PULL);
  butt2.setType(HIGH_PULL);

  lcd.init();  // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();

  lcd.setCursor(6, 0);
  lcd.print("ZTV");
  lcd.setCursor(4, 1);
  lcd.print("Airsoft");
  delay(2000);
  lcd.clear();

  if (digitalRead(BUTTONGREEN) == LOW) {  //кнопка нажата при включении
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(WF.WF_SSID, WF.WF_PASS);
    isFlagWiFi = true;  //поднимаем флаг для удержания надписи на экране в loop
    holdingIP = millis();
    Serial.print("AP: ");
    Serial.println(WiFi.softAPIP());
    lcd.setCursor(0, 0);
    lcd.print(WF.WF_SSID);
    lcd.print(" ");
    lcd.print(WF.WF_PASS);
    lcd.setCursor(0, 1);
    lcd.print(WiFi.softAPIP());
  } else if (digitalRead(BUTTONRED) == LOW) {
    mem1.reset();  // Сброс настроек
    Serial.println("Сброс");
    lcd.clear();
    lcd.setCursor (2, 0);
    lcd.print ("MEMORY RESET");
    lcd.setCursor (1, 1);
    lcd.print ("RELEASE BUTTON");
    while (digitalRead(BUTTONRED) == HIGH) {
      delay(1);
    }
    lcd.clear();
    lcd.setCursor (5, 0);
    lcd.print ("REBOOT");
    ESP.restart();
  } else {
    WiFi.mode(WIFI_OFF);
  }

  ui.attachBuild(build);
  ui.attach(action);
  ui.start();
  ui.enableOTA();

  if (!LittleFS.begin()) Serial.println("FS Error");
  ui.downloadAuto(true);

  setVolume(30);

  pinMode(SIREN, OUTPUT);
}