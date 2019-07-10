#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

unsigned char SOUNDS[8] = {238, 212, 189, 178, 158, 141, 126, 118};
int COUNT;

ISR(TIMER1_COMPA_vect)
{
    if (COUNT == 8)
    {
        TIMSK1 &= ~_BV(OCIE1A);
        TCCR2B = 0x00;
    }
    OCR2A = SOUNDS[COUNT];
    COUNT++;
}

ISR(PCINT1_vect)
{
    // SOUNDS[0];
    COUNT = 0;
    TIMSK1 |= _BV(OCIE1A);
    TCNT1 = 0;
    TCCR2B = 0x04;
}

int main(void)
{
    DDRC = 0x0f;
    DDRD = 0xfe;

    PORTC = 0x30;
    PORTD = 0x00;

    PCMSK1 = 0x30;
    PCICR = _BV(PCIE1);

    OCR1A = 15624;
    TCCR1A = 0;
    TCCR1B = 0x0c;
    TCNT1 = 0;

    TCCR2A = 0x12;
    OCR2A = 238;

    TIMSK1 = _BV(OCIE1A);

    sei();
    for (;;)
    {
        wdt_reset();
    }
    return 0;
}