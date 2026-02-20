void build() {
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);

  GP.UI_MENU("Меню", GP_RED);  // начать меню
  GP.UI_LINK("/", "Домашняя страница");
  GP.UI_LINK("/settings", "Дополнительные настройки");
  GP.UI_LINK("/info", "Информация");
  GP.UI_LINK("/ota_update", "Обновление");

  GP.UI_BODY();

  GP.UPDATE("mode,hold,holdPoint,def,WinBeep,actingTEXT,actTEXT,expTEXT,defTEXT,defingTEXT,holTEXT,pass,save,res,R1,R2,OV,vol,pr,WFsave,HoNot,hingTEXT,BOM,PNT,CTM,TM1,TM2,PN,WN,TWN,STM1,STM2");

  GP.TITLE("ZTV ТОЧКА v1.0");
  GP.HR();

  if (ui.uri("/")) {
    M_BOX(GP.LABEL("Режим"); GP.SELECT("mode", "Бомба,Захват точки,Шахмотный таймер", Settings.mode); GP.BREAK(););
    GP.LABEL("Время удержания кнопки");
    M_BOX(GP.LABEL("сек."); GP.SLIDER("hold", Settings.holdTime, 1, 20););
    GP.BREAK();
    GP.LABEL("Время удержания точки");
    M_BOX(GP.LABEL("мин."); GP.SLIDER("holdPoint", Settings.bombTimerMin, 1, 60););
    GP.BREAK();
    GP.LABEL("Время на разминирование");
    M_BOX(GP.LABEL("сек."); GP.SLIDER("def", Settings.defuseHoldTime, 1, 60););
    GP.BREAK();
    GP.LABEL("Время писка после срабатывания");
    M_BOX(GP.LABEL("сек."); GP.SLIDER("WinBeep", Settings.finalBeepTime, 1, 60););
    GP.BREAK();
    GP.LABEL("сохранять прогресс захвата");
    GP.SWITCH("CuCo", Settings.isCumulativeCounter);
    GP.BREAK();
    GP.LABEL("Писк во время удержания точки стороной");
    GP.SWITCH("HoNot", Settings.isHoldPointNotification);
    GP.BREAK();

    GP.BUTTON("save", "Сохранить настройки");

    M_SPOILER(
      "Сброс",
      GP.BUTTON_MINI("res", "Сброс настроек"););

  } else if (ui.uri("/settings")) {
    GP.LABEL("Пароль точки доступа");
    GP.TEXT("pass", "pass", WF.WF_PASS);
    GP.BUTTON("WFsave", "Сохранить Wi-Fi");
    GP.BREAK();
    GP.BREAK();
    GP.BREAK();



    M_BLOCK_THIN_TAB(
      "Настройка делителя",
      M_BOX(GP.LABEL("R1"); GP.NUMBER_F("R1", "", Settings.divR1););
      M_BOX(GP.LABEL("R2"); GP.NUMBER_F("R2", "", Settings.divR2););
      M_BOX(GP.LABEL("Опорное"); GP.NUMBER_F("OV", "", Settings.voltCorr););
      GP.BUTTON("save", "Сохранить настройки");
      GP.BREAK();

      GP.LABEL("Напряжение: ");
      GP.LABEL("", "vol");
      GP.LABEL("В");
      GP.BREAK();
      GP.LABEL("Процент заряда: ");
      GP.LABEL("NAN", "pr");
      GP.LABEL("%");
      GP.BREAK(););

    M_BLOCK_THIN_TAB(
      "Отображение надписей",
      GP.LABEL("Что будет написано при:");
      GP.BREAK();
      GP.LABEL("(писать только латиницей)");
      GP.BREAK();
      GP.BREAK();

      GP.LABEL("Бомба активируется");
      GP.BREAK();
      GP.TEXT("actingTEXT", "text", Settings.ACTIVATING);
      GP.BREAK();

      GP.LABEL("Бомба активирована");
      GP.BREAK();
      GP.TEXT("actTEXT", "text", Settings.ACTIVATED);
      GP.BREAK();

      GP.LABEL("Подорвалась");
      GP.BREAK();
      GP.TEXT("expTEXT", "text", Settings.EXPLODED);
      GP.BREAK();

      GP.LABEL("Обезвреживывается");
      GP.BREAK();
      GP.TEXT("defingTEXT", "text", Settings.DEFUSING);
      GP.BREAK();

      GP.LABEL("Обезврежена");
      GP.BREAK();
      GP.TEXT("defTEXT", "text", Settings.DEFUSE);
      GP.BREAK();

      GP.LABEL("Захвате");
      GP.BREAK();
      GP.TEXT("holTEXT", "text", Settings.HOLDTIMERES);
      GP.BREAK();

      GP.LABEL("Точка (удерживывается)");
      GP.BREAK();
      GP.TEXT("hingTEXT", "text", Settings.HOLDING);
      GP.BREAK();

      GP.LABEL("Режим бомбы");
      GP.BREAK();
      GP.TEXT("BOM", "text", Settings.BOMBMODE);
      GP.BREAK();

      GP.LABEL("Режим точки");
      GP.BREAK();
      GP.TEXT("PNT", "text", Settings.POINTMODE);
      GP.BREAK();

      GP.LABEL("Режим шахматного таймера");
      GP.BREAK();
      GP.TEXT("CTM", "text", Settings.CHESSTIMERMODE);
      GP.BREAK();


      GP.LABEL("Команда 1");
      GP.BREAK();
      GP.TEXT("TM1", "text", Settings.TEAM1);
      GP.BREAK();

      GP.LABEL("Команда 2");
      GP.BREAK();
      GP.TEXT("TM2", "text", Settings.TEAM2);
      GP.BREAK();

      GP.LABEL("Точка нейтральная");
      GP.BREAK();
      GP.TEXT("PN", "text", Settings.NETRAL);
      GP.BREAK();

      GP.LABEL("Выиграла");
      GP.BREAK();
      GP.TEXT("WN", "text", Settings.WIN);
      GP.BREAK();

      GP.LABEL("Террористы победили");
      GP.BREAK();
      GP.TEXT("TWN", "text", Settings.TWIN);
      GP.BREAK();

      GP.LABEL("Кратко команда 1");
      GP.BREAK();
      GP.TEXT("STM1", "text", Settings.T1);
      GP.BREAK();

      GP.LABEL("Кратко команда 2");
      GP.BREAK();
      GP.TEXT("STM2", "text", Settings.T2);
      GP.BREAK(););

    GP.BUTTON("save", "Сохранить настройки");
    GP.BREAK();

  } else if (ui.uri("/info")) {
    GP.LABEL("Прошивка v1.0");
    // GP.BREAK();
    // GP.BREAK();
    // GP.LABEL("Магазин");
    // GP.BREAK();
    // GP.UI_LINK("https://vk.com/ztv_airsoft", "VK магазин (скоро)");
    // GP.BREAK();
    // GP.BREAK();
    // GP.LABEL("Дополнительные материалы");
    // GP.BREAK();
    // GP.UI_LINK("https://boosty.to/ztv", "Boosty");
    // GP.BREAK();
    // GP.BREAK();
    // GP.LABEL("Последние прошивки");
    // GP.BREAK();
    // GP.UI_LINK("https://github.com/ZTVairsoft", "GitHub (скоро)");
    // GP.BREAK();
    // GP.BREAK();
    // GP.LABEL("Последние релизы");
    // GP.BREAK();
    // GP.UI_LINK("https://github.com/ZTVairsoft", "Релизы BIN (скоро)");

  } else if (ui.uri("/update")) {
  }

  GP.UI_END();
  GP.BUILD_END();
}

void action() {
  if (ui.click("res")) {
    mem1.reset();  // Сброс настроек
    Serial.println("Сброс");
    ESP.restart();
  }
  if (ui.click("save")) {
    mem1.updateNow();  // обновить сейчас
    Serial.println("настройки сохранены");
  }
  if (ui.click("WFsave")) {
    mem2.updateNow();  // обновить сейчас
    Serial.println("пароль сохранен");
  }

  if (ui.click()) {

    if (ui.clickInt("hold", Settings.holdTime)) {}
    if (ui.clickInt("holdPoint", Settings.bombTimerMin)) {}
    if (ui.clickInt("def", Settings.defuseHoldTime)) {}
    if (ui.clickInt("WinBeep", Settings.finalBeepTime)) {}

    if (ui.clickInt("mode", Settings.mode)) {}


    if (ui.clickString("actingTEXT", Settings.ACTIVATING)) {}
    if (ui.clickString("actTEXT", Settings.ACTIVATED)) {}
    if (ui.clickString("expTEXT", Settings.EXPLODED)) {}
    if (ui.clickString("defingTEXT", Settings.DEFUSING)) {}
    if (ui.clickString("defTEXT", Settings.DEFUSE)) {}
    if (ui.clickString("holTEXT", Settings.HOLDTIMERES)) {}
    if (ui.clickString("hingTEXT", Settings.HOLDING)) {}
    if (ui.clickString("BOM", Settings.BOMBMODE)) {}
    if (ui.clickString("PNT", Settings.POINTMODE)) {}
    if (ui.clickString("CTM", Settings.CHESSTIMERMODE)) {}
    if (ui.clickString("TM1", Settings.TEAM1)) {}
    if (ui.clickString("TM2", Settings.TEAM2)) {}
    if (ui.clickString("PN", Settings.NETRAL)) {}
    if (ui.clickString("WN", Settings.WIN)) {}
    if (ui.clickString("TWN", Settings.TWIN)) {}
    if (ui.clickString("STM1", Settings.T1)) {}
    if (ui.clickString("STM2", Settings.T2)) {}

    if (ui.clickFloat("R1", Settings.divR1)) {}
    if (ui.clickFloat("R2", Settings.divR2)) {}
    if (ui.clickFloat("OV", Settings.voltCorr)) {}

    if (ui.clickBool("CuCo", Settings.isCumulativeCounter)) {}
    if (ui.clickBool("HoNot", Settings.isHoldPointNotification)) {}
  }



  if (ui.update()) {
    if (ui.update("vol")) ui.answer(resVoltValue, 2);
    if (ui.update("pr")) ui.answer(procVol);
  }
}