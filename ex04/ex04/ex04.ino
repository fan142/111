#define TOUCH_PIN 4
#define LED_PIN   2

const int freq = 5000;
const int resolution = 8;
const int threshold = 700;
const int debounceDelay = 50; 

bool ledOn = false; // false=灭，true=亮

void setup() {
  Serial.begin(115200);
  ledcAttach(LED_PIN, freq, resolution);
  ledcWrite(LED_PIN, 255); // 强制初始熄灭
  Serial.println("极简触摸开关（已加防抖）");
  Serial.println("摸一下亮，再摸一下灭");
}

void loop() {
  int touchVal = touchRead(TOUCH_PIN);
  Serial.printf("触摸值：%d\n", touchVal);


  if (touchVal < threshold) {
    delay(debounceDelay); 
    if (touchRead(TOUCH_PIN) < threshold) { 
      
      ledOn = !ledOn;
      ledcWrite(LED_PIN, ledOn ? 0 : 255); 
      Serial.printf(" 翻转！LED现在：%s\n", ledOn ? "亮" : "灭");

      while (touchRead(TOUCH_PIN) < threshold) {
        delay(10);
      }
    }
  }

  delay(50);
}
