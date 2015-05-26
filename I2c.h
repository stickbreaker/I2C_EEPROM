#ifndef I2c_h

#define I2c_h

#include <inttypes.h>
#include <Wire.h>
#include <avr/pgmspace.h>

class I2c {
public:
  I2c();
  void set_eeprom_size(uint8_t addressSizeFlag); 
  uint8_t get_eeprom_size(void);
  bool ready(uint8_t i2c_adr, uint16_t time_out=0);
  
  uint8_t read_byte(uint8_t i2c_adr, uint16_t adr, bool RS=true);
  uint16_t read_word(uint8_t i2c_adr, uint16_t adr, bool RS=true);
  long read_long(uint8_t i2c_adr, uint16_t adr, bool RS=true);
  uint8_t read_block(uint8_t i2c_adr, uint16_t adr, uint8_t* buf, uint8_t bufsize, bool RS=true);
  
  uint8_t write_byte(uint8_t i2c_adr, uint16_t adr, uint8_t val);
  uint8_t write_word(uint8_t i2c_adr, uint16_t adr, uint16_t val);
  uint8_t write_long(uint8_t i2c_adr, uint16_t adr, long val);
  uint8_t write_block(uint8_t i2c_adr, uint16_t adr, uint8_t* buf, uint8_t bufsize);

private:
  uint8_t _sizeFlag; 
 };

extern I2c I2C;
#endif
