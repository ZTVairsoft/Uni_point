void loop() {
  ui.tick();
  mem1.tick();
  mem2.tick();
  butt1.tick();
  butt2.tick();

  if (isFlagWiFi && millis() - holdingIP < 3000) {
    return;
  }

  if (millis() - voltTime > 5000) {  //проверка напряжения
    Volt();
  }

  switch (Settings.mode) {
    case 0:  //режим бомбы
      Bomb();
      break;
    case 1:  //режим захвата точки
      PointCapture();
      break;
    case 2:  //режим шахматного таймера
      ChessTimer();
      break;
    default:
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("ERROR MODE");
      break;
  }

  if (isMainScreenUse && millis() - lastMainScreenUpdate > SCREENUPDATE && !isBombActivated) {
    UpdateMainScreen();
    lastMainScreenUpdate = millis();
  }
}