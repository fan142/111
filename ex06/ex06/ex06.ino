// ESP32 警车双闪灯效 (双通道PWM) ex06作业 

// 引脚定义
const int led1Pin = 2;  // 第一个LED引脚
const int led2Pin = 4;  // 第二个LED引脚

// 呼吸灯变量
int brightness = 0;     // LED1的亮度(0~255)
int fadeStep = 1;       // 亮度变化步长
unsigned long lastFadeTime = 0;
const int fadeInterval = 10; // 亮度更新间隔(ms)，数值越小闪烁越快

void setup() {
  Serial.begin(115200);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  
  Serial.println("ESP32 双通道PWM警车双闪启动");
}

void loop() {
  // 非阻塞式渐变
  if (millis() - lastFadeTime >= fadeInterval) {
    lastFadeTime = millis();
    
    // 更新LED1的亮度
    brightness += fadeStep;
    
    // 到达亮度边界时反转方向
    if (brightness <= 0 || brightness >= 255) {
      fadeStep = -fadeStep;
      brightness = constrain(brightness, 0, 255); // 防止溢出
    }
    
    // 核心逻辑：两个LED亮度完全反相
    // 当灯A从0增加到255时，灯B恰好从255减小到0
    analogWrite(led1Pin, brightness);
    analogWrite(led2Pin, 255 - brightness);
  }
}
