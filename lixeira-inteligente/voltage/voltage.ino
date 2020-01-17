#include <Wire.h>


void setup()
{

  Serial.begin(9600);

}

void loop()
{
  printVolts();
}
 
 void printVolts()
{
  int sensorValue = analogRead(A0); //read the A0 pin value
  float voltage = sensorValue * (5.00 / 1023.00) * 2; //convert the value to a true voltage.
  Serial.println(voltage);
  if (voltage < 6.50) //set the voltage considered low battery here
  {
    
  }
}
