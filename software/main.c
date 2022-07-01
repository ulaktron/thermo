#pragma config FOSC = INTOSC    // use internal oscillator
#pragma config WDTE = ON        // enable watchdog timer
#pragma config PWRTE = OFF      // disable power-up timer
#pragma config MCLRE = ON       // MCLR pin enabled
#pragma config CP = OFF         // flash memory protection disabled
#pragma config BOREN = OFF      // brown-out reset disabled
#pragma config CLKOUTEN = OFF   // clock output disabled
#pragma config WRT = OFF        // flash self-write protection disabled
#pragma config STVREN = ON      // stack overflow/underflow reset enabled
#pragma config BORV = LO        // low trip point for brown-out reset
#pragma config LPBOR = OFF      // low-power brown-out reset disabled
#pragma config LVP = ON         // low-voltage programming enabled

#include <xc.h>
#include "lcd.h"
#include "sensor.h"

#define PIN5_bm (0b00100000)

void main(void)
{
    // use 31kHz LFINTOSC
    OSCCONbits.IRCF = 0;

    // WDT period of 4 seconds
    WDTCON = 0b00011000;

    // disable analog inputs
    ANSELA = 0;
    ANSELB = 0;
    ANSELE = 0;

    // enable LCD outputs as well as bit-banged SPI outputs
    TRISA = 0b00000100;
    TRISB = 0b00101110;
    TRISC = 0b00000001;
    TRISD = 0b00000100;
    TRISE = 0b11111000;

    // drive all outputs low except for ~CS and SCK
    PORTA = 0b00000000;
    PORTB = 0b00000000;
    PORTC = 0b00000000;
    PORTD = 0b00000011;
    PORTE = 0b00000000;

    // initialize the LCD subsystem
    lcd_init();
    lcd_dec(888);

    // initialize the sensor
    if (sensor_init())
    {
        lcd_hex(0xE01);
        while(1);
    }

    // enable interrupt on change for pin 5 of PORTB (Celsius/Fahrenheit switch)
    IOCBP = PIN5_bm;
    IOCBN = PIN5_bm;
    INTCON = 0b00001000;

    int16_t disp = INT16_MAX;

    while(1)
    {
        SLEEP();
        IOCBF = 0;

        int16_t v = sensor_read();

        if (PORTB & PIN5_bm)
            v = (int16_t)((int)(v * 18) >> 4) + 320; // Fahrenheit x10
        else
            v = (int16_t)((int)(v * 10) >> 4); // Celsius x10

        if (v != disp)
            lcd_dec(disp = v);
    }
}
