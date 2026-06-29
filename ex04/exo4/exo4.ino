// 定义触摸引脚 (T0对应GPIO4)
#define TOUCH_PIN 4
// 定义LED引脚 (ESP32 DevKit板载LED通常是GPIO2)
#define LED_PIN 2

// 触摸阈值，根据实际串口数值调整
int threshold = 20; 

int touchValue;
// 题目要求：LED状态布尔变量
bool ledState = false;
// 记录上一次触摸判定状态
bool lastIsTouch = false;
// 软件防抖计时
unsigned long debounceTime = 0;
const int debounceDelay = 80;

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  touchValue = touchRead(TOUCH_PIN);
  Serial.print("Touch Value: ");
  Serial.println(touchValue);

  // 当前是否处于触摸状态
  bool nowIsTouch = (touchValue < threshold);

  // 防抖 + 状态变化检测
  if (nowIsTouch != lastIsTouch && millis() - debounceTime > debounceDelay)
  {
    debounceTime = millis();
    // 边缘检测：仅 未触摸 → 刚触摸瞬间 翻转状态
    if(nowIsTouch == true && lastIsTouch == false)
    {
      ledState = !ledState;       // 翻转灯状态
      digitalWrite(LED_PIN, ledState);
    }
  }

  // 更新上一轮触摸状态
  lastIsTouch = nowIsTouch;
  delay(50);
}
