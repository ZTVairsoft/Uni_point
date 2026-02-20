void Bomb() {
  handleButtonPress();     //проверка нажатия и отпускания кнопки
  handleBombActivation();  //проверка активации бомбы
  handleBombDefusal();
  handleBombExplosion();
}

void handleButtonPress() {  //проверка удержания кнопки
  if (butt1.isHolded() || butt2.isHolded()) {
    holdStartTime = millis();  //Запоминаем время начала удержания кнопки
    isMainScreenUse = false;   //отключаем показ главного экрана
    lcd.clear();
  }

  if (butt1.isRelease() || butt2.isRelease()) {  //Если кнопка отпущена то сбрасываем счетчики
    holdStartTime = 0;
    defuseHoldStartTime = 0;
    isMainScreenUse = true;  //включаем показ главного экрана
    noTone(ZOOM);
    return;
  }
}

void handleBombActivation() {
  if ((butt1.isHold() || butt2.isHold()) && !isBombActivated) {         //если кнопка удерживается и бомба не активирована
    if (millis() - holdStartTime >= Settings.holdTime * SECTOMILLIS) {  //и если начало времени удержания кнопки больше чем время необходимого удержания
      activateBomb();                                                   //переходим в активацию бомбы
    } else {
      handleCountdown();  //иначе пищим через короткие промежутки
    }
  }
}

void activateBomb() {            //активация бомбы
  isBombActivated = true;        //переключаем в активированна
  bombActivatedTime = millis();  //запоминаем время активации бомбы
  holdStartTime = 0;             //сбрасываем счетчик удержания кнопки
  noTone(ZOOM);                  //отключаем пищалку
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(Settings.ACTIVATED);
}

void handleCountdown() {
  if (millis() - lastBeepTime >= BEEPINTERVAL) {  //прошел интервал между писками
    tone(ZOOM, 1000, 100);                        //пищим
    displayRemainingTime();                       //выводим на экран счетчик
    lastBeepTime = millis();                      //запоминаем последний писк
  }
}

void displayRemainingTime() {                                                                  //оставшееся время удержания кнопки
  unsigned long currentMillis = (holdStartTime + Settings.holdTime * SECTOMILLIS) - millis();  //вычисляем сколько millis осталось
  unsigned long seconds = currentMillis / 1000;                                                //
  unsigned long minutes = seconds / 60;
  seconds = seconds % 60;

  lcd.setCursor(0, 0);
  lcd.print(Settings.ACTIVATING);
  lcd.setCursor(0, 1);  //обратный отсчет выводим на экран сколько осталось удерживать кнопку
  if (minutes < 10) {
    lcd.print("0");
  }
  lcd.print(minutes);
  lcd.print(":");
  if (seconds < 10) {
    lcd.print("0");
  }
  lcd.print(seconds);
}

void handleBombDefusal() {                                      //отработка обезвреживания
  if (isBombActivated && (butt1.isHold() || butt2.isHold())) {  //если кнопка нажата и бомба активирована
    if (defuseHoldStartTime == 0) {                             //если запускается в первый раз
      defuseHoldStartTime = millis();                           //устанавливаем время начала
      lcd.clear();                                              //очищаем экран
      lcd.setCursor(0, 0);
      lcd.print(Settings.DEFUSING);  //выводим сообщение о разминировании
    }

    unsigned long currentMillis = (defuseHoldStartTime + Settings.defuseHoldTime * SECTOMILLIS) - millis();  //вычисляем сколько millis осталось
    unsigned long seconds = currentMillis / 1000;                                                            //
    unsigned long minutes = seconds / 60;
    seconds = seconds % 60;

    lcd.setCursor(0, 1);  //выводим оставшееся время на второй строке экрана
    if (minutes < 10) {
      lcd.print("0");
    }
    lcd.print(minutes);
    lcd.print(":");
    if (seconds < 10) {
      lcd.print("0");
    }
    lcd.print(seconds);

    if (millis() - defuseHoldStartTime >= Settings.defuseHoldTime * SECTOMILLIS) {  //если время удержания вышло
      defuseBomb();                                                                 //переходим в функцию обезвреживания
    }
  }
}

void defuseBomb() {                                            //если бомба обезвреджена
  noTone(ZOOM);                                                //отключаем звук
  isBombActivated = false;                                     //переключаем в неактивированна
  unsigned long currentMillis = millis() - bombActivatedTime;  //вычисляем сколько времени она была захвачена
  unsigned long seconds = currentMillis / 1000;
  unsigned long minutes = seconds / 60;
  seconds = seconds % 60;

  lcd.clear();  //чистим экран и выводим информацию
  lcd.setCursor(0, 0);
  lcd.print(Settings.DEFUSE);
  lcd.setCursor(11, 1);
  if (minutes < 10) {
    lcd.print("0");
  }
  lcd.print(minutes);
  lcd.print(":");
  if (seconds < 10) {
    lcd.print("0");
  }
  lcd.print(seconds);
  enterDeepSleep();  //и уходим в сон
}

void handleBombExplosion() {  //отрабатываем взрыв бомбы
  if (isBombActivated) {
    unsigned long remainingTime = Settings.bombTimerMin * MINTOMILLIS - (millis() - bombActivatedTime);  //вычисляем сколько ей еще осталось до взрыва

    if (remainingTime > Settings.bombTimerMin * MINTOMILLIS) {
      remainingTime = 0;  // Если время истекло, устанавливаем remainingTime в 0
    }

    unsigned long currentMillis = remainingTime;  //вычисляем сколько времени она была захвачена
    unsigned long seconds = currentMillis / 1000;
    unsigned long minutes = seconds / 60;
    seconds = seconds % 60;

    lcd.setCursor(0, 0);
    if (defuseHoldStartTime > 0) {
      lcd.print(Settings.DEFUSING);
      lcd.print("                ");
    } else {
      lcd.print(Settings.ACTIVATED);
      lcd.print("                ");
    }
    lcd.setCursor(0, 1);
    if (minutes < 10) {
      lcd.print("0");
    }
    lcd.print(minutes);
    lcd.print(":");
    if (seconds < 10) {
      lcd.print("0");
    }
    lcd.print(seconds);

    if (remainingTime <= ACCELERATIONTIME) {                                     //если оставшееся время меньше или равно времени начала ускорения писка, то
      currentBeepInterval = map(remainingTime, ACCELERATIONTIME, 0, 1000, 100);  //задаем увеличение частоты писка
    }

    if (remainingTime <= 0) {  //если время вышло
      if (millis() - bombActivatedTime - Settings.bombTimerMin * MINTOMILLIS <= Settings.finalBeepTime * SECTOMILLIS) {
        tone(ZOOM, 1000, 100);  //включаем писк на указанное время
      } else {
        explodeBomb();  //если время финального писка прошло то переходим в подрыв бомбы
      }
    } else {                                                                                     //если время не вышло, то
      if (millis() - lastBeepTime >= currentBeepInterval && Settings.isHoldPointNotification) {  //если бипы все не не до конца сделаны
        tone(ZOOM, 1000, 100);                                                                   //пишим по старой схеме
        lastBeepTime = millis();                                                                 //запоминаем время последнего бипа
      }
    }
  }
}

void explodeBomb() {        //подрыв
  isBombActivated = false;  //выключаем бомбу
  noTone(ZOOM);             //отключаем звук
  lcd.clear();              //чистим экран
  lcd.setCursor(0, 0);
  lcd.print(Settings.EXPLODED);  //выводим что бомба взорвана
  lcd.setCursor(1, 1);
  lcd.print(Settings.TWIN);
  digitalWrite(SIREN, HIGH);
  delay(Settings.finalBeepTime * SECTOMILLIS);
  digitalWrite(SIREN, LOW);
  enterDeepSleep();  //уходим спать
}
//------------------------------------------------------------РЕЖИМ ЗАХВАТА ТОЧКИ

void PointCapture() {
  //отработка первого нажатия
  if (butt1.isHolded() || butt2.isHolded()) {  //если одна из кнопок зажата
    isMainScreenUse = false;                   //отключаем основной экран
    captureStartTime = millis();               //запоминаем начало отсчета
    lcd.clear();                               //очищаем дисплей
  }

  if (!isMainScreenUse) {
    UpdateScreen();
  }

  if (butt1.isHold() || butt2.isHold()) {
    if (millis() - lastBeepTime > CAPTURE_BEEP_INTERVAL) {
      tone(ZOOM, CAPTURE_BEEP_FREQ, CAPTURE_BEEP_DURATION);  // Писк при захвате
      lastBeepTime = millis();
    }
  }

  // Обработка звуковых сигналов при удержании точки
  if (flagState == GREENCAPTURED && Settings.isHoldPointNotification || flagState == REDCAPTURED && Settings.isHoldPointNotification) {
    if (millis() - lastBeepTime > HOLD_BEEP_INTERVAL) {
      tone(ZOOM, HOLD_BEEP_FREQ, HOLD_BEEP_DURATION);  // Писк при удержании
      lastBeepTime = millis();
    }
  }

  //обработка переключения режимов
  if (butt1.isHold()) {
    if (flagState != GREENCAPTURED) {
      if (millis() - captureStartTime >= Settings.holdTime * SECTOMILLIS) {
        if (flagState == REDCAPTURED) {
          flagState = NEUTRAL;
          if (!Settings.isCumulativeCounter) {
            redHoldTime = 0;  // Сброс времени удержания красной команды
          }
        } else {
          flagState = GREENCAPTURED;
        }
        if (flagState == GREENCAPTURED) {
          lastGreenCaptureTime = millis();
        } else if (flagState == NEUTRAL && Settings.isCumulativeCounter) {
          redHoldTime += millis() - lastRedCaptureTime;
        }
        captureStartTime = millis();
      }
    }
  } else if (butt2.isHold()) {
    if (flagState != REDCAPTURED) {
      if (millis() - captureStartTime >= Settings.holdTime * SECTOMILLIS) {
        if (flagState == GREENCAPTURED) {
          flagState = NEUTRAL;
          if (!Settings.isCumulativeCounter) {
            greenHoldTime = 0;  // Сброс времени удержания зеленой команды
          }
        } else {
          flagState = REDCAPTURED;
        }
        if (flagState == REDCAPTURED) {
          lastRedCaptureTime = millis();
        } else if (flagState == NEUTRAL && Settings.isCumulativeCounter) {
          greenHoldTime += millis() - lastGreenCaptureTime;
        }
        captureStartTime = millis();
      }
    }
  }

  // Обновление времени удержания в реальном времени
  if (flagState == GREENCAPTURED) {
    currentGreenHoldTime = millis() - lastGreenCaptureTime + greenHoldTime;
    if (currentGreenHoldTime >= Settings.bombTimerMin * MINTOMILLIS) {
      CaptureWin();
    }
  } else if (flagState == REDCAPTURED) {
    currentRedHoldTime = millis() - lastRedCaptureTime + redHoldTime;
    if (currentRedHoldTime >= Settings.bombTimerMin * MINTOMILLIS) {
      CaptureWin();
    }
  }
}


void UpdateScreen() {
  if (millis() - lastPrintTime >= 1000) {
    lastPrintTime = millis();

    // Проверка на изменение состояния флага
    if (flagState != prevFlagState) {
      lcd.clear();
      prevFlagState = flagState;
    }

    // Проверка на изменение времени удержания
    if (currentGreenHoldTime != prevGreenTime || currentRedHoldTime != prevRedTime) {
      prevGreenTime = currentGreenHoldTime;
      prevRedTime = currentRedHoldTime;
      lcd.clear();
    }
    int i;
    switch (flagState) {
      case GREENCAPTURED:
        lcd.setCursor(0, 0);
        lcd.print(Settings.TEAM1);
        lcd.print(" ");
        lcd.print(Settings.HOLDING);
        lcd.setCursor(0, 1);
        lcd.print(Settings.T1);
        lcd.print(formatTime((Settings.bombTimerMin * MINTOMILLIS) - currentGreenHoldTime));
        i = 7;
        while (i-- > 0) {
          lcd.print(" ");
        }
        if (Settings.isCumulativeCounter) {  // Если включен накопительный счетчик
          lcd.setCursor(9, 1);
          lcd.print(Settings.T2);
          lcd.print(formatTime((Settings.bombTimerMin * MINTOMILLIS) - currentRedHoldTime));
        }
        break;
      case NEUTRAL:
        lcd.setCursor(0, 0);
        lcd.print(Settings.NETRAL);
        if (!Settings.isCumulativeCounter) {
          lcd.setCursor(0, 1);
          i = 16;
          while (i-- > 0) {
            lcd.print(" ");
          }
        }
        if (Settings.isCumulativeCounter) {  // Если включен накопительный счетчик
          lcd.setCursor(0, 1);
          lcd.print(Settings.T1);
          lcd.print(formatTime((Settings.bombTimerMin * MINTOMILLIS) - currentGreenHoldTime));
          lcd.setCursor(9, 1);
          lcd.print(Settings.T2);
          lcd.print(formatTime((Settings.bombTimerMin * MINTOMILLIS) - currentRedHoldTime));
        }
        break;
      case REDCAPTURED:
        lcd.setCursor(0, 0);
        lcd.print(Settings.TEAM2);
        lcd.print(" ");
        lcd.print(Settings.HOLDING);
        lcd.setCursor(0, 1);
        if (Settings.isCumulativeCounter) {  // Если включен накопительный счетчик
          lcd.print(Settings.T1);
          lcd.print(formatTime((Settings.bombTimerMin * MINTOMILLIS) - currentGreenHoldTime));
        } else {
          i = 9;
          while (i-- > 0) {
            lcd.print(" ");
          }
        }
        lcd.setCursor(9, 1);
        lcd.print(Settings.T2);
        lcd.print(formatTime((Settings.bombTimerMin * MINTOMILLIS) - currentRedHoldTime));
        break;
    }
  }
}

void CaptureWin() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(flagState == GREENCAPTURED ? Settings.TEAM1 : Settings.TEAM2);
  lcd.print(Settings.WIN);
  lcd.setCursor(0, 1);
  lcd.print(Settings.T1);
  lcd.print(formatTime(currentGreenHoldTime));
  lcd.setCursor(9, 1);
  lcd.print(Settings.T2);
  lcd.print(formatTime(currentRedHoldTime));
  digitalWrite(SIREN, HIGH);
  delay(Settings.finalBeepTime * SECTOMILLIS);
  digitalWrite(SIREN, LOW);
  enterDeepSleep();
}

void UpdateMainScreen() {
  if (procVol != prevProcVol) {
    lcd.clear();
    prevProcVol = procVol;
  }

  lcd.setCursor(0, 0);
  lcd.print("MODE: ");
  if (Settings.mode == BOMB) {
    lcd.print(Settings.BOMBMODE);
  } else if (Settings.mode == POINT) {
    lcd.print(Settings.POINTMODE);
  } else if (Settings.mode == CHESSTIMER) {
    lcd.print(Settings.CHESSTIMERMODE);
  } else {
    lcd.print("ERROR");
  }
  lcd.print(Settings.mode == BOMB ? Settings.BOMBMODE : Settings.POINTMODE);
  lcd.setCursor(0, 1);
  lcd.print(Settings.bombTimerMin);
  lcd.print(" min. B=");
  lcd.print(procVol);
  lcd.print("%");
}

String formatTime(unsigned long time) {  //преобразование времени из millis в 00:00
  unsigned long minutes = time / MINTOMILLIS;
  unsigned long seconds = (time % MINTOMILLIS) / SECTOMILLIS;
  char buffer[6];
  sprintf(buffer, "%02lu:%02lu", minutes, seconds);
  return String(buffer);
}

//------------------------------------------------------------Режим шахматных часов

void ChessTimer() {
  if (butt1.isPress()) {
    if (flagState == REDCAPTURED) {
      tone(ZOOM, CAPTURE_BEEP_FREQ, CAPTURE_BEEP_DURATION);
      flagState = GREENCAPTURED;
      lastGreenCaptureTime = millis();
      redHoldTime += millis() - lastRedCaptureTime;
    } else if (flagState == NEUTRAL) {
      tone(ZOOM, CAPTURE_BEEP_FREQ, CAPTURE_BEEP_DURATION);
      flagState = GREENCAPTURED;
      lastGreenCaptureTime = millis();
      isMainScreenUse = false;
      lcd.clear();
    }
  }

  if (butt2.isPress()) {
    if (flagState == GREENCAPTURED) {
      tone(ZOOM, CAPTURE_BEEP_FREQ, CAPTURE_BEEP_DURATION);
      flagState = REDCAPTURED;
      lastRedCaptureTime = millis();
      greenHoldTime += millis() - lastGreenCaptureTime;
    } else if (flagState == NEUTRAL) {
      tone(ZOOM, CAPTURE_BEEP_FREQ, CAPTURE_BEEP_DURATION);
      flagState = REDCAPTURED;
      lastRedCaptureTime = millis();
      isMainScreenUse = false;
      lcd.clear();
    }
  }

  if (flagState == GREENCAPTURED) {
    currentGreenHoldTime = millis() - lastGreenCaptureTime + greenHoldTime;
  } else if (flagState == REDCAPTURED) {
    currentRedHoldTime = millis() - lastRedCaptureTime + redHoldTime;
  } else {
  }

  if (currentGreenHoldTime >= Settings.bombTimerMin * MINTOMILLIS || currentRedHoldTime >= Settings.bombTimerMin * MINTOMILLIS) {
    CaptureWin();
  }

  if (isMainScreenUse == false && millis() - lastPrintTime > SCREENUPDATE) {
    lastPrintTime = millis();
    lcd.setCursor(0, 0);
    lcd.print(flagState == GREENCAPTURED ? Settings.TEAM1 : Settings.TEAM2);
    lcd.print(" ");
    lcd.print(Settings.HOLDING);
    lcd.setCursor(0, 1);
    lcd.print(Settings.T1);
    lcd.print(formatTime(currentGreenHoldTime));
    lcd.setCursor(9, 1);
    lcd.print(Settings.T2);
    lcd.print(formatTime(currentRedHoldTime));
  }
}