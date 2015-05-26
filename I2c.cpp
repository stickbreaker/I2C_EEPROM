/*
I2C read / write routines with busy detection, write page consideration

Chuck Todd
ctodd@cableone.net
05JUL2013

*/
/*
18AUG13  Added code to BlockWrite to use a 8byte pages size for 8bit addresses EEPROMS.
  the 16byte assumed page size did not match the EEPROM in the MCP7941x RTCC,
  It is a 128byte EEPROM at Address 0x57, and uses an 8byte Write Page size.
  
*/
#include "I2c.h"
#include <inttypes.h>
#include "Wire.h"
#include "Arduino.h"

I2c I2C;  // create global declaration

I2c::my_i2c(){
 _sizeFlag = 0; // all eeproms are single byte addresses
 Wire.begin();
  }
  
void I2c::set_eeprom_size(uint8_t addressSizeFlag){
/*  This byte is used to identify single byte or double byte addresses for EEproms devices who's I2C identifiers
range 0x50 to 0x57. All other I2C devices are assumed to require only an 8bit address field.

Bit 0 is address 0x50, a 1 marks the device at address 0x50 as requiring a 16bit address.
  a 0 indicates a 8bit address device.
Bit 1 is address 0x51 ...
*/
_sizeFlag = b;
}

uint8_t I2c::get_eeprom_size(){
return _sizeFlag;
}

uint8_t I2c::read_block( uint8_t i2c_adr, uint16_t adr, uint8_t* buf, uint8_t bufsize, bool RS){
uint16_t b=0;
if (ready(i2c_adr,500)){ //Wait upto 500ms for memory ic to become ready, possibly in write

  while (Wire.available()){ // clear out buffer
    Wire.read();
    }
  Wire.beginTransmission(i2c_adr); // set address
  if((i2c_adr>=0x50)&&(i2c_adr<=0x57)&&(_sizeFlag&(1<<(i2c_adr&7)))){
    Wire.write(highByte(adr));
    }
  Wire.write(lowByte(adr));
  Wire.endTransmission(RS);
  uint8_t b1;
	while(bufsize>0){
	  b1 = 16;
	  if (b1>bufsize) b1=bufsize;
	  bufsize -=b1;
    Wire.requestFrom(i2c_adr,b1);
    while(Wire.available()&&(b1>0)){
      buf[b++]= Wire.read();
      b1--;
      }
    }
  }
return b;
}

uint8_t I2c::read_byte( uint8_t i2c_adr, uint16_t adr,bool RS){
uint8_t b;
if(!(read_block(i2c_adr,adr,&b,1,RS)==1)) b=0;
return b;
}

uint16_t I2c::read_word( uint8_t i2c_adr, uint16_t adr, bool RS){
uint16_t w;
if(!(read_block(i2c_adr,adr,(uint8_t*)&w,2,RS)==2)) w=0;
return w;
}

long I2c::read_long(uint8_t i2c_adr, uint16_t adr, bool RS){
long l;
if(!(read_block(i2c_adr,adr,(uint8_t*)&l,4,RS)==4)) l=0;
return l;
}

uint8_t I2c::write_block( uint8_t i2c_adr, uint16_t adr, uint8_t * buf, uint8_t bufsize){
  // uses 8 byte pages for 8bit address memory ic's
	// uses 16 byte pages for 16bit address ic's
 	// needs to work with page write buffer in IC. any write will rap around on page boundary
uint16_t t_addr,t_remain;
uint8_t t_block,b=0,pagesize=16;
t_addr = adr;
t_remain = bufsize;
bool b16=(i2c_adr>=0x50)&&(i2c_adr<=0x57)&&(_sizeFlag&(1<<(i2c_adr&7)));
if((i2c_adr>=0x50)&&(i2c_adr<=0x57)&&!b16) pagesize=8;
do{
  t_block = pagesize-(t_addr&(pagesize-1));
  if(t_block > t_remain) t_block = t_remain;
  t_remain -= t_block;
  if (ready(i2c_adr,500)){ //Wait for memory ic to become ready, possible prior write in progress
    Wire.beginTransmission(i2c_adr); // set address
    if(b16){
      Wire.write(highByte(t_addr));
      }
    Wire.write(lowByte(t_addr));
    t_addr += t_block;
    while (t_block-- >0) {
      Wire.write(buf[b++]);
	    }	  
    Wire.endTransmission();
    }
  else t_remain=0;	// failed, i2c chip did not respond. possible partial transfer	  
  }while(t_remain>0);
return b; // count of bytes successfully transferred
}

uint8_t I2c::write_byte( uint8_t i2c_adr,uint16_t adr, uint8_t val){
return write_block(i2c_adr,adr,&val,1);
}

uint8_t I2c::write_word( uint8_t i2c_adr,uint16_t adr, uint16_t val){
return write_block(i2c_adr,adr,(uint8_t*)&val,2);
}

uint8_t I2c::my_i2c::write_long( uint8_t i2c_adr,uint16_t adr, long val){
return write_block(i2c_adr,adr,(uint8_t*)&val,4);
}

bool I2c::ready(uint8_t i2c_adr, uint16_t timeout_out){
uint8_t st =0xff;
unsigned long time = millis()+time_out;
do{
  Wire.beginTransmission(i2c_adr);
  st = Wire.endTransmission();
  }while((st!=0)&&(time>millis()));
return (st==0); 
}
