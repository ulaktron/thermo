#ifndef LCD_H
#define	LCD_H

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
extern void lcd_init(void);

/**
 * lcd_dec
 *
 *  Shows a decimal value.
 *
 *  Parameters:
 *    val - value to display
 *
 *  Returns:
 *    nothing.
 */
extern void lcd_dec(int16_t val);

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
extern void lcd_hex(uint16_t val);

#endif	/* LCD_H */
