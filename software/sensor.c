#include <xc.h>
#include "sensor.h"

#define MISO_bm (0b00000001)
#define MOSI_bm (0b00000010)
#define SCK_bm  (0b00000001)
#define CS_bm   (0b00000010)

#define REG_STATUS  (0x00)
#define REG_CONFIG  (0x01)
#define REG_TEMP    (0x02)
#define REG_ID      (0x03)

static void _sensor_push(uint8_t val);
static uint8_t _sensor_pull(void);

/**
 * sensor_init
 *
 *  Initializes the sensor subsystem.
 *
 *  Parameters:
 *    none.
 *
 *  Returns:
 *    nothing.
 */
uint8_t sensor_init(void)
{
    // reset ADT7311 serial interface
    PORTD &= ~CS_bm;
    _sensor_push(0xFF);
    _sensor_push(0xFF);
    _sensor_push(0xFF);
    _sensor_push(0xFF);
    PORTD |= CS_bm;

    // begin transaction
    PORTD &= ~CS_bm;

    // read id register
    _sensor_push(0x40 | (REG_ID << 3));
    uint8_t sig = _sensor_pull();

    // finish transaction
    PORTD |= CS_bm;

    return sig != 0xC3;
}

/**
 * sensor_read
 *
 *  Reads the temperature.
 *
 *  Parameters:
 *    none.
 *
 *  Returns:
 *    temperature value in 0.0625°C increments
 */
int16_t sensor_read(void)
{
    // begin transaction
    PORTD &= ~CS_bm;

    // request one-shot conversion
    _sensor_push(REG_CONFIG << 3);
    _sensor_push(0x20);

    // finish transaction
    PORTD |= CS_bm;

    // wait for about 256ms
    uint8_t wdt = WDTCON;
    WDTCON = 0b00010000;
    SLEEP();
    WDTCON = wdt;

    // begin transaction
    PORTD &= ~CS_bm;

    _sensor_push(0x40 | (REG_TEMP << 3));
    uint8_t msb = _sensor_pull();
    uint8_t lsb = _sensor_pull();

    // finish transaction
    PORTD |= CS_bm;

    return ((int16_t)msb) << 5 | (uint8_t)(lsb >> 3);
}

/**
 * _sensor_push
 *
 *  Pushes out 8-bit into the serial interface.
 *
 *  Parameters:
 *    val - value to push out
 *
 *  Returns:
 *    nothing.
 */
static void _sensor_push(uint8_t val)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        PORTD &= ~SCK_bm;

        if (val & 0x80)
            PORTC |= MOSI_bm;
        else
            PORTC &= ~MOSI_bm;

        PORTD |= SCK_bm;
        val = (uint8_t)(val << 1);
    }
}

/**
 * _sensor_pull
 *
 *  Reads a 8-bit value from the serial interface.
 *
 *  Parameters:
 *    none.
 *
 *  Returns:
 *    received value.
 */
static uint8_t _sensor_pull(void)
{
    uint8_t val = 0;

    // shift in input byte
    for (uint8_t i = 0; i < 8; i++)
    {
        PORTD &= ~SCK_bm;
        val = (uint8_t)(val << 1);
        val |= PORTC & MISO_bm;
        PORTD |= SCK_bm;
    }

    return val;
}
