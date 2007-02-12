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

#ifndef __ZENITKA_SPRITES_H__
#define __ZENITKA_SPRITES_H__

#include <SDL.h>

extern SDL_Surface	*g_SurfaceScreen;

extern SDL_Surface	*g_SurfaceBackground;

extern SDL_Surface	*g_SurfaceBomb;
extern SDL_Surface	*g_SurfaceBrick;
extern SDL_Surface	*g_SurfaceBullet[3];
extern SDL_Surface	*g_SurfaceGrass[3];
extern SDL_Surface	*g_SurfaceGun[2];
extern SDL_Surface	*g_SurfaceHeli[6];
extern SDL_Surface	*g_SurfaceHouse[2];
extern SDL_Surface	*g_SurfaceMeat[6];
extern SDL_Surface	*g_SurfaceParafall;
extern SDL_Surface	*g_SurfaceParafly[8];
extern SDL_Surface	*g_SurfaceParaland[9];
extern SDL_Surface	*g_SurfaceParaopen[6];
extern SDL_Surface	*g_SurfacePararun[16];
extern SDL_Surface	*g_SurfaceParasmoke[4];
extern SDL_Surface	*g_SurfacePlane[2];
extern SDL_Surface	*g_SurfaceTractor[4];
extern SDL_Surface	*g_SurfaceWreckage[10];

void	DrawSprite(SDL_Surface *source, float x, float y);
void	DrawRotatedSprite(SDL_Surface *source, float x, float y, float cx, float cy, float angle);

int	LoadBackground(char *filename);
void	CleanupBackground();

int	LoadSprites();
void	CleanupSprites();

#endif
