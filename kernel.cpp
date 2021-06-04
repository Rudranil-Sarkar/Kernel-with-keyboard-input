#include "types.h"
#include "color.h"
#include "gdt.h"
#include "interrupthandle.h"

#define maxcol 80
#define maxrow 25

int terminalCol = 0;
int terminalRow = 0;

uint16_t* screen_buffer = (uint16_t*) 0xB8000;

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
	for(constructor* i = &start_ctors; i != &end_ctors; i++)
	{
		(*i)();
	}
}

void init()
{
	for(int i = 0; i < maxcol; i++)
	{
		for(int j = 0; j < maxrow; j++)
		{
			int index = (maxcol * j) + i;
			screen_buffer[index] = ((uint16_t) 0x0F << 8) | ' ';
		}
	}
}

void term_putc(const char c, int bcolor, int fcolor)
{
	switch(c)
	{
		case '\n':
			terminalCol = 0;
			terminalRow++;
			break;

		default:
			{
				int index = (maxcol * terminalRow) + terminalCol;
				uint8_t term_color = (bcolor << 4) | fcolor;
				screen_buffer[index] = ((uint16_t) term_color << 8) | c;
				terminalCol++;
				break;
			}
	}

	if(terminalCol >= maxcol)
	{
		terminalCol = 0;
		terminalRow++;
	}
	
	if(terminalRow >= maxrow)
	{
		terminalRow = 0;
		terminalCol = 0;
	}
}

void print(const char * str, int bcolor, int fcolor)
{
	for(unsigned int i = 0; str[i] != '\0'; i++)
		term_putc(str[i], bcolor, fcolor);
}

extern "C" void KernelMain(void)
{
	init();
	print("\n==========================[ LOADING WOLF KERNEL ]=====================\n\n", BLACK, GREEN);
	print("==========================[ SUCCESSFULLY LOADED ]=====================\n\n", BLACK, GREEN);
	GlobalDescriptorTable gdt;

	InterruptHandler inttr(&gdt);
	
	inttr.Activate();
	while(1);
}
