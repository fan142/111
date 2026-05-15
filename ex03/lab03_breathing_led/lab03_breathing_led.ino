// 实验3：PWM呼吸灯实验
const int ledPin = 2;
const int freq = 5000;      // PWM频率5000Hz
const int resolution = 8;   // 8位分辨率（0-255）

void setup() {
  Serial.begin(115200);
  ledcAttach(ledPin, freq, resolution);  // 绑定PWM通道
}

void loop() {
  // 逐渐变亮
  for (int duty = 0; duty <= 255; duty++) {
    ledcWrite(ledPin, duty);
    delay(10);
  }
  // 逐渐变暗
  for (int duty = 255; duty >= 0; duty--) {
    ledcWrite(ledPin, duty);
    delay(10);
  }
  Serial.println("呼吸周期完成");
}
