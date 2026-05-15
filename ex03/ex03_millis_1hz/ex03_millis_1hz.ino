// 作业6：millis()实现1Hz稳定闪烁（亮500ms灭500ms）
const int ledPin = 2;
bool ledState = LOW;
unsigned long lastTime = 0;
const long interval = 500;  // 闪烁间隔

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  unsigned long now = millis();
  // 时间差判断，不阻塞程序
  if (now - lastTime >= interval) {
    lastTime = now;
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    Serial.println(ledState ? "ON" : "OFF");
  }
}
