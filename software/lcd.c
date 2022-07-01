#include <xc.h>
#include "lcd.h"

static const uint8_t _lcd_map[16] = {
    0b00111111,
    0b00000110,
    0b01011011,
    0b01001111,
    0b01100110,
    0b01101101,
    0b01111101,
    0b00000111,
    0b01111111,
    0b01101111,
    0b01110111,
    0b01111100,
    0b00111001,
    0b01011110,
    0b01111001,
    0b01110001
};

/**
 * lcd_init
 *
 *  Initializes the LCD subsystem.
 *
 *  Parameters:
 *    none.
 *
 *  Returns:
 *    nothing.
 */
void lcd_init(void)
{
    // static mode, LFINTOSC as clock source, enable the LCD module
    LCDCON = 0b00001100;

    // 1:5 prescaler -> ~49Hz frame rate
    LCDPS = 0b00000100;

    // enable internal LCD voltage reference
    LCDREF = 0b10000000;
    LCDRL  = 0b00010000;
    LCDCST = 0b00000000;

    // enable required segments
    LCDSE0 = 0b11111111;    // SEG0..SEG7
    LCDSE1 = 0b11111111;    // SEG8..SEG15
    LCDSE2 = 0b11111111;    // SEG16..SEG23

    // light up all segments
    LCDDATA0 = 0b11111111;
    LCDDATA1 = 0b11111111;
    LCDDATA2 = 0b11111111;

    // enable the LCD module
    LCDCON = 0b10001100;
}

/**
 * lcd_dec
 *
 *  Shows a decimal value.
 *
 *  Parameters:
 *    none.
 *
 *  Returns:
 *    nothing.
 */
void lcd_dec(int16_t val)
{
    if (val < 0 || val >= 10000)
    {
        LCDDATA0 = LCDDATA1 = LCDDATA2 = 0b01000000;
        return;
    }

    uint8_t d0, d1, d2, dot;

    if (val >= 1000)
    {
        val /= 10;
        dot = 0;
    }
    else
    {
        dot = 0b10000000;
    }

    div_t res = div(val, 10);
    d0 = _lcd_map[res.rem];

    res = div(res.quot, 10);
    d1 = _lcd_map[res.rem];
    d2 = _lcd_map[res.quot];

    LCDDATA2 = d2;
    LCDDATA1 = d1 | dot;
    LCDDATA0 = d0;
}

/**
 * lcd_hex
 *
 *  Shows a hexadecimal value.
 *
 *  Parameters:
 *    val - value to display
 * 
 *  Returns:
 *    nothing.
 */
extern void lcd_hex(uint16_t val)
{
    LCDDATA0 = _lcd_map[val & 0x0F];
    LCDDATA1 = _lcd_map[(val >> 4) & 0x0F];
    LCDDATA2 = _lcd_map[(val >> 8) & 0x0F];
}
