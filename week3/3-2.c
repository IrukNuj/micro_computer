#include <stdlib.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

volatile unsigned char wait = 0;
volatile unsigned char sw, sw_flag;
volatile unsigned char led_flag;

unsigned char led[8];
unsigned char ltor[8];
unsigned char rtol[8];

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
}

ISR(TIMER1_COMPA_vect)
{
    led_flag = 1;
}

int main()
{
    unsigned char n = 0, i = 0;
    DDRB = 0xFF;
    DDRC = 0x0F;
    DDRD = 0xFE;
    PORTB = 0x00;
    PORTC = 0x3F;
    PORTD = 0xF0;

    PCMSK1 = 0x30;
    PCICR = _BV(PCIE1);

    TCCR0A = 2;
    TCCR0B = 3;
    OCR0A = 249;
    TIMSK0 |= _BV(OCIE0A);

    TCCR1A = 0;
    TCCR1B = 0x0c;
    OCR1A = 3149;
    TIMSK1 |= _BV(OCIE1A);

    sei();
    int cnt2 = 0;
    for (;;)
    {
        cnt2++;
        if (cnt2 > 100)
        {
            cnt2 = 0;
            wdt_reset();
            if (sw_flag)
            {
                if (sw_flag & sw & 1)
                {
                    n = rand() % 8;
                    ltor[n] |= 0x80;
                }
                if (sw_flag & sw & 2)
                {
                    n = rand() % 8;
                    rtol[n] |= 0x01;
                    n++;
                    n &= 7;
                }
                sw_flag = 0;
            }
            if (led_flag)
            {
                for (i = 0; i < 8; i++)
                {
                    ltor[i] >>= 1;
                    rtol[i] <<= 1;
                    led[i] = ltor[i] ^ rtol[i];
                }
                led_flag = 0;
            }
        }
    }

    return 0;
}
