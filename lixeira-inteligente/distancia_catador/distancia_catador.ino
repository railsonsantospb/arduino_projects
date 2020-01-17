/*
 * Ultrasonic Simple
 * Prints the distance read by an ultrasonic sensor in
 * centimeters. They are supported to four pins ultrasound
 * sensors (liek HC-SC04) and three pins (like PING)))
 * and Seeed Studio sensors).
 *
 * The circuit:
 * * Module HR-SC04 (four pins) or PING))) (and other with
 *   three pins), attached to digital pins as follows:
 * ---------------------    --------------------
 * | HC-SC04 | Arduino |    | 3 pins | Arduino |
 * ---------------------    --------------------
 * |   Vcc   |   5V    |    |   Vcc  |   5V    |
 * |   Trig  |   12    | OR |   SIG  |   13    |
 * |   Echo  |   13    |    |   Gnd  |   GND   |
 * |   Gnd   |   GND   |    --------------------
 * ---------------------
 * Note: You do not obligatorily need to use the pins defined above
 * 
 * By default, the distance returned by the read()
 * method is in centimeters. To get the distance in inches,
 * pass INC as a parameter.
 * Example: ultrasonic.read(INC)
 *
 * created 3 Apr 2014
 * by Erick Simões (github: @ErickSimoes | twitter: @AloErickSimoes)
 * modified 23 Jan 2017
 * by Erick Simões (github: @ErickSimoes | twitter: @AloErickSimoes)
 * modified 03 Mar 2017
 * by Erick Simões (github: @ErickSimoes | twitter: @AloErickSimoes)
 * modified 11 Jun 2018
 * by Erick Simões (github: @ErickSimoes | twitter: @AloErickSimoes)
 *
 * This example code is released into the MIT License.
 */

#include <Ultrasonic.h>
#include <RHReliableDatagram.h>                      // biblioteca Radiohead reliableDatagram
#include <RH_ASK.h>                                  // biblioteca Radiohead ASK
#include <SPI.h>                                     // biblioteca SPI
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>

#define TX_ADDRESS 1                                 // endereço do transmissor
#define RX_ADDRESS 2                                 // endereço do recept

Ultrasonic ultrasonic(8, 9);
int distance;
RH_ASK driver;   // instância RH ASK
RHReliableDatagram gerente(driver, TX_ADDRESS);   // configurando o gerenciador
 

uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];   // buffer da mensagem
 

void setup()
{
  Serial.begin(9600);   // inicializa console serial 9600 bps
  if (!gerente.init())   // se a inicialização do gerenciador falhar
   Serial.println("Falha na inicializacao");   // print na console serial
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

  char data[20] = "CL1C";   // Lixeira 1 atingiu capacidade
  char data1[20] = "L1LVC"; // Lixeira1 está com baixa energia

  distance = ultrasonic.read();
  
  char y[3]; sprintf(y, "%i", distance);

  int sensorValue = analogRead(A0); //read the A0 pin value
  float voltage = sensorValue * (5.00 / 1023.00) * 2; //convert the value to a true voltage.
  
  char aux[5]={'\0'};
  ftoa(voltage, aux, 2);
  char r[15] = "";
  strcat(data1, aux);
  strcat(data, y);
  strcat(data1, data);
  
   Serial.println(data1);
   if (!gerente.sendtoWait(data1, sizeof(data1), RX_ADDRESS)) {
      
      Serial.print("Transmitindo mensagem\n");
   } else {
      Serial.println("Message not send!");
   }
  
  
  delay(100);   // atraso 0,5 segundo
}
