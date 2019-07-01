#include <avr/io.h>
#include <avr/wdt.h>

#define CTOP 2000UL
#define CTOP2 100UL
unsigned char pno = 0;
unsigned char qno = 0;
unsigned char led[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char sw;

unsigned char pat[2][8] = {
	{1, 2, 4, 8, 16, 32, 64, 128}, // /
	{128, 64, 32, 16, 8, 4, 2, 1}, // ＼
};

void update_sw()
{
	sw = (~PINC >> 4) & 3; //入力ピンを読み取り，スイッチ変数を更新
}

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

	// int state = 0;

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

			// led_coordinate(x, y);
			cnt2++;
			if (cnt2 > 100)
			{
				cnt2 = 0;
				update_sw();
				switch (sw)
				{ // スイッチ状態に応じたアクション
				case 0:
					for (int i = 0; i < 8; i++)
					{
						led[i] = 0;
					}
					break;
				case 1:
					for (int i = 0; i < 8; i++)
					{
						led[i] = pat[0][i];
					}
					break;
				case 2:
					for (int i = 0; i < 8; i++)
					{
						led[i] = pat[1][i];
					}
					break;
				case 3:
					for (int i = 0; i < 8; i++)
					{
						led[i] = pat[0][i] + pat[1][i];
					}
					break;
				}
			}
			return 0;
		}
	}
}
