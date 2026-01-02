/* See COPYRIGHT for copyright information. */

#ifndef _CONSOLE_H_
#define _CONSOLE_H_
#ifndef JOS_KERNEL
# error "This is a JOS kernel header; user programs should not #include it"
#endif

#include <inc/types.h>

#define MONO_BASE	0x3B4
#define MONO_BUF	0xB0000
#define CGA_BASE	0x3D4
#define CGA_BUF		0xB8000

#define CRT_ROWS	25
#define CRT_COLS	80
#define CRT_SIZE	(CRT_ROWS * CRT_COLS)

#define COLOR_BLUE  0x1
#define COLOR_GREEN  0x2
#define COLOR_RED  0x4
#define COLOR_BROWN 0x6
#define COLOR_LIGHTBLUE 0x9
#define COLOR_PINK 0xD
#define COLOR_YELLOW 0xE
#define COLOR_CYAN 0x3
#define COLOR_LIGHTGRAY 0x7
#define COLOR_DARKGRAY 0x8
#define COLOR_LIGHTGREEN 0xA
#define COLOR_LIGHTCYAN 0xB
#define COLOR_LIGHTRED 0xC
#define COLOR_MAGENTA 0x5


void cons_init(void);
int cons_getc(void);

int colored_cprintf(uint8_t color, const char *fmt, ...);
void cons_putc_color(int c, uint8_t color);

void kbd_intr(void); // irq 1
void serial_intr(void); // irq 4

#endif /* _CONSOLE_H_ */
