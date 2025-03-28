#ifndef __FT_EEPROM_H__
# define __FT_EEPROM_H__


# define GET_PRINTABLE_HEX_C(c)    ((c >= 0x20 && c <= 0x7e) ? (c) : ('.'))

void eeprom_write(const uint8_t data, const uint32_t addr);
uint8_t eeprom_read(const uint32_t addr);    
void print_eeprom(const uint8_t verbose);

#endif // __FT_EEPROM_H__