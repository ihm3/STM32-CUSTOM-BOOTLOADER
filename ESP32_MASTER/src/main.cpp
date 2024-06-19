#include <Arduino.h>
#include "data.h"

typedef enum
{
  Read,
  Write,
};

#define __boot
#ifdef __boot

  #include "SPIFFS.h"
#endif

#define WRITE true
#define READ  false

SLAVE slave;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 19);
  #ifdef __boot
    
    SPIFFS.begin();
    

    slave.PARS.start          = 163;
    slave.PARS.commandNo      = 12;
    slave.PARS.slaveNo        = 3;
    slave.PARS.commandAddr    = 0x0032;
    slave.PARS.commandType    = true;
    slave.PARS.dataType       = _int;
    slave.PARS.data           = 0x0010;
    slave.PARS.end            = 37;
  #endif

}

void loop() {

  #ifdef __boot
  static unsigned char a = 0;

  while (Serial.available())
  {
    a = Serial.read();
  }
  
  if (a)
  {
    File binfile = SPIFFS.open("/stm32f4.bin", "r");
    
    uint32_t i = 0;
    while (binfile.available())
    {
      //tmp = binfile.read();
      binfile.seek(i);

      binfile.readBytes((char*)&slave.PARS.data, sizeof slave.PARS.data);
  
      for (size_t j = 0; j < sizeof(slave.COMMAND); j++)
      {
            
          Serial2.write(slave.COMMAND[j]);
            
      }
        Serial.printf("Writing (%x)\n", slave.PARS.data);
        delay(10);
      
      i += 4;
      
    }
      a = 0;
    }
  

  #else

    static unsigned long ctr;
    static uint32_t i;

    slave.PARS.start          = 163;
    slave.PARS.commandNo      = 12;
    slave.PARS.slaveNo        = 3;
    slave.PARS.commandAddr    = 0x0032;
    slave.PARS.commandType    = Write;
    slave.PARS.data           = 0x0010;
    slave.PARS.dataType       = _int; 
    slave.PARS.end            = 37;

    if (millis()-ctr > 5000)
    {
      ctr = millis();
      for (size_t i = 0; i < sizeof(slave.COMMAND); i++)
      {
        Serial2.write(slave.COMMAND[i]);
        
      }
      Serial.println("attı");
    }
    while (Serial2.available())
    {
      Serial.write(Serial2.read());
    }
    i++;
  #endif
}