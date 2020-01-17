
const int LM35 = A0; // Pino Analogico onde vai ser ligado ao pino 2 do LM35
const int BUFFER_SIZE = 1000; //Quantidade de vezes que vai ler para cria a média
const int REFRESH_RATE = 2000; //Tempo de atualização entre as leituras em ms
const float CELSIUS_BASE = 0.4887585532746823069403714565; //Base de conversão para Graus Celsius ((5/1023) * 100)

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop() {
  Serial.print("Temperatura: ");
  Serial.println(readTemperature());
  Serial.print("Temperatura com buffer: ");
  Serial.println(readTemperatureWithBuffer());
  delay(REFRESH_RATE);
}

float readTemperature(){
  return (analogRead(LM35) * CELSIUS_BASE);
}

float readTemperatureWithBuffer(){
  float buffer = 0;
  for (int i = 0; i < BUFFER_SIZE; i++){
      buffer += analogRead(LM35);
  }
  return ((buffer/BUFFER_SIZE) * CELSIUS_BASE);
}
