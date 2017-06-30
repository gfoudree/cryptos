#include <screen.h>
#include <sysctl.h>
#include <serial.h>
#include <ioports.h>

unsigned char *videoPtr;
int csr_x = 0, csr_y = 0, curr_line = 0;

void k_cls(void)
{
	int i;
	for (i = 0; i < COLUMNS*LINES*2; i++)
	{
		*(videoPtr + i) = 0;
	}
}

unsigned int k_printfEx(const char *message, unsigned int line, unsigned int mode)
{
	unsigned int i = 0;
	i = (line*80*2);
	while (*message != 0)
	{
		if (*message == '\n')
		{
			line++;
			i = (line*80*2);
			*message++;
		}
		else
		{
			videoPtr[i] = *message;
			*message++;
			i++;
			videoPtr[i] = mode;
			i++;
		}
	}
	return 1;
}

void k_printf(const char *message, unsigned int mode)
{
	//Check for serial debugging
        extern int *p_sysctl;
        if (p_sysctl[COM_DEBUG] > 0)
        {
                sendStr(COM1, message);
								sendByte(COM1, 10);
        }

	unsigned int i = 0;
	i = (curr_line*80*2);
	while (*message != 0)
	{
		if (*message == '\n')
		{
			curr_line++;
			i = (curr_line*80*2);
			*message++;
			csr_y++;
		}
		else
		{
			videoPtr[i] = *message;
			*message++;
			i++;
			videoPtr[i] = mode;
			i++;

			csr_x++;
			update_cursor(csr_y, csr_x);

		}
	}
	curr_line++;
	csr_y++;
	csr_x = 0;
}
/*
signed int k_printhex(unsigned int hexvalue, unsigned int mode)
{
	signed int hextemp;
	char zerocount = 1;
	int counter;
	k_printf(" 0x", mode);
	for (counter = 28; counter > 0; counter -=4)
	{
		hextemp = (hexvalue >> counter) & 0xF;
		if (hextemp == 0 && zerocount != 0)
		{
			continue;
		}
		if (hextemp >= 0xA)
		{
			zerocount = 0;
			k_printf(hextemp-0xA+'a', mode);
		}
		else
		{
			zerocount = 0;
			k_printf(hextemp+'0', mode);
		}
	}
	hextemp = hexvalue & 0xF;
	if (hextemp >= 0xA)
	{
		k_printf(hextemp-0xA+'a', mode);
	}
	else
	{
		k_printf((char)hextemp+'0', mode);
	}
	return hextemp;
}
*/
void k_printdec(unsigned int value, unsigned int mode)
{
	unsigned int temp = value;
	int i = 0, j = 0;
	char chra[32], chrb[32];
	if (value == 0)
	{
		k_printf("0", mode);
	}
	while (temp > 0)
	{
		chra[i] = '0' + temp%10;
		temp /= 10;
		i++;
	}
	chra[i] = 0;
	chrb[i--] = 0;
	while (i >= 0)
	{
		chrb[i--] = chra[j++];
	}
	k_printf(chrb, mode);
}

void update_cursor(int row, int col)
{
  unsigned short position=(row*80) + col;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (unsigned char)(position&0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned char )((position>>8)&0xFF));
}

void init_video(void)
{
	videoPtr = (unsigned char *)0xB8000;
	k_cls();
}
