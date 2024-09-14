
#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <U8g2lib.h>

void warning(U8G2_SSD1306_128X64_NONAME_F_HW_I2C * u8g2) ;
void using_pin(U8G2_SSD1306_128X64_NONAME_F_HW_I2C * u8g2);
void connecting(U8G2_SSD1306_128X64_NONAME_F_HW_I2C * u8g2);
void success_connect(U8G2_SSD1306_128X64_NONAME_F_HW_I2C * u8g2);
void fail_connect(U8G2_SSD1306_128X64_NONAME_F_HW_I2C * u8g2);
void robot_arms(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* u8g2);
void left_2(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* u8g2);
void right_2(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* u8g2);
void under_3(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* u8g2);
void upper_3(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* u8g2);
void hand_4(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* u8g2);
void unconnect_bluetooth_mode(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* u8g2);
void unconnect_cable_mode(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* u8g2);
#endif
