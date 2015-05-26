# I2C_EEPROM
Arduino I2C library to Access 24 series EEProms
  
  void set_eeprom_size(uint8_t addressSizeFlag); 
    Set 8bit or 16bit Address flag for I2C devices with Address range of 0x50 thru 0x57 (EEPROM devices)
    addressSizeFlag is a bit field.  0=8bit, 1= 16bit
    bit 0 = 0x50, bit 1 = 0x51 .. bit 7 = 0x57
   Example:
    set_eeprom_size(0xFE); I2C device at Address 0x50 is a 8bit address EEProm, all other 0x5x are 16bit.
   NOTE: BY DEFAULT all I2C devices are assumed to use 8bit addressing, if you are using any EEProms bigger than
     2,048kb (256x8) you must call this function, and correctly identify all devices 0x50..0x57 that use 16bit 
     addressing.
     Some devices like 24LC04 respond to all I2C address from 0x50 to 0x57, each address acts like a 8bit address EEPROM
  
  uint8_t get_eeprom_size(void);
   Returns current eeprom_size bit field.
  
  bool ready(uint8_t i2c_adr, uint16_t time_out = 0);
   checks that I2C device is ready to receive commands, used mainly to allow EEProm device to complete write cycle.
   time_out is milliseconds to wait for device to become ready.
   Returns:
     TRUE = device responded.
     FALSE = device is busy or does not exist on I2C buss.
  
  uint8_t read_byte(uint8_t i2c_adr, uint16_t adr, bool RS=true);
  uint16_t read_word(uint8_t i2c_adr, uint16_t adr, bool RS=true);
  long read_long(uint8_t i2c_adr, uint16_t adr, bool RS=true);
    RS controls repeated-start function of I2C protocol
      TRUE Issues STOP and Releases I2C buss between set address pointer write and read data operation.
      FALSE retains I2C buss between set address pointer write and read data operations.
         Some I2C devices(FreeScale MMA845x) reset internal address pointers when STOP is received.
  
  uint8_t read_block(uint8_t i2c_adr, uint16_t adr, uint8_t* buf, uint8_t bufsize, bool RS=true);
    Reads bufsize bytes from I2C device in 16byte blocks.
    RETURNS: count of bytes actually read, data in buf
  
  uint8_t write_byte(uint8_t i2c_adr, uint16_t adr, uint8_t val);
  uint8_t write_word(uint8_t i2c_adr, uint16_t adr, uint16_t val);
  uint8_t write_long(uint8_t i2c_adr, uint16_t adr, long val);
  uint8_t write_block(uint8_t i2c_adr, uint16_t adr, uint8_t* buf, uint8_t bufsize);
    Writes val or buf into device at adr, assumes 8byte write page for 8bit EEProms (0x50..0x57) 16byte pages for
    all other devices.
    RETURNS: count of bytes written
