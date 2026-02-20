#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <LittleFS.h>

#define AP_SSID "ZTV_POINT"       //логин
#define AP_PASS "12345678"  //пароль от wifi готовой точки доступа

#include <GyverPortal.h>
GyverPortal ui(&LittleFS);

#include <EEManager.h>  //eeprom

#define SIREN D5       //пин соленоида основного
#define BUTTONGREEN D7  //КНОПКА ПЕРВОЙ СТОРОНЫ
#define BUTTONRED D4    //КНОПКА ВТОРОЙ СТОРОНЫ
#define ZOOM D6         //ПИЩАЛКА

#define BEEPINTERVAL 500        //промежутки писка при удержании
#define ACCELERATIONTIME 10000  // Время ускорения писка перед окончанием режима бомбы (10 секунд)

#define SECTOMILLIS 1000   // Для перевода из секунд в millis
#define MINTOMILLIS 60000  // Для перевода из минут в millis

#define SCREENUPDATE 1000  // частота обновления экрана

// Константы для звуковых сигналов
#define CAPTURE_BEEP_FREQ 4500     // Частота звука при захвате (Гц)
#define CAPTURE_BEEP_DURATION 50   // Длительность звука при захвате (мс)
#define CAPTURE_BEEP_INTERVAL 300  // Интервал звука при захвате (мс)

#define HOLD_BEEP_FREQ 5000      // Частота звука при удержании (Гц)
#define HOLD_BEEP_DURATION 100   // Длительность звука при удержании (мс)
#define HOLD_BEEP_INTERVAL 2000  // Интервал звука при удержании (мс)

unsigned long holdStartTime = 0;           // Переменная для отслеживания времени удержания кнопки
unsigned long lastBeepTime = 0;            // Переменная для отслеживания времени последнего писка
unsigned long holdingIP = 0;               //Переменная для отслеживания отображения имени и пароля точки.
bool isFlagWiFi = false;                   //флаг включения wifi
unsigned long voltTime = 0;                //Время с последней проверки напряжения
bool isResFl;                              //
float resVoltValue = 0.0;                  //результат рассчета напряжения
float procVol = 0.0;                       //процент заряда
int voltValue = 0;                         //Значение с аналогового пина
bool isBombActivated = false;              //Флаг активации бомбы
bool isBatSafeFlag = false;                //флаг для отключения в случае разряда аккумулятора
unsigned long bombActivatedTime = 0;       //Время с последней проверки напряжения
unsigned long defuseHoldStartTime = 0;     // Переменная для отслеживания времени удержания кнопки в режиме активации
unsigned long currentBeepInterval = 1000;  // Начальный интервал 1 секунда

enum FlagState { GREENCAPTURED,
                 NEUTRAL,
                 REDCAPTURED };
FlagState flagState = NEUTRAL;
FlagState prevFlagState = NEUTRAL;       // Предыдущее состояние флага
bool isCapturing = false;                // Флаг, указывающий, что точка захватывается
bool isNeutral = true;                   // Флаг, указывающий, что точка в нейтральном состоянии
unsigned long lastCaptureBeepTime = 0;   // Время последнего писка при захвате
unsigned long lastCapturedBeepTime = 0;  // Время последнего писка при удержании точки
unsigned long lastArrowUpdateTime = 0;   // Время последнего обновления стрелки
int arrowPosition = 0;                   // Позиция стрелки (0-4)
// Время удержания каждой стороны
unsigned long greenHoldTime = 0;
unsigned long redHoldTime = 0;
unsigned long lastGreenCaptureTime = 0;  // Время последнего захвата зеленой командой
unsigned long lastRedCaptureTime = 0;    // Время последнего захвата красной командой
unsigned long currentGreenHoldTime = 0;  // Текущее время удержания зеленой командой
unsigned long currentRedHoldTime = 0;    // Текущее время удержания красной командой
unsigned long captureStartTime = 0;      // Время начала захвата точки
unsigned long lastPrintTime = 0;         // Время последнего вывода информации
bool isMainScreenUse = true;             //необходимость отображать что то на главном экране
unsigned long lastMainScreenUpdate = 0;  // Время последнего обновлениглавного экрана

// Переменные для хранения предыдущих значений
unsigned long prevGreenTime = 0;
unsigned long prevRedTime = 0;
float prevProcVol = 0.0;

#include "GyverButton.h"
GButton butt1(BUTTONGREEN);
GButton butt2(BUTTONRED);

#define BOMB 0   //РЕЖИМ БОМБЫ
#define POINT 1  //РЕЖИМ ЗАХВАТА ТОЧКИ
#define CHESSTIMER 2 //РЕЖИМ ШАХМАТНОГО ТАЙМЕРА

LiquidCrystal_I2C lcd(0x3F, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

struct {                               //структура в которой хранятся настройки
  int holdTime = 10;                   //необходимое время удержания кнопки в секундах
  int countdownTimerMin = 25;          //время удержания точки в минутах
  int countdownTimerSec = 0;           //время удержания точки секунд
  int bombTimerMin = 10;               //минут для таймера в режиме бомбы
  int bombTimerSec = 0;                //секунд для таймера в режиме бомбы
  unsigned long holdingTimeGreen = 0;  //сохранение времени удержания 1 стороны
  int mode = 0;                        //режим работы
  unsigned long holdingTimeRed = 0;    //сохранение времени удержания 2 стороны
  float voltCorr = 1.0;                //напряжение для рассчета напряжения на аккумуляторе
  float divR1 = 120.0;                 //сопротивление R1 на делителе
  float divR2 = 10.0;                  //сопротивление R2 на делителе
  int batType = 1;                     //тип литиевого аккумулятора, 0-2s, 1-3s
  bool isBatSafe = false;              //Флаг защиты батареи
  int defuseHoldTime = 20;             //время удержания для разминирования секунд
  int finalBeepTime = 20;              //время писка после подрыва секунд
  String ACTIVATED = "ACTIVATED";      //текст при активации бомбы
  String EXPLODED = "EXPLODED";        //текст при подрыве
  String DEFUSE = "DEFUSE";            //текст при обезвреживании бомбы
  String HOLDTIMERES = "HOLDED: ";     //результат удержания точки
  String BOMBMODE = "BOMB";            //Название режима бомбы
  String POINTMODE = "POINT";          //Название режима захвата точки
  String CHESSTIMERMODE = "CHESS_TIMER";  //Название режима шахматного таймера
  String DEFUSING = "DEFUSING";        //тескт обезвреживания бомбы
  String ACTIVATING = "ACTIVATING";    //текст при активации бомбы
  String HOLDING = "HOLDING";          //текст когда точка удерживается одной стороной
  String NETRAL = "NEUTRAL";           //надпись при нейтральном состоянии точки
  String WIN = " WIN";                 //победила
  String TWIN = "T WIN";               //террористы победили
  String T1 = "G_";                    //краткое обозначение 1 команды
  String T2 = "R_";                    //краткое обозначение 2 команды

  int captureHoldTime = 10;             //секунд удержания кнопки для смены флага
  int winHoldTime = 15;                 //минут удержания точки для победы
  String TEAM1 = "GREEN_TEAM";          //название первой команды
  String TEAM2 = "RED_TEAM";            //название второй команды
  bool isCumulativeCounter = true;      //false - не сохраняется время захвата, true - накопительное время захвата
  bool isHoldPointNotification = true;  //включение, выключение оповещения при удержании точки
} Settings;                             //Название структуры к которой обращаемся

struct {
  String WF_SSID = "AP_ZTV";
  String WF_PASS = "12345678";
  bool apply = 1;
} WF;

IPAddress apIP(63, 63, 63, 63);

EEManager mem1(Settings);  // передаём нашу переменную (фактически её адрес)
EEManager mem2(WF);