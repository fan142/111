// 作业5：多档位触摸调速呼吸灯
#define TOUCH_PIN 4
#define LED_PIN 2

// PWM配置（与实验3一致）
const int freq = 5000;
const int resolution = 8;

const int threshold = 700;
int speedLevel = 1;          // 速度档位：1(慢)→2(中)→3(快)
bool lastTouchState = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(115200);
  ledcAttach(LED_PIN, freq, resolution); // 绑定PWM通道
}

void loop() {
  // 触摸检测与档位切换
  int touchValue = touchRead(TOUCH_PIN);
  bool currentTouchState = (touchValue < threshold);

  if (currentTouchState != lastTouchState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentTouchState && !lastTouchState) {
      speedLevel++;
      if (speedLevel > 3) speedLevel = 1; // 循环切换
    }
  }

  lastTouchState = currentTouchState;

  // 根据档位设置呼吸速度
  int delayTime;
  switch (speedLevel) {
    case 1: delayTime = 20; break;
    case 2: delayTime = 10; break;
    case 3: delayTime = 5;  break;
  }

  // 呼吸灯效果
  for (int duty = 0; duty <= 255; duty++) {
    ledcWrite(LED_PIN, duty);
    delay(delayTime);
  }
  for (int duty = 255; duty >= 0; duty--) {
    ledcWrite(LED_PIN, duty);
    delay(delayTime);
  }
}

