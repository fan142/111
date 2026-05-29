#include <WiFi.h>
#include <WebServer.h>

// ------------------- 配置区 -------------------
const char* ssid = "QAQ";
const char* password = "94pnnxzmr6gpifj";

// LED引脚定义（ESP32开发板板载LED通常是GPIO2）
const int ledPin = 2;
// ---------------------------------------------

WebServer server(80);

// 网页HTML代码（包含滑动条和JavaScript）
const char* htmlPage = R"HTML(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 无极调光器</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            margin-top: 50px;
            background-color: #f0f0f0;
        }
        .container {
            background-color: white;
            padding: 30px;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
            max-width: 400px;
            margin: 0 auto;
        }
        h1 {
            color: #333;
        }
        .slider-container {
            margin: 30px 0;
        }
        #brightnessSlider {
            width: 100%;
            height: 20px;
            cursor: pointer;
        }
        #brightnessValue {
            font-size: 24px;
            font-weight: bold;
            color: #2196F3;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>LED 无极调光器</h1>
        <div class="slider-container">
            <input type="range" id="brightnessSlider" min="0" max="255" value="0">
            <p>亮度值: <span id="brightnessValue">0</span></p>
        </div>
    </div>

    <script>
        const slider = document.getElementById('brightnessSlider');
        const valueDisplay = document.getElementById('brightnessValue');

        // 监听滑动条变化
        slider.addEventListener('input', function() {
            const brightness = this.value;
            valueDisplay.textContent = brightness;
            
            // 发送GET请求给ESP32
            fetch(`/set?brightness=${brightness}`)
                .catch(err => console.error('发送请求失败:', err));
        });
    </script>
</body>
</html>
)HTML";

// 处理根目录请求，返回网页
void handleRoot() {
    server.send(200, "text/html", htmlPage);
}

// 处理亮度设置请求
void handleSetBrightness() {
    if (server.hasArg("brightness")) {
        // 获取URL中的亮度参数并转换为整数
        int brightness = server.arg("brightness").toInt();
        
        // 限制数值范围在0-255之间
        brightness = constrain(brightness, 0, 255);
        
        // 设置PWM占空比（analogWrite在ESP32上自动使用LEDC）
        analogWrite(ledPin, brightness);
        
        Serial.print("设置亮度为: ");
        Serial.println(brightness);
        
        // 返回成功响应
        server.send(200, "text/plain", "OK");
    } else {
        server.send(400, "text/plain", "缺少参数");
    }
}

void setup() {
    Serial.begin(115200);
    
    // 设置LED引脚为输出模式
    pinMode(ledPin, OUTPUT);
    // 初始亮度为0（熄灭）
    analogWrite(ledPin, 0);
    
    // 连接WiFi
    Serial.println();
    Serial.print("连接到WiFi: ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("");
    Serial.println("WiFi连接成功!");
    Serial.print("IP地址: ");
    Serial.println(WiFi.localIP());
    
    // 注册路由
    server.on("/", handleRoot);
    server.on("/set", handleSetBrightness);
    
    // 启动Web服务器
    server.begin();
    Serial.println("Web服务器已启动");
}

void loop() {
    // 处理客户端请求
    server.handleClient();
}
