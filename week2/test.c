// Knight flasher 


#include<avr/io.h>
#include<avr/wdt.h>

#define CTOP 150000UL

int main()
{
	unsigned long cnt = 0;
    unsigned char n =0;

	DDRB = 0xFF;
	DDRC = 0x0F;
	DDRD = 0xFE;

	PORTB = 0xFF;
	PORTC = 0x00;
	PORTD = 0x00;

    unsigned char dir=1;
	for (;;) {
		wdt_reset();
		cnt++;
		if (cnt >= CTOP) {
			cnt = 0;
            n+=dir;
            if(n==9){
                dir=-1;
            }
            if(n==0){
                dir=1;
            }
            PORTB=(0x0007 << n ) >> 2; 
		}
	}
	return 0;
}
