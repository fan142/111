#include <WiFi.h>
#include <WebServer.h>

// -------------------------- 配置项 --------------------------
const char* ssid = "QAQ";
const char* password = "94pnnxzmr6gpifj";
const int TOUCH_PIN = T0; // 触摸传感器引脚T0（GPIO4/D4）
// -----------------------------------------------------------

WebServer server(80);

// 处理根目录，返回仪表盘网页
void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 实时传感器仪表盘</title>
  <style>
    body { font-family: Arial; text-align: center; margin-top: 100px; }
    .dashboard { font-size: 120px; font-weight: bold; color: #007bff; }
    .label { font-size: 24px; color: #666; margin-top: 20px; }
  </style>
</head>
<body>
  <h1>触摸传感器实时数据</h1>
  <div class="dashboard" id="sensorValue">0</div>
  <div class="label">触摸值（手靠近数值变小）</div>
  
  <script>
    const valueDisplay = document.getElementById('sensorValue');

    // 每100ms拉取一次传感器数据
    setInterval(async () => {
      try {
        const response = await fetch('/getSensorData');
        const value = await response.text();
        valueDisplay.textContent = value;
      } catch (err) {
        console.error('获取数据失败:', err);
      }
    }, 100);
  </script>
</body>
</html>
  )rawliteral";
  
  server.send(200, "text/html; charset=UTF-8", html);
}

// 处理传感器数据请求，返回纯文本数值
void handleGetSensorData() {
  int touchValue = touchRead(TOUCH_PIN);
  server.send(200, "text/plain", String(touchValue));
}

void setup() {
  Serial.begin(115200);
  
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
  server.on("/getSensorData", handleGetSensorData);
  
  server.begin();
  Serial.println("Web服务器已启动");
}

void loop() {
  server.handleClient();
}
