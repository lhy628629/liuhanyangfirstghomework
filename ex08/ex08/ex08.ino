#include <WiFi.h>
#include <WebServer.h>

// 填写自己WiFi信息
const char* ssid = "verfure";
const char* password = "33333333";

#define TOUCH_PIN 4    // 触摸检测引脚 T0-GPIO4
#define ALARM_LED 2    // 报警LED引脚
int touchThreshold = 20;

WebServer server(80);

// 全局状态变量
bool isArm = false;      // false撤防  true布防
bool isAlarm = false;    // 是否正在报警

// 生成网页
String getHtml()
{
  String statusText = isArm ? "已布防" : "已撤防";
  String alarmText = isAlarm ? "⚠️ 警报触发中！" : "正常待命";

  String html = R"HTML(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1.0">
<title>物联网安防报警器</title>
<style>
body{text-align:center;margin-top:60px;font-size:20px;}
.btn{padding:12px 30px;margin:15px;font-size:18px;cursor:pointer;}
.arm{background:#f44336;color:white;border:none;}
.disarm{background:#2196F3;color:white;border:none;}
</style>
</head>
<body>
<h2>智能安防监控系统</h2>
<p>系统状态：)HTML" + statusText + R"HTML(</p>
<p>设备状态：)HTML" + alarmText + R"HTML(</p>
<a href="/arm"><button class="btn arm">一键布防</button></a>
<a href="/disarm"><button class="btn disarm">一键撤防</button></a>
</body>
</html>
)HTML";
  return html;
}

// 主页
void handleRoot()
{
  server.send(200, "text/html;charset=utf-8", getHtml());
}

// 布防
void handleArm()
{
  isArm = true;
  server.sendHeader("Location", "/");
  server.send(303);
}

// 撤防（解除报警+重置状态）
void handleDisarm()
{
  isArm = false;
  isAlarm = false;
  digitalWrite(ALARM_LED, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup()
{
  Serial.begin(115200);
  pinMode(ALARM_LED, OUTPUT);
  digitalWrite(ALARM_LED, LOW);

  // 连接WiFi
  WiFi.begin(ssid, password);
  Serial.print("正在连接WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功");
  Serial.print("安防系统管理地址：");
  Serial.println(WiFi.localIP());

  // 注册网页路由
  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.begin();
}

void loop()
{
  server.handleClient();

  // 已报警就持续高频闪烁
  if(isAlarm)
  {
    digitalWrite(ALARM_LED, HIGH);
    delay(80);
    digitalWrite(ALARM_LED, LOW);
    delay(80);
    return;
  }

  // 未报警时只检测触摸
  if(isArm)  // 只有布防状态才检测入侵
  {
    int val = touchRead(TOUCH_PIN);
    if(val < touchThreshold)
    {
      // 触发入侵，锁定报警状态
      isAlarm = true;
    }
  }
}
