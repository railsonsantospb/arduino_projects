#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(9,10);

const uint64_t PIPE_COMANDO = 0xE8E8F0F0E1LL;
const uint64_t PIPE_RESPOSTA = 0xE8E8F0F0E2LL;


const int CMD_INTERROGA=11;
const int CMD_RESPONDEINTERROG=12;
const int CMD_PING=21;
const int CMD_RESPONDEPING=22;

int msg[5];

const byte pinoChave=2;
boolean temMensagem;

void setup() {
  Serial.begin(9600);
  pinMode(pinoChave,INPUT_PULLUP);
  radio.begin();
  radio.openWritingPipe(PIPE_COMANDO);
  radio.openReadingPipe(1,PIPE_RESPOSTA);
  radio.startListening();
}

void enviaMsg() {
  radio.stopListening();
  radio.write(msg, sizeof(msg));
  radio.startListening();
}
 
void interroga(int comando) {
  msg[0]=comando; 
  msg[1]=millis() % 32768;
  enviaMsg();
  Serial.print("Enviei comando ");
  Serial.println(msg[0]);
  for (int i=0; i<500; i++) {
    temMensagem=radio.available();
    if (temMensagem) {
      break;
    }  
    delay(4);
  }  
  if (temMensagem) {
    boolean concluido=false;
    while (!concluido) {
      concluido=radio.read(msg, sizeof(msg));
    }  
    switch (msg[0]) {
      case CMD_RESPONDEINTERROG:
        Serial.print("Recebi resposta de interrogacao: ");
        Serial.println(msg[1]);
        break;
      case CMD_RESPONDEPING:
        Serial.print("Recebi resposta de um ping enviado ha ");
        Serial.print((millis() % 32768)-msg[1]);
        Serial.println("ms.");
        break;
      default:
        Serial.print("Recebi resposta desconhecida ou impropria:");
        Serial.print(msg[0]);
        Serial.print(" ");
        Serial.println(msg[1]);
    }  
  } else {
    Serial.println("Nao houve resposta");
  }    
  Serial.println(" ");
} 

void responde() {
  int resposta=0;
  if (radio.available()) {
    boolean concluido=false;
    while (!concluido) {
      concluido=radio.read(msg, sizeof(msg));
    }  
    switch (msg[0]) {
      case CMD_INTERROGA:
          resposta=random(8000,9000);
          Serial.print("Recebi comando de interrogacao e vou responder: ");
          Serial.println(resposta);
          msg[0]=CMD_RESPONDEINTERROG;
          msg[1]=resposta;
          enviaMsg();
          break;
      case CMD_PING:
          msg[0]=CMD_RESPONDEPING;
          enviaMsg();
          Serial.print("Recebi comando ping e ja respondi: ");
          Serial.println(msg[1]);
          break;
        default:
          Serial.print("Recebi comando desconhecido ou improprio:");
          Serial.println(msg[0]);
      }  
  } else {
    Serial.println("Nao ha mensagens disponiveis");
  }  
} 
 
 
void loop() {
  if (digitalRead(pinoChave)==LOW) {
    if (random(0,5)==3) interroga(CMD_PING);
    else interroga(CMD_INTERROGA);
  } else {
    responde();
  }  
  delay(1000);
} 
