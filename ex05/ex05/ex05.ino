// 定义触摸引脚 (T0对应GPIO4)
#define TOUCH_PIN 4
// 定义LED引脚 (ESP32 DevKit板载LED通常是GPIO2)
#define LED_PIN 2

// 触摸阈值，根据串口数值自行微调
int threshold = 20; 
int touchValue;

// 触摸自锁相关
bool lastIsTouch = false;
unsigned long debounceTime = 0;
const int debounceDelay = 80;

// 呼吸灯档位与参数
int gear = 1;          // 1/2/3 三档位循环
int brightness = 0;    // PWM亮度值 0~255
int fadeStep = 1;      // 亮度变化步长

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // 读取触摸数值
  touchValue = touchRead(TOUCH_PIN);
  Serial.print("TouchVal: ");
  Serial.print(touchValue);
  Serial.print("  档位：");
  Serial.println(gear);

  bool nowIsTouch = (touchValue < threshold);

  // 防抖 + 上升沿检测：触摸瞬间切换档位
  if (nowIsTouch != lastIsTouch && millis() - debounceTime > debounceDelay)
  {
    debounceTime = millis();
    if(nowIsTouch == true && lastIsTouch == false)
    {
      gear++;
      if(gear > 3) gear = 1; // 1→2→3→1 循环
    }
  }
  lastIsTouch = nowIsTouch;

  // 整体提速，所有档位延时大幅缩短，呼吸全程变快
  int breathDelay;
  switch (gear)
  {
    case 1: breathDelay = 18; break;  // 低速（偏快）
    case 2: breathDelay = 10; break;  // 中速
    case 3: breathDelay = 4;  break;  // 高速急促呼吸
  }

  // 标准PWM呼吸灯逻辑
  brightness += fadeStep;
  if(brightness <= 0 || brightness >= 255)
  {
    fadeStep = -fadeStep;
  }
  analogWrite(LED_PIN, brightness);

  delay(breathDelay);
}
