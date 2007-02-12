#ifndef __ZENITKA_ERROR_H__
#define __ZENITKA_ERROR_H__

#ifdef DEBUG
void debug(const char *fmt, ...);
#else
#define debug(format, args...)
#endif

void warning(const char *fmt, ...);
void fatal(const char *fmt, ...);

#endif
