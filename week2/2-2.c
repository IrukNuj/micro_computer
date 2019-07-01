#include <avr/io.h>
#include <avr/wdt.h>

#define CTOP 2000UL
#define CTOP2 100UL
unsigned char pno = 0;
unsigned char qno = 0;
unsigned char led[8];

unsigned char pat[12][8] = {
    {0, 7, 5, 5, 5, 7, 0, 0}, //0
    {0, 1, 1, 1, 1, 1, 0, 0}, //1
    {0, 7, 1, 7, 4, 7, 0, 0}, //2
    {0, 7, 1, 7, 1, 7, 0, 0}, //3
    {0, 5, 5, 7, 1, 1, 0, 0}, //4
    {0, 7, 4, 7, 1, 7, 0, 0}, //5
    {0, 7, 4, 7, 5, 7, 0, 0}, //6
    {0, 7, 1, 1, 1, 1, 0, 0}, //7
    {0, 7, 5, 7, 5, 7, 0, 0}, //8
    {0, 7, 5, 7, 1, 7, 0, 0}, //9
    {0, 0, 0, 0, 0, 0, 8, 0}, //・
    {0, 0, 0, 0, 0, 0, 0, 7}, //-
};

void update_led()
{
    static unsigned char sc = 0xFE;
    static unsigned char scan = 0;

    PORTB = 0; // 残像対策
    sc = (sc << 1) | (sc >> 7);
    PORTD = (PORTD & 0x0F) | (sc & 0xF0); // 上位4ビット書き換え
    PORTC = (PORTC & 0xF0) | (sc & 0x0F); // 下位4ビット書き換え
    scan = (scan + 1) & 7;
    PORTB = led[scan];
}

int main()
{
    unsigned long cnt = 0, cnt2 = 0;
    unsigned char i;

    int state = 0;

    DDRB = 0xFF;
    DDRC = 0x0F;
    DDRD = 0xFE;

    PORTB = 0xFF;
    PORTC = 0x30;
    PORTD = 0x00;

    for (;;)
    {
        wdt_reset();
        cnt++;
        if (cnt >= CTOP)
        {
            cnt = 0;
            update_led();

            cnt2++;
            if (cnt2 >= CTOP2)
            {
                cnt2 = 0;
                if (state == 1)
                {
                    pno++;
                }
                if (pno >= 10)
                {
                    pno = 0;
                    qno++;
                }
                if (qno >= 10)
                {
                    qno = 0;
                }
                for (i = 0; i < 8; i++)
                {
                    led[i] = pat[pno][i];
                }
                for (i = 0; i < 8; i++)
                {
                    led[i] |= pat[qno][i] << 4;
                }
                if ((~PINC >> 4) & 0x1)
                {
                    state = 1;
                }
                if ((~PINC >> 5) & 0x1)
                {
                    if (state == 2)
                    {
                        state = 1;
                    }
                    else
                    {
                        state = 2;
                    }
                }
            }
        }
    }
    return 0;
}