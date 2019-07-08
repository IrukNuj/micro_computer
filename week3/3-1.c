#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

volatile unsigned char wait = 0;
volatile unsigned char sw, sw_flag;
volatile unsigned char bz_conf;
volatile unsigned char led[8];

void update_led()
{
    static unsigned char sc = 0xFE;
    static unsigned char scan = 0;
    PORTB = 0;
    sc = (sc << 1) | (sc >> 7);
    PORTD = (PORTD & 0x0F) | (sc & 0xF0);
    PORTC = (PORTC & 0xF0) | (sc & 0x0F);
    scan = (scan + 1) & 7;
    PORTB = led[scan];
}

void update_sw()
{
    if (wait)
    {
        wait--;
        if (wait == 0)
        {
            unsigned char prev = sw;
            sw = (~PINC >> 4) & 3;
            sw_flag = sw ^ prev;
        }
    }
}

ISR(PCINT1_vect)
{
    wait = 10;
}

ISR(TIMER0_COMPA_vect)
{
    update_led();
    update_sw();
    if (bz_conf)
    {
        PORTD ^= 0x08;
    }
}

ISR(TIMER1_COMPA_vect)
{
    TCCR1B = 0;
    TCNT1 = 0;

    bz_conf = 0;
    PORTD &= ~0x08;
}

int main()
{
    DDRB = 0xFF;
    DDRC = 0x0F;
    DDRD = 0xFE;
    PORTB = 0x00;
    PORTC = 0x3F;
    PORTD = 0xF0;

    PCMSK1 = 0x30;
    PCICR = _BV(PCIE1);

    TCNT0 = 0;
    TCCR0A = 2;
    TCCR0B = 3;
    OCR0A = 249;
    TIMSK0 |= _BV(OCIE0A);

    TCNT1 = 0;
    32 TCCR1A = 0;
    TCCR1B = 0;
    OCR1A = 6249;
    g char pat[12][8] = {
        {1, 2, 4, 8, 16, 32, 64, 128},
        {128, 64, 32, 16, 8, 4, 2, 1}};

    TIMSK1 |= _BV(OCIE1A);

    sei();
    for (;;)
    {
        wdt_reset();
        if (sw_flag)
        {
            if (sw_flag & 1)
            {
                for (int i = 0; i < 8; i++)
                {
                    led[i] ^= pat[0][i];
                }
            }
            if (sw_flag & 2)
            {
                for (int i = 0; i < 8; i++)
                {
                    led[i] ^= pat[1][i];
                }
            }
            sw_flag = 0;
            bz_conf = 1;
            TCCR1B = 0x0c;
        }
    }
    return 0;
}