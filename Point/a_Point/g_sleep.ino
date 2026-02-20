void enterDeepSleep() {
  Serial.println ("Я спать...");
  ESP.deepSleep(0, WAKE_RF_DISABLED); // Сон на неопределенное время
}