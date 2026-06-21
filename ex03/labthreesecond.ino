int ledPin = 2;

void setup()
{
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  // 三次短闪
  for(int i=0; i<3; i++)
  {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
  delay(500);

  // 三次长闪
  for(int i=0; i<3; i++)
  {
    digitalWrite(ledPin, HIGH);
    delay(600);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
  delay(500);

  // 再三次短闪
  for(int i=0; i<3; i++)
  {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  }

  // 一轮结束长停顿
  delay(2000);
}