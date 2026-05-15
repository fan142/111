// 作业8：millis()实现SOS求救信号
// 标准：三短(200ms) 三长(600ms) 三短(200ms)，间隔200ms，循环停顿2000ms
const int ledPin = 2;

enum State { SHORT_ON, SHORT_OFF, LONG_ON, LONG_OFF, PAUSE };
State currentState = SHORT_ON;
int shortCnt = 0, longCnt = 0;
unsigned long lastTime = 0;

const long T_SHORT_ON = 200;
const long T_SHORT_OFF = 200;
const long T_LONG_ON = 600;
const long T_LONG_OFF = 200;
const long T_PAUSE = 2000;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(115200);
  Serial.println("SOS信号启动");
}

void loop() {
  unsigned long now = millis();

  switch (currentState) {
    case SHORT_ON:
      digitalWrite(ledPin, HIGH);
      if (now - lastTime >= T_SHORT_ON) {
        lastTime = now;
        currentState = SHORT_OFF;
        shortCnt++;
      }
      break;

    case SHORT_OFF:
      digitalWrite(ledPin, LOW);
      if (now - lastTime >= T_SHORT_OFF) {
        lastTime = now;
        if (shortCnt < 3) currentState = SHORT_ON;
        else { longCnt = 0; currentState = LONG_ON; }
      }
      break;

    case LONG_ON:
      digitalWrite(ledPin, HIGH);
      if (now - lastTime >= T_LONG_ON) {
        lastTime = now;
        currentState = LONG_OFF;
        longCnt++;
      }
      break;

    case LONG_OFF:
      digitalWrite(ledPin, LOW);
      if (now - lastTime >= T_LONG_OFF) {
        lastTime = now;
        if (longCnt < 3) currentState = LONG_ON;
        else { shortCnt = 0; currentState = SHORT_ON; }
      }
      break;

    case PAUSE:
      digitalWrite(ledPin, LOW);
      if (now - lastTime >= T_PAUSE) {
        lastTime = now;
        shortCnt = longCnt = 0;
        currentState = SHORT_ON;
        Serial.println("--- SOS循环完成 ---");
      }
      break;
  }

  // 一轮SOS完成后进入停顿
  if (shortCnt == 3 && longCnt == 3 && currentState == SHORT_OFF) {
    currentState = PAUSE;
    lastTime = now;
  }
}
