#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
void debug(const char *fmt, ...) {
	va_list arglist;
	va_start(arglist, fmt);
	vfprintf(stderr, fmt, arglist);
	va_end(arglist);
}
#endif

void warning(const char *fmt, ...) {
	va_list arglist;
	va_start(arglist, fmt);
	vfprintf(stderr, fmt, arglist);
	va_end(arglist);
}

void fatal(const char *fmt, ...) {
	va_list arglist;
	va_start(arglist, fmt);
	vfprintf(stderr, fmt, arglist);
	va_end(arglist);

	exit(1);
}
