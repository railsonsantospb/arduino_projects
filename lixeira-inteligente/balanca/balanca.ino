#include "HX711.h"
#include <RHReliableDatagram.h>                      
#include <RH_ASK.h>      
#include <string.h>                            
#include <SPI.h>                                     
 
#define TX_ADDRESS 1                                 // endereço do transmissor
#define RX_ADDRESS 2                                 // endereço do receptor 
 

RH_ASK driver;   // instância RH ASK
RHReliableDatagram gerente(driver, TX_ADDRESS);   // configurando o gerenciador
 
uint8_t count = 1;   // contador
uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];   // buffer da mensagem
 
const int LOADCELL_DOUT_PIN = A0;
const int LOADCELL_SCK_PIN = A1;
int aux = 0;                                          


HX711 scale;
 
float calibration_factor =  4130;     // fator de calibração aferido na Calibração 
 
void setup()
{
  Serial.begin(9600);
  if (!gerente.init())   // se a inicialização do gerenciador falhar
     Serial.println("Falha na inicializacao");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);             // ajusta fator de calibração
  scale.tare();                                    // zera a Balança
}

void reverse(char *str, int len) 
{ 
    int i=0, j=len-1, temp; 
    while (i<j) 
    { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; j--; 
    } 
} 

int intToStr(int x, char str[], int d) 
{ 
    int i = 0; 
    while (x) 
    { 
        str[i++] = (x%10) + '0'; 
        x = x/10; 
    } 
  
    // If number of digits required is more, then 
    // add 0s at the beginning 
    while (i < d) 
        str[i++] = '0'; 
  
    reverse(str, i); 
    str[i] = '\0'; 
    return i; 
}

void ftoa(float n, char *res, int afterpoint) 
{ 
    // Extract integer part 
    int ipart = (int)n; 
  
    // Extract floating part 
    float fpart = n - (float)ipart; 
  
    // convert integer part to string 
    int i = intToStr(ipart, res, 0); 
  
    // check for display option after point 
    if (afterpoint != 0) 
    { 
        res[i] = '.';  // add dot 
  
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter is needed 
        // to handle cases like 233.007 
        fpart = fpart * pow(10, afterpoint); 
  
        intToStr((int)fpart, res + i + 1, afterpoint); 
    } 
}
 
void loop()
{
  float n = 0;
  if(scale.get_units() < 0){
    n = (-1)*scale.get_units();
  } else {
    n = scale.get_units();
  }

  char data[20] = "BL1B"; // Lixeira 1 atingiu 80%
  char data1[20] = "L1LVB"; // Lixeira 1 atingiu baixa energia
  
  int sensorValue = analogRead(A0); //read the A0 pin value
  float voltage = sensorValue * (5.00 / 1023.00) * 2; //convert the value to a true voltage.
  //voltage = 2.3;
  //n = 12.5;
  
  char aux[15]="";
  char peso[15]="";
  ftoa(voltage, aux, 2);
  ftoa(n, peso, 2);

  Serial.println(n);
 
  strcat(data1, aux);
  strcat(data, peso);
  strcat(data1, data);

   Serial.println(data1);
   if (!gerente.sendtoWait(data1, sizeof(data1), RX_ADDRESS)) {
      
      Serial.print("Transmitindo mensagem\n");
   } else {
      Serial.println("Message not send!");
   }

  
  if (Serial.available()){
    char temp = Serial.read();   // le carcter da serial 
    if (temp == 't' || temp == 'T')   // se pressionar t ou T
    {
    scale.tare();   // zera a balança
     Serial.println(" Balança zerada");   // imprime no monitor serial
    }
  }  
}
