// 作业6：警车双闪灯效（双通道PWM）
#define LED_A_PIN 2  // 板载LED
#define LED_B_PIN 13 // 外接LED（串联220Ω电阻接GND）

const int freq = 5000;
const int resolution = 8;

void setup() {
  Serial.begin(115200);
  // 初始化两个独立的PWM通道
  ledcAttach(LED_A_PIN, freq, resolution);
  ledcAttach(LED_B_PIN, freq, resolution);
}

void loop() {
  // 核心：两个LED占空比反相
  for (int duty = 0; duty <= 255; duty++) {
    ledcWrite(LED_A_PIN, duty);       // A从0→255（变亮）
    ledcWrite(LED_B_PIN, 255 - duty); // B从255→0（变暗）
    delay(10);
  }
  for (int duty = 255; duty >= 0; duty--) {
    ledcWrite(LED_A_PIN, duty);
    ledcWrite(LED_B_PIN, 255 - duty);
    delay(10);
  }
}
