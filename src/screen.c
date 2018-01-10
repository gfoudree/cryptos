#include <screen.h>
#include <sysctl.h>
#include <serial.h>
#include <ioports.h>

void k_cls(void)
{
	for (int i = 0; i < COLUMNS*LINES*2; i++)
	{
		*(tty.videoPtr + i) = 0;
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
			tty.videoPtr[i] = *message;
			*message++;
			i++;
			tty.videoPtr[i] = mode;
			i++;
		}
	}
	return 1;
}

void k_printf(const char *message)
{
  extern int *p_sysctl;
  if (p_sysctl[COM_DEBUG] > 0) //Check for serial debugging
  {
          sendStr(COM1, message);
					sendByte(COM1, 10);
  }

	unsigned int i = (tty.curr_line*80*2);
	while (*message != 0)
	{
		if (*message == '\n')
		{
			tty.curr_line = (tty.curr_line + 1) % 25;
			if (tty.curr_line == 0) {
				k_cls();
			}
			i = (tty.curr_line*80*2);
			*message++;
			tty.csr_y++;
		}
		else
		{
			tty.videoPtr[i] = *message;
			*message++;
			i++;
			tty.videoPtr[i] = tty.curr_color;
			i++;

			tty.csr_x++;
			update_cursor(tty.csr_y, tty.csr_x);
		}
	}
	tty.curr_line = (tty.curr_line + 1) % 25;
	if (tty.curr_line == 0) {
		k_cls();
	}
	tty.csr_y++;
	tty.csr_x = 0;
}

void k_printdec(unsigned int value)
{
	unsigned int temp = value;
	int i = 0, j = 0;
	char chra[32], chrb[32];
	if (value == 0)
	{
		k_printf("0");
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
	k_printf(chrb);
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
	tty.videoPtr = (unsigned char *)0xB8000;

	tty.csr_x = 0;
	tty.csr_y = 0;
	tty.curr_line = 0;
	tty.curr_color = lGray;

	k_cls();
}

void tty_set_color(unsigned int color) {
	tty.curr_color = color;
}
