#include <SPI.h>
//#include <dht.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mrf24j.h>
//#define dht_pin A1
#define pin_reset 6
#define pin_cs 10
#define pin_interrupt 2

//const static int pin_reset = 6;
//const static int pin_cs = 10;
//const static int pin_interrupt = 2;
//dht sensor; //const

const int LM35 = A0; // Pino Analogico onde vai ser ligado ao pino 2 do LM35
const int REFRESH_RATE = 2000;  //Tempo de atualização entre as leituras em ms
const float CELSIUS_BASE = 0.4887585532746823069403714565; //Base de conversão para Graus Celsius ((5/1023) * 100)

Mrf24j mrf(pin_reset, pin_cs, pin_interrupt);

long last_time;
long tx_interval = 1000;

char data[16] = {' ', ' ', ' ', ' ', ' ',
                 ' ', ' ', ' ', ' ', ' ',
                 ' ', ' ', ' ', ' ', ' ', ' '};

void interrupt_routine() {
  mrf.interrupt_handler();
}

void handle_rx() {}

void handle_tx() {
  if (mrf.get_txinfo()->tx_ok) {
    //Serial.println("TX went ok, got ack");
  } else {
    //Serial.print("TX failed after ");
    //Serial.print(mrf.get_txinfo()->retries);
    //Serial.println(" retries\n");
  }
}

//char* humidade(double v){
//  char c[5] = {' ', ' ', ' ', ' ', ' '};
//  dtostrf(v, 2, 2, c);
//  return c;
//}

//char* temperatura(double v){
//  char c[5] = {' ', ' ', ' ', ' ', ' '};
//  dtostrf(v, 2, 2, c);
//  return c;
//}

float readTemperature(){
  return analogRead(LM35) * CELSIUS_BASE; 
}

void setup() {
  Serial.begin(9600);
  
  mrf.reset();
  mrf.init();
  mrf.set_channel(20);
  
  mrf.set_pan(0xcafe);
  mrf.address16_write(0x2302); 
  
  mrf.set_promiscuous(true);
  mrf.set_palna(true);
  
  attachInterrupt(0, interrupt_routine, CHANGE);
  last_time = millis();
  interrupts();
}


void loop() {
  mrf.check_flags(&handle_rx, &handle_tx);
  //sensor.read(dht_pin);
  unsigned long current_time = millis();
  if(current_time - last_time > tx_interval){
    last_time = current_time;
    Serial.println("Capturando a Temperatura:");
    
    //humidade(sensor.humidity);
    float n = readTemperature();
    char b[256];
    char c[256];
    dtostrf(n, 2, 2, c);
    String f = "      ";
    f += c;
    strcpy(b, f.c_str() );
    Serial.println(b);
    mrf.send16(0x2301, b);
  }
  delay(2000);
}
