#include <string.h>
#include "user.h"

static unsigned char pat[8] = {
	{0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

void user_init(void)
{
}

void user_main(void)
{
	static char stat = 0, cnt = 0, n = 0;
	static char x = 1;
	if (sw_flag)
	{
		sw_flag = 0;
		switch (sw)
		{
		case 0:
			x = 1;
		case 1:
			stat = 1;
			x = 1;
			break;
		case 2:
			x = 2;
			break;
		}
	}
	switch (stat)
	{
	case 0: // タイトル画面
		if (sw_flag)
		{
			sw_flag = 0;
			switch (sw)
			{
			case 1:
				stat = 1;
				break;
			case 2:
				break;
			}
		}
		goto draw;
	case 1: // 上列
		pat[0] = pat[0] >> x;
		n += x;
		if (n >= 7)
		{
			stat = 2;
			n = 0;
		}
		goto draw;
	case 2: // 右列
		pat[n + 1] = 0x01;
		pat[n] = 0x00;
		n += x;
		if (n >= 7)
		{
			stat = 3;
			n = 0;
		}
		goto draw;
	case 3:
		pat[7] = pat[7] << 1;
		n += x;

		n++;
		if (pat[7] == 0x80)
		{
			stat = 4;
			n = 0;
		}
		goto draw;
	case 4: // 右列
		pat[6 - n] = pat[7 - n];
		pat[7 - n] = 0x00;
		n += x;
		if (pat[0] != 0x00)
		{
			stat = 1;
			n = 0;
		}
		goto draw;
	}

draw:
	memcpy(led, pat, 8);
}
