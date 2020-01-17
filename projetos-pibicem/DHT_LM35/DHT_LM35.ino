#include <dht.h>
#define dhtpin 5

int portAnalog = 5;
float temperatura = 0;
float tempo = 0;

dht sensor;

void setup(){
  Serial.begin(9600);
}
void loop(){
    sensor.read(5);
    Serial.print("Umidade: ");
    Serial.print(sensor.humidity);
    Serial.print(" % ");
    Serial.print("Temperatura: ");
    Serial.print(sensor.temperature);
    Serial.print(" Celsius ");
    Serial.print(" LM35 = ");
    temperatura = analogRead(portAnalog);
    tempo = (temperatura * 500) / 1023;
    Serial.print(tempo);
    Serial.print(" temp = ");
    Serial.print(millis());
    Serial.println();
    delay(2000);
}
