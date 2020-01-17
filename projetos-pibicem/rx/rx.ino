#include <SPI.h>
#include <mrf24j.h>

const static int pin_reset = 6;
const static int pin_cs = 10;
const static int pin_interrupt = 2;

Mrf24j mrf(pin_reset, pin_cs, pin_interrupt);

long last_time;
long tx_interval = 1000;

void interrupt_routine(){
        mrf.interrupt_handler();
}

void handle_rx(){
        //Serial.print("received a packet ");
        //Serial.print(mrf.get_rxinfo()->frame_length, DEC);
        //Serial.println(" bytes long");
        
        if(mrf.get_bufferPHY()){
                //Serial.println("Packet data (PHY Payload):");
                for(int i = 0; i < mrf.get_rxinfo()->frame_length; i++){
                        Serial.print(mrf.get_rxbuf()[5]);
                }
        }
        //Serial.println("\r\nASCII data (relevant data):");
        for(int i = 0; i < mrf.rx_datalength(); i++){
                Serial.write(mrf.get_rxinfo()->rx_data[i]);
        }
        //Serial.print("\r\nLQI/RSSI=");
        //Serial.print(mrf.get_rxinfo()->lqi, DEC);
        //Serial.print("/");
        //Serial.println(mrf.get_rxinfo()->rssi, DEC);
}

void handle_tx(){}

void setup(){
        Serial.begin(9600);
        
        mrf.reset();
        mrf.init();
        mrf.set_channel(20);
        
        mrf.set_pan(0xcafe);
        mrf.address16_write(0x2301);
        
        mrf.set_promiscuous(true);
        mrf.set_palna(true);
        
        attachInterrupt(0, interrupt_routine, CHANGE);
        last_time = millis();
        interrupts();
}

void loop(){
        mrf.check_flags(&handle_rx, &handle_tx);
        unsigned long current_time = millis();
        if(current_time - last_time > tx_interval){
          last_time = current_time;
          Serial.println("");
          mrf.send16(0x2302, "");
          
        }
        delay(2000);
}


