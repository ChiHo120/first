#ifndef _FUN_H
#define _FUN_H


void led_show(uint8_t led,uint8_t mode);
void change(void);
void lcd_show(void);
float get_vol1(void);
float get_vol2(void);
void uart_rx(void);
void eeprom_write(uint8_t addr,uint8_t data);
uint16_t eeprom_read(uint8_t addr);


#endif
