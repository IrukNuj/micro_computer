#include <avr/io.h>
#include <avr/wdt.h>

#define CTOP 2000UL

unsigned char sw; // スイッチ変数

unsigned char led[8] = {
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00};

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

void led_coordinate(int x, int y, int z)
{
	static int tmp_y = 0;
	// unsigned char led[8] = {};

	if (z == 0)
	{
		led[tmp_y] = 0x00;
		led[y] = 0x01 << x;
		tmp_y = y;
	}
	else
	{
		led[y] ^= 0x01 << x;
	}
}

void update_sw()
{
	sw = (~PINC >> 4) & 3; //入力ピンを読み取り，スイッチ変数を更新
}

int main() 描写
{
	unsigned long cnt = 0;

	DDRB = 0xFF;
	DDRC = 0x0F;
	DDRD = 0xFE;

	PORTB = 0xFF;
	PORTC = 0x30;
	PORTD = 0x00;

	int x = 6;
	int y = 6;
	int z = 0;
	int cnt2 = 0;

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
				z = 0;
				cnt2 = 0;
				update_sw();
				switch (sw)
				{ // スイッチ状態に応じたアクション
				case 0:
					led = 0x00;
					break;
				case 1:
					break;
				case 2:
					if (y > 0)
					{
						y--;
					}
					break;
				case 3:
					z = 1;
					break;
				}
				led_coordinate(x, y, z);
			}
		}
	}
	return 0;
}
