// Функция для воспроизведения трека по номеру
void playTrack(int trackNumber) {
  uint8_t command[] = { 0x7E, 0xFF, 0x06, 0x03, 0x00, 0x00, trackNumber, 0xEF };  // Команда для воспроизведения трека
  Serial.write(command, sizeof(command));                                         // Отправка команды
  //delay(100);// Задержка для обработки команды
}

// Функция для остановки воспроизведения
void stopPlayback() {
  uint8_t command[] = { 0x7E, 0xFF, 0x06, 0x16, 0x00, 0x00, 0x00, 0xEF };  // Команда для остановки воспроизведения
  Serial.write(command, sizeof(command));                                  // Отправка команды
  //delay(100);// Задержка для обработки команды
}

// Функция для перехода в режим сна
void SlpMode() {
  uint8_t command[] = { 0x7E, 0xFF, 0x06, 0x0A, 0x00, 0x00, 0x00, 0xEF };  // Команда для перехода в режим сна
  Serial.write(command, sizeof(command));                                  // Отправка команды
  //delay(100);// Задержка для обработки команды
}

// Функция для пробуждения DFPlayer Mini
void wakUp() {
  uint8_t command[] = { 0x7E, 0xFF, 0x06, 0x0B, 0x00, 0x00, 0x00, 0xEF };  // Команда для пробуждения
  Serial.write(command, sizeof(command));                                  // Отправка команды
  //delay(100);// Задержка для обработки команды
}

void setVolume(int volume) {
  volume = constrain(volume, 0, 30);                                         // Ограничение громкости в пределах 0-30
  uint8_t command[] = { 0x7E, 0xFF, 0x06, 0x06, 0x00, 0x00, volume, 0xEF };  // Команда для установки громкости
  Serial.write(command, sizeof(command));                                    // Отправка команды
  //delay(100);// Задержка для обработки команды
}