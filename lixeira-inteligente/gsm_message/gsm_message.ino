//Progama : Envio de SMS com o GSM Shield SIM900
//Autor: Arduino e Cia

#include "SIM900.h"
#include <SoftwareSerial.h>
//Carrega a biblioteca SMS
#include "sms.h"
#include <RHReliableDatagram.h>                      // biblioteca Radiohead reliableDatagram
#include <RH_ASK.h>                                  // biblioteca Radiohead ASK
#include <SPI.h>                                     // biblioteca SPI
 
#define TX_ADDRESS 1                                 // endereço do transmissor
#define RX_ADDRESS 2                                 // endereço do receptor 
 
uint8_t count = 0;   // contador
uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];   // buffer da mensagem
uint8_t tamanho;   // tamanho da mensagem
uint8_t from;   // endereço de quem transmite

RH_ASK driver;   // instância RH ASK
RHReliableDatagram gerente(driver, RX_ADDRESS);   // configurando o gerenciador


SMSGSM sms;

//char numero[20] = "996291717";
char numero[20] = "981422402";
char mensagem[60] = "Comunicacao realizada com Sucesso!";

boolean started = false;

void setup()
{
  //Inicializa a serial
  Serial.begin(9600);
  if (!gerente.init()){   // se a inicialização do gerenciador falhar
     Serial.println("Falha na inicializacao");   // print na console serial
  } else {
     Serial.println("Gerenciador active!"); 
  }
  

}
void loop()
{
  Serial.println("Receber Mensagem...");
  if (gerente.available())   // se gerenciador estiver ativo
  {
   tamanho = sizeof(buf);   // determina o tamanho do buffer
   if (gerente.recvfromAck(buf, &tamanho, &from))   // se o gerenciador receber mensagem
   {
   Serial.print("Recebido de: 0x");   // print na console serial
   Serial.print(from, HEX);   // print do endereço do transmissor em Hexadecimal
   Serial.print(": ");   // print na console serial
   Serial.println((char*)buf);   // print da mensagem recebida
   
   Serial.print("Ligando shield GSM SIM900. ");
   liga_desliga_GSMShield();
   Serial.println("Testando GSM shield...");
   //Comunicacao com o Shield GSM a 2400 bauds
   if (gsm.begin(2400))
   {
     Serial.println("nstatus=READY");
     started = true;
   }
   else Serial.println("nstatus=IDLE");

   if (started)
   {
     //Envia um SMS para o numero selecionado
     if (sms.SendSMS(numero, (char*)buf))
       Serial.println("nSMS enviado!");
   }
   Serial.print("Desligando shield GSM SIM900. ");
   liga_desliga_GSMShield();
   
   } else {
     Serial.println("Not Menssage");
   }
  }
  
  delay(1000);

}

void liga_desliga_GSMShield()
{
  
  Serial.print(F("Aguarde..."));
  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);
  delay(1000);
  digitalWrite(6, HIGH);
  delay(1000);
  Serial.println(F("OK!"));
  digitalWrite(6, LOW);
  delay(500);
}
