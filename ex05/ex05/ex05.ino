// ESP32 多档位触摸调速呼吸灯 (ex05作业)

// 引脚定义
const int ledPin = 2;          // PWM输出引脚(板载LED)
const int touchPin = T0;       // 触摸引脚(T0对应GPIO4)

// 全局变量
int currentGear = 1;           // 当前速度档位(1-3档)
bool lastTouchState = false;   // 上次触摸状态
unsigned long lastTouchTime = 0; // 上次触摸时间(用于消抖)
const unsigned long debounceDelay = 200; // 触摸消抖时间(ms)

// 呼吸灯相关变量
int brightness = 0;           // 当前LED亮度
int fadeStep = 1;             // 亮度变化步长
unsigned long lastFadeTime = 0; // 上次亮度更新时间

// 不同档位对应的呼吸速度(数值越小，呼吸越快)
// 单位：ms，即每隔多少毫秒改变一次亮度
const int gearSpeeds[] = {30, 15, 5}; // 1档最慢，3档最快

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  
  // 初始化触摸引脚(ESP32内置触摸功能，无需额外设置)
  Serial.println("ESP32 多档位触摸呼吸灯启动");
  Serial.print("当前档位: ");
  Serial.println(currentGear);
}

void loop() {
  // -------------------------- 1. 触摸检测与档位切换 --------------------------
  // 读取触摸值(ESP32触摸值范围: 0~1023，触摸时数值会显著降低)
  int touchValue = touchRead(touchPin);
  // 触摸阈值(根据实际情况调整，一般小于500表示被触摸)
  bool isTouched = (touchValue < 400);

  // 触摸消抖处理
  if (isTouched && !lastTouchState && (millis() - lastTouchTime > debounceDelay)) {
    // 切换档位(1→2→3→1循环)
    currentGear++;
    if (currentGear > 3) {
      currentGear = 1;
    }
    
    Serial.print("触摸触发! 切换到档位: ");
    Serial.println(currentGear);
    lastTouchTime = millis();
  }
  
  lastTouchState = isTouched;

  // -------------------------- 2. 非阻塞式呼吸灯实现 --------------------------
  // 根据当前档位获取对应的更新间隔
  int currentSpeed = gearSpeeds[currentGear - 1];
  
  // 每隔currentSpeed毫秒更新一次亮度
  if (millis() - lastFadeTime >= currentSpeed) {
    lastFadeTime = millis();
    
    // 更新亮度
    brightness += fadeStep;
    
    // 当亮度达到最大值或最小值时，反转方向
    if (brightness <= 0 || brightness >= 255) {
      fadeStep = -fadeStep;
      // 防止亮度溢出
      brightness = constrain(brightness, 0, 255);
    }
    
    // 设置PWM输出(ESP32的analogWrite范围0~255)
    analogWrite(ledPin, brightness);
  }
}
