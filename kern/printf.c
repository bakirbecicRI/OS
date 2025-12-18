// Simple implementation of cprintf console output for the kernel,
// based on printfmt() and the kernel console's cputchar().

#include <inc/types.h>
#include <inc/stdio.h>
#include <inc/stdarg.h>
#include "console.h"

static void
putch_color(int c, void *data)
{
  uint8_t color = *(uint8_t *)data;
  cons_putc_color(c, color);
}

static void
putch(int ch, int *cnt)
{
	cputchar(ch);
	*cnt++;
}

int
vcprintf(const char *fmt, va_list ap)
{
	int cnt = 0;

	vprintfmt((void*)putch, &cnt, fmt, ap);
	return cnt;
}

int colored_vcprintf(uint8_t col, const char *fmt, va_list ap){
  int cnt=0;
  vprintfmt(putch_color, &col, fmt, ap);
  return cnt;
}
int
cprintf(const char *fmt, ...)
{
	va_list ap;
	int cnt;

	va_start(ap, fmt);
	cnt = vcprintf(fmt, ap);
	va_end(ap);

	return cnt;
}


int colored_cprintf(uint8_t coll, const char *fmt, ...)
{
  va_list ap;
  int cnt;

  va_start(ap,fmt);
  cnt = colored_vcprintf(coll, fmt, ap);
  va_end(ap);

  return cnt;

}
