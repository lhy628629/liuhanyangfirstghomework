#include <WiFi.h>
#include <WebServer.h>

// WiFi信息自行修改
const char* ssid = "verfure";
const char* password = "33333333";
const int LED_PIN = 2;

WebServer server(80);
int ledBright = 0;  // 全局存储LED亮度 0~255

// 构建带滑动条的网页
String makeHtmlPage()
{
  String html = R"HTML(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1.0">
<title>网页无极调光器</title>
<style>
body{text-align:center;margin-top:60px;font-size:18px;}
input{width:80%;height:30px;margin-top:30px;}
</style>
</head>
<body>
<h2>ESP32 网页无极调光</h2>
<p>当前亮度值：<span id="valShow">0</span></p>
<input type="range" min="0" max="255" value="0" id="slider">

<script>
let slider = document.getElementById("slider");
let show = document.getElementById("valShow");

// 监听滑块滑动
slider.addEventListener('input',function(){
  let num = this.value;
  show.innerText = num;
  // 发送数值到ESP32
  fetch("/set?d="+num);
})
</script>
</body>
</html>
)HTML";
  return html;
}

// 主页路由
void handleIndex()
{
  server.send(200,"text/html;charset=utf-8",makeHtmlPage());
}

// 接收滑块亮度数值
void handleSetDuty()
{
  if(server.hasArg("d"))
  {
    ledBright = server.arg("d").toInt();
    // 限制范围0~255
    if(ledBright < 0) ledBright = 0;
    if(ledBright > 255) ledBright = 255;
  }
  server.send(200,"text/plain","ok");
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN,OUTPUT);
  
  // 连接WiFi
  WiFi.begin(ssid,password);
  Serial.print("正在连接WiFi");
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功");
  Serial.print("控制网页地址：");
  Serial.println(WiFi.localIP());

  // 注册路由
  server.on("/",handleIndex);
  server.on("/set",handleSetDuty);
  server.begin();
}

void loop()
{
  server.handleClient();
  // 实时PWM输出亮度
  analogWrite(LED_PIN,ledBright);
  delay(10);
}