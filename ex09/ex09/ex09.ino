#include <WiFi.h>
#include <WebServer.h>

// 自行修改WiFi账号密码
const char* ssid = "verfure";
const char* password = "33333333";

#define TOUCH_PIN 4
int touchVal = 0;

WebServer server(80);

// 主页面HTML + AJAX自动拉取数据
String getIndexHtml()
{
  String page = R"HTML(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1.0">
<title>触摸传感器实时仪表盘</title>
<style>
body{text-align:center;margin-top:80px;background:#f5f5f5;}
.box{width:300px;margin:0 auto;padding:30px;background:#fff;border-radius:12px;box-shadow:0 0 10px #ccc;}
.num{font-size:60px;color:#e53935;font-weight:bold;margin:20px 0;}
p{font-size:18px;color:#666;}
</style>
</head>
<body>
<div class="box">
<h2>触摸传感器实时数值</h2>
<div class="num" id="dataShow">0</div>
<p>靠近引脚数值变小 | 远离数值变大</p>
</div>

<script>
// 定时每秒请求一次传感器数据
function getData(){
  fetch("/getTouchData")
  .then(res=>res.text())
  .then(val=>{
    document.getElementById("dataShow").innerText = val;
  })
}
// 100毫秒刷新一次，实时流畅
setInterval(getData,100);
</script>
</body>
</html>
)HTML";
  return page;
}

// 主页路由
void handleHome()
{
  server.send(200,"text/html;charset=utf-8",getIndexHtml());
}

// 接口：返回纯触摸数值
void handleGetTouch()
{
  server.send(200,"text/plain",String(touchVal));
}

void setup()
{
  Serial.begin(115200);
  pinMode(TOUCH_PIN,INPUT);

  // 连接WiFi
  WiFi.begin(ssid,password);
  Serial.print("正在连接WiFi");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n连接成功！");
  Serial.print("仪表盘访问地址：");
  Serial.println(WiFi.localIP());

  // 注册路由
  server.on("/",handleHome);
  server.on("/getTouchData",handleGetTouch);
  server.begin();
}

void loop()
{
  server.handleClient();
  // 循环读取触摸原始数值
  touchVal = touchRead(TOUCH_PIN);
}