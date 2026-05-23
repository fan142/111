#include <WiFi.h>
#include <WebServer.h>

// -------------------------- 配置项 --------------------------
const char* ssid = "QAQ";
const char* password = "94pnnxzmr6gpifj";
const int LED_PIN = 2;                // 报警LED引脚（D2）
const int TOUCH_PIN = T0;             // 触摸引脚T0（对应GPIO4/D4）
const int TOUCH_THRESHOLD = 30;       // 触摸阈值（低于此值视为触摸）
// -----------------------------------------------------------

WebServer server(80);

// 全局状态变量
bool isArmed = false;    // 初始状态：撤防
bool isAlarming = false; // 初始状态：未报警
unsigned long lastBlinkTime = 0;
const int BLINK_INTERVAL = 100; // 报警闪烁间隔100ms

// 处理根目录，返回带布防/撤防按钮的网页
void handleRoot() {
  String statusText = isArmed ? "已布防" : "已撤防";
  String alarmText = isAlarming ? " 正在报警！" : "系统正常";
  
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 安防报警器</title>
  <style>
    body { font-family: Arial; text-align: center; margin-top: 50px; }
    .btn { padding: 15px 30px; font-size: 20px; margin: 10px; border: none; border-radius: 5px; cursor: pointer; }
    .arm-btn { background-color: #dc3545; color: white; }
    .disarm-btn { background-color: #28a745; color: white; }
    .status { font-size: 24px; margin: 20px 0; }
    .alarm { color: red; font-weight: bold; }
  </style>
</head>
<body>
  <h1>物联网安防报警器</h1>
  <div class="status">系统状态: <span id="status">)rawliteral" + statusText + R"rawliteral(</span></div>
  <div class="status alarm" id="alarmStatus">)rawliteral" + alarmText + R"rawliteral(</div>
  
  <button class="btn arm-btn" onclick="location.href='/arm'">布防 (Arm)</button>
  <button class="btn disarm-btn" onclick="location.href='/disarm'">撤防 (Disarm)</button>
</body>
</html>
  )rawliteral";
  
  server.send(200, "text/html; charset=UTF-8", html);
}

// 处理布防请求
void handleArm() {
  isArmed = true;
  isAlarming = false;
  digitalWrite(LED_PIN, LOW); // 确保LED熄灭
  Serial.println("系统已布防");
  server.sendHeader("Location", "/");
  server.send(303);
}

// 处理撤防请求
void handleDisarm() {
  isArmed = false;
  isAlarming = false;
  digitalWrite(LED_PIN, LOW); // 熄灭LED
  Serial.println("系统已撤防");
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // 初始熄灭
  
  // 连接WiFi
  Serial.print("连接WiFi中...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n连接成功!");
  Serial.print("访问地址: http://");
  Serial.println(WiFi.localIP());
  
  // 注册路由
  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  
  server.begin();
  Serial.println("Web服务器已启动");
}

void loop() {
  server.handleClient();
  
  // 布防状态下检测触摸
  if (isArmed && !isAlarming) {
    int touchValue = touchRead(TOUCH_PIN);
    Serial.printf("触摸值: %d\n", touchValue); // 串口打印方便调试阈值
    
    if (touchValue < TOUCH_THRESHOLD) {
      isAlarming = true;
      Serial.println("触发报警!");
    }
  }
  
  // 报警状态下LED闪烁
  if (isAlarming) {
    unsigned long now = millis();
    if (now - lastBlinkTime >= BLINK_INTERVAL) {
      lastBlinkTime = now;
      digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // 翻转LED状态
    }
  }
}
