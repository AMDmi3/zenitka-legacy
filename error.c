/*                                                                                                                                                                                                  
 * zenitka - excessive Paratrooper clone                                                                                                                                                            
 * Copyright (C) 2000-2002 Ilia Piljev <ell_1@mail.ru>                                                                                                                                              
 *               2006-2007 Dmitry Marakasov <amdmi3@amdmi3.ru>                                                                                                                                      
 *                                                                                                                                                                                                  
 * This program is free software; you can redistribute it and/or modify                                                                                                                             
 * it under the terms of the GNU General Public License as published by                                                                                                                             
 * the Free Software Foundation; either version 2 of the License, or                                                                                                                                
 * (at your option) any later version.                                                                                                                                                              
 *                                                                                                                                                                                                  
 * This program is distributed in the hope that it will be useful,                                                                                                                                  
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                                                                                                                                   
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                                                                                                                                    
 * GNU General Public License for more details.                                                                                                                                                     
 *                                                                                                                                                                                                  
 * You should have received a copy of the GNU General Public License along                                                                                                                          
 * with this program; if not, write to the Free Software Foundation, Inc.,                                                                                                                          
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                                                                                                                                      
 */

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
