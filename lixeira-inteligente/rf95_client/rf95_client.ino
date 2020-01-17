// rf95_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf95_server
// Tested with Anarduino MiniWirelessLoRa, Rocket Scream Mini Ultra Pro with
// the RFM95W, Adafruit Feather M0 with RFM95

#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>                      
#include <RH_ASK.h>
#include <string.h>                                   

#define TX_ADDRESS 1                                 // endereço do transmissor
#define RX_ADDRESS 2                                 // endereço do receptor 
 
uint8_t count = 0;   // contador
uint8_t buf1[RH_ASK_MAX_MESSAGE_LEN];   // buffer da mensagem
uint8_t tamanho;   // tamanho da mensagem
uint8_t from;   // endereço de quem transmite

RH_RF95 rf95;

RH_ASK driver;   // instância RH ASK
RHReliableDatagram gerente(driver, RX_ADDRESS);   // configurando o gerenciador



void setup() 
{

  Serial.begin(9600);
  if (!gerente.init()){   // se a inicialização do gerenciador falhar
    Serial.println("Falha na inicializacao");   // print na console serial
  } else {
    Serial.println("Gerenciador active!"); 
  }
  while (!Serial) ; // Wait for serial port to be available
  if(rf95.init()){
    Serial.println("Active!");
  }  
}

void loop()
{

  
  
  rf95.waitPacketSent();

  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  uint8_t data[]= "";
  rf95.send(data, sizeof(data));
  if (rf95.waitAvailableTimeout(3000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      Serial.print("Response: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);

      Serial.println("Receber Mensagem...");
      if (gerente.available())   // se gerenciador estiver ativo
      {
       tamanho = sizeof(buf1);   // determina o tamanho do buffer
         if (gerente.recvfromAck(buf1, &tamanho, &from))   // se o gerenciador receber mensagem
         {
           Serial.print("Recebido de: 0x");   // print na console serial
           Serial.print(from, HEX);   // print do endereço do transmissor em Hexadecimal
           Serial.print(": ");   // print na console serial
           Serial.println((char*)buf1);   // print da mensagem recebida
           Serial.print("got reply: ");
            Serial.println((char*)buf1);
           char r[20] = "";
           strcat(r, (char*)buf1);
           rf95.send(r, sizeof(r));
           
           
         } 
      } else {
        Serial.println((char*)buf1);
        rf95.send((char*)buf1, sizeof((char*)buf1));
      }
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is rf95_server running?");
  
  }
  
  delay(2000);
}
