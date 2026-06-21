// millis 实现1Hz LED闪烁 周期1秒
#define LED 2
unsigned long lastTime = 0;
const int timeGap = 500;

void setup() {
  pinMode(LED,OUTPUT);
}

void loop() {
  unsigned long now = millis();
  if(now - lastTime >= timeGap)
  {
    digitalWrite(LED,!digitalRead(LED));
    lastTime = now;
  }
}
