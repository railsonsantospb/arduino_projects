int humidade, temperatura;

void setup(){
  Serial.begin(9600);

  randomSeed(digitalRead(0));
}

void loop() {
  String t, h, m, n;
  
  temperatura = random(1, 100);  
  humidade = random(1, 100);
  t = m + temperatura;
  h = n + humidade;
  //Serial.println(randNumber);
  Serial.println(t +" "+h);

  delay(500);
}
