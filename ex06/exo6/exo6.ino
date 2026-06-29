// 双通道反相PWM 警车渐变双闪灯
#define LED_A 2    // 第一个LED
#define LED_B 15   // 第二个LED

int pwmVal = 0;
int step = 2;     // 调大数值整体加快渐变速度

void setup()
{
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
}

void loop()
{
  // A渐亮，B同步渐暗，严格反相
  analogWrite(LED_A, pwmVal);
  analogWrite(LED_B, 255 - pwmVal);

  pwmVal += step;
  // 到达极值反转方向
  if(pwmVal >= 255 || pwmVal <= 0)
  {
    step = -step;
  }

  delay(6);  // 缩短延时，整体节奏更快更顺滑
}
