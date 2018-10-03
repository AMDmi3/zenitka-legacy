/*                                                                                                                                                                                                  
 * zenitka - excessive Paratrooper clone                                                                                                                                                            
 * Copyright (C) 2000-2002 Ilya Pilev <ell_1@mail.ru>                                                                                                                                              
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

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "main.h"
#include "sprites.h"
#include "global.h"

int	g_ButtonLeft = 0;
int	g_ButtonRight = 0;
int	g_ButtonFire = 0;

void InitGame() {
	srand(time(NULL));

	/* frame_delay = 30 */
	iPoleWidth = 640;
	iPoleHeight = 480;

	BackGround.state = 0;
	BackGround.freq = 20;
	BackGround.count = 0;

	Zenitka.height = 100;
	Zenitka.width = 100;
	Zenitka.state = 0;
	Zenitka.gun.width = 20;
	Zenitka.gun.height = 80;
	Zenitka.gun.w = 0;
	Zenitka.gun.state = 0;
	Zenitka.gun.condition = FreeForFire;

	Zenitka.x = iPoleWidth/2 - Zenitka.width/2;
	Zenitka.y = iPoleHeight - Zenitka.height;
	Zenitka.centerx = Zenitka.x + Zenitka.width/2;
	/* XXX: wtf?! */
	Zenitka.centery = Zenitka.x + Zenitka.width/2;
	Zenitka.gun.x = iPoleWidth/2;
	Zenitka.gun.y = iPoleHeight - Zenitka.height + 30;

	/* TRACTOR */
	Tractor.height = 91;
	Tractor.width = 140;
	Tractor.y = iPoleWidth-Tractor.height;
	Tractor.x = -200;
	Tractor.state = 1;
	Tractor.speed = 4;
	Tractor.freq = 3;
	Tractor.counter = 1 /*0*/;
	Tractor.count = 0;
	Tractor.interval = 500;

	int i;
	for (i = 0; i < MaxBullets; i++) {
		Bullet[i].condition = FreeForFire;
		Bullet[i].radius = 7;
	}

	for (i = 0; i < MaxHelicopters; i++) {
		Helicopter[i].condition = FreeForFlight;
		Helicopter[i].width = 122;
		Helicopter[i].height = 64;
		Helicopter[i].state = 0;
		Helicopter[i].count = 0; 
		Helicopter[i].vintFreq = 1;
	}

	for (i = 0; i < MaxPlanes; i++) {
		Plane[i].condition = FreeForFlight;
		Plane[i].width = 167;
		Plane[i].height = 52;
		/* Plane[i].state = 0;
		Plane[i].count = 0; */
	}

	iCurrentWreckage = 0;
	for (i = 0; i < MaxWreckages*MaxFragments; i++) {
		Wreckage[i].condition = FreeForFlight;
	}

	for (i = 0; i < MaxParaTroopers; i++) {
		Paratrooper[i].condition = ParaFree;
		Paratrooper[i].speedx = 0;
		Paratrooper[i].speedy = 0;
		Paratrooper[i].width = 0;
		Paratrooper[i].height = 0;
		Paratrooper[i].state = 0;
		Paratrooper[i].count = 0; 
		Paratrooper[i].openfreq = 6;
		Paratrooper[i].flyfreq = 5 + rand() % 3; /* XXX: is this correct for delphi's rand(3)? */
	}

	for (i = 0; i < MaxLandTroopers; i++) {
		Landtrooper[i].condition = ParaFree;
		Landtrooper[i].speedx = 0;
		Landtrooper[i].speedy = 0;
		Landtrooper[i].width = 18;
		Landtrooper[i].height = 35;
		Landtrooper[i].state = 0;
		Landtrooper[i].count = 0;
		Landtrooper[i].landingfreq = 3;
		Landtrooper[i].smokingfreq = 8;
		Landtrooper[i].runfreq = 3;
	}

	for (i = 0; i < MaxBombs; i++) {
		Bomb[i].condition = BombFree;
		Bomb[i].speedx = 0;
		Bomb[i].speedy = 0;
		Bomb[i].speedw = 10;
		Bomb[i].width = 50;
		Bomb[i].height = 20;
		Bomb[i].w = 0;
	}

	Statistics.HKilled = 0;
	Statistics.PKilled = 0;
	Statistics.BFired = 0;
	Statistics.Counter = 0;
	Statistics.KP10s = 0;
	Statistics.All = 0;

	FM.counter = 0;
	FM.movie = 0;

	/* Main settings */
	iLevel = 1;
	iHelicoptersFreq = 50 /* 40 */;
	iHelicoptersCount = 0;
	iPlanesFreq = 0;
	iPlanesCount = 0;
	iParatroopersFreq = 40 /* 30 */; /* !!!!!!!!!!!1 */
	iParatroopersCount = 0;
	iBombFreq = 50;
	iBombCount = 0;

	if (0) {	// if frmMenu.chkLastLevel.state=cbChecked then begin
		iHelicoptersFreq = iHelicoptersFreq-9*3;
		/* iPlanesFreq = iPlanesFreq-9*3; */
		iParatroopersFreq = iParatroopersFreq-9*3;
		iHelicoptersCount = 0;
		iParatroopersCount = 0;
		iParaFlySpeed = 1.9;
		iLevel = 4;
		Tractor.interval = 2000;
	}

	iFireFreq = 16;
	iGunWSpeed = 8;
	iParaFlySpeed = 1.6;
	iParaFallSpeed = 8;
	iHelicopterSpeed = 5;
	iPlaneSpeed = 8;
	iBulletSpeed = 12; /* 15 */
	iParaFallSpeedBorder = 3;
	iBombSpeed = 9;

	/* iBorder1 = -20;
	iBorder2 = Zenitka.x-80;
	iBorder3 = Zenitka.x+Zenitka.width-20;
	iBorder4 = iPoleWidth-80; */
	iBorder1 = 10;
	iBorder2 = Zenitka.x; /* Zenitka.x; */
	iBorder3 = Zenitka.x; /* Zenitka.x+Zenitka.width;*/
	iBorder4 = iPoleWidth-130;

	bTheEnd = 0;
}

void HelicopterCreate() {
	int i;

	iHelicoptersCount++;
	if (iHelicoptersCount == iHelicoptersFreq) {
		iHelicoptersCount = 0;

		for (i = 0; i < MaxHelicopters && Helicopter[i].condition != FreeForFlight; i++);
		if (i == MaxHelicopters)
			return;

		Helicopter[i].condition = Flying;

		if (rand() & 1) {
			Helicopter[i].direction = HDirectionRight;
			Helicopter[i].x = -Helicopter[i].width;
			Helicopter[i].y = 5;
			Helicopter[i].state = HMaxStates;
		} else {
			Helicopter[i].direction = HDirectionLeft;
			Helicopter[i].x = iPoleWidth;
			Helicopter[i].y = 100;
			Helicopter[i].state = 0;
		}
	}
	Statistics.All++;
}

void PlaneCreate() {
	int i;
	iPlanesCount++;
	if (iPlanesCount == iPlanesFreq) {
		iPlanesCount = 0;

		for (i = 0; i < MaxPlanes && Plane[i].condition != FreeForFlight; i++);
		if (i == MaxPlanes)
			return;

		Plane[i].condition = Flying;

		if (rand() & 1) {
			Plane[i].direction = PDirectionRight;
			Plane[i].x = -Plane[i].width;
			Plane[i].y = 50;
		} else {
			Plane[i].direction = PDirectionLeft;
			Plane[i].x = iPoleWidth;
			Plane[i].y = 150;
		}
		Statistics.All++;
	}
}

void BombCreate(int p) {
	int i;

	for (i = 0; i < MaxBombs && Bomb[i].condition != BombFree && Bomb[i].condition != DownOnGround; i++);
	if (i == MaxBombs)
		return;

	/* XXX: independent from direction, is this ok? */
	Bomb[i].x = Plane[p].x + 83;
	Bomb[i].y = Plane[p].y + 35;

	if (Plane[p].direction == PDirectionLeft)
		Bomb[i].w = 126;
	else
		Bomb[i].w = 2;

	Bomb[i].speedx = (int)(cos((float)Bomb[i].w/128.0*3.14)*iBombSpeed + 0.5);
	Bomb[i].speedy = (int)(sin((float)Bomb[i].w/128.0*3.14)*iBombSpeed + 0.5);
	Bomb[i].speedw = 2;

	Bomb[i].condition = BombFly;
	Bomb[i].realy = Bomb[i].y;
	Bomb[i].realx = Bomb[i].x;

	Bomb[i].direction = Plane[p].direction;
	//PlaySound(Bomb[i].x,'gunfire');
	//playsound(Bomb[i].x,'bomba_down');	{bomba_up}
	Statistics.All++;
}

void PlaneMove() {
	int i;
	for (i = 0; i < MaxPlanes; i++) {
		if (Plane[i].condition == Flying) {
			if (Plane[i].direction == PDirectionRight) {
				Plane[i].x += iPlaneSpeed;
				if ((Plane[i].x < 29 + iPlaneSpeed) && (Plane[i].x > 30))
					BombCreate(i);
			} else {
				Plane[i].x -= iPlaneSpeed;
				if ((Plane[i].x + Plane[i].width > iPoleWidth - 29 - iPlaneSpeed) && (Plane[i].x + Plane[i].width < iPoleWidth - 30))
					BombCreate(i);
			}

			DrawSprite(g_SurfacePlane[Plane[i].direction], Plane[i].x, Plane[i].y);
		}
	}
}

void ParatrooperCreate(int h) {
	int i;
	iParatroopersCount++;

	if (iParatroopersCount == iParatroopersFreq) {
		iParatroopersCount = 0;
		if (((Helicopter[h].x > iBorder1) && (Helicopter[h].x < iBorder2)) || ((Helicopter[h].x > iBorder3) && (Helicopter[h].x < iBorder4))) {
			for (i = 0; i < MaxParaTroopers && Paratrooper[i].condition != ParaFree; i++);
			if (i == MaxParaTroopers)
				return;

			if (Helicopter[h].direction == HDirectionLeft) {
				Paratrooper[i].x = Helicopter[h].x + 40;
				Paratrooper[i].speedx = -iHelicopterSpeed;
			} else {
				Paratrooper[i].x = Helicopter[h].x + 62;
				Paratrooper[i].speedx = iHelicopterSpeed;
			}
			Paratrooper[i].y = Helicopter[h].y+28;

			Paratrooper[i].state = 0;
			Paratrooper[i].condition = ParaOpen;
			Paratrooper[i].speedy = -6;
			Paratrooper[i].realy = Paratrooper[i].y; 

			//PlaySound(ParaTrooper[i].x,'jump');	{jump}

			Statistics.All++;
		}
	}
}

void ParatrooperMove() {
	int i;
	for (i = 0; i < MaxParaTroopers; i++) {
		if (Paratrooper[i].condition == ParaOpen) {
			Paratrooper[i].y = (int)(Paratrooper[i].realy + 0.5);

			DrawSprite(g_SurfaceParaopen[Paratrooper[i].state], Paratrooper[i].x, Paratrooper[i].y);

			Paratrooper[i].x += Paratrooper[i].speedx;
			Paratrooper[i].realy += Paratrooper[i].speedy;

			if (Paratrooper[i].speedy < iParaFallSpeedBorder)
				Paratrooper[i].speedy += G;

			Paratrooper[i].count++;
			if (Paratrooper[i].count >= Paratrooper[i].openfreq) {
				Paratrooper[i].count = 0;

				if (Paratrooper[i].speedx != 0)
					Paratrooper[i].speedx /= 2;

				Paratrooper[i].width = g_SurfaceParaopen[Paratrooper[i].state]->w;
				Paratrooper[i].height = g_SurfaceParaopen[Paratrooper[i].state]->h;

				Paratrooper[i].state++;

				if (Paratrooper[i].state > MaxParaOpenState) {
					Paratrooper[i].condition = ParaFly;
					Paratrooper[i].state = 0;

					Paratrooper[i].width = g_SurfaceParafly[0]->w;
					Paratrooper[i].height = g_SurfaceParafly[0]->h;
				}
			}
		} else if (Paratrooper[i].condition == ParaFly) {
			Paratrooper[i].y = (int)(Paratrooper[i].realy + 0.5);

			DrawSprite(g_SurfaceParafly[Paratrooper[i].state], Paratrooper[i].x, Paratrooper[i].y);

			Paratrooper[i].realy += iParaFlySpeed;

			Paratrooper[i].count++;

			if (Paratrooper[i].count >= Paratrooper[i].flyfreq) {
				Paratrooper[i].count = 0;
				Paratrooper[i].state++;

				if (Paratrooper[i].state == MaxParaFlyState) {
					Paratrooper[i].state = 0;
				}
			}
		} else if (Paratrooper[i].condition == ParaFall) {
			Paratrooper[i].y = (int)(Paratrooper[i].realy + 0.5);

			DrawSprite(g_SurfaceParafall, Paratrooper[i].x, Paratrooper[i].y);

			Paratrooper[i].realy += iParaFallSpeed;

			Paratrooper[i].count++;
		}
	}
}

void HelicopterMove() {
	int i;
	for (i = 0; i < MaxHelicopters; i++) {
		if (Helicopter[i].condition == Flying) {
			Helicopter[i].state++;
			if (Helicopter[i].state >= Helicopter[i].direction + HMaxStates)
				Helicopter[i].state = Helicopter[i].direction;

			if (Helicopter[i].direction == HDirectionRight)
				Helicopter[i].x += iHelicopterSpeed;
			else
				Helicopter[i].x -= iHelicopterSpeed;

			DrawSprite(g_SurfaceHeli[Helicopter[i].state], Helicopter[i].x, Helicopter[i].y);

			ParatrooperCreate(i);
		}
	}
}

void GrassMove() {
	BackGround.count++;
	if (BackGround.count == BackGround.freq) {
		BackGround.count = 0;
		BackGround.state++;
		if (BackGround.state >= MaxGrasses)
			BackGround.state = 0;
	}
	DrawSprite(g_SurfaceGrass[BackGround.state == 3 ? 1 : BackGround.state], 0, iPoleHeight - GrassHeight);
}

void BackgroundMove() {
	DrawSprite(g_SurfaceBackground, 0, 0);
}

void MeatMove() {
	int i;
	for (i = 0; i < MaxPieces*MaxMeats; i++) {
		if (Meat[i].condition == Flying) {
			Meat[i].speedy = Meat[i].speedy + G;
			if (abs(Meat[i].speedx) > 3)
				Meat[i].speedx /= 2;
			Meat[i].y += Meat[i].speedy;
			Meat[i].x += Meat[i].speedx;
			Meat[i].angle += Meat[i].anglespeed;

			DrawRotatedSprite(g_SurfaceMeat[Meat[i].piecetip], Meat[i].x, Meat[i].y, 0.5, 0.5, Meat[i].angle/128.0*3.15);
		} else if (Meat[i].condition == DownOnGround) {
			DrawRotatedSprite(g_SurfaceMeat[Meat[i].piecetip], Meat[i].x, Meat[i].y, 0.5, 0.5, Meat[i].angle/128.0*3.15);
		}
	}
}

void ZenitkaFire() {
	int i;
	/* if (Zenitka.gun.condition=FreeForFire) { */
	Zenitka.gun.condition = Firing;

	for (i = 0; i < MaxBullets && Bullet[i].condition != FreeForFire; i++);
	if (i == MaxBullets)
		return;

	Bullet[i].condition = Flying;
	Bullet[i].x = iPoleWidth / 2 + (int)(sin((float)Zenitka.gun.w/128.0*3.14)*60.0 + 0.5);
	Bullet[i].y = iPoleHeight - Zenitka.height + 30 - (int)(cos((float)Zenitka.gun.w/128.0*3.14)*60.0 + 0.5);
	Bullet[i].speedx = (int)((float)sin(Zenitka.gun.w/128.0*3.14)*(float)iBulletSpeed + 0.5);
	Bullet[i].speedy = -(int)((float)cos(Zenitka.gun.w/128.0*3.14)*(float)iBulletSpeed + 0.5);
	//PlaySound(iPoleWidth div 2,'GunFire');
	Statistics.BFired++;
	//}
}

void ZenitkaMove() {
	int AngleBorder=64-8;

	Zenitka.gun.counter++;

	if (FM.movie != 4) {
		if (FM.movie < 2) {
			if (g_ButtonLeft)
				if (Zenitka.gun.w > -AngleBorder)
					Zenitka.gun.w -= iGunWSpeed;
			if (g_ButtonRight)
				if (Zenitka.gun.w < AngleBorder)
					Zenitka.gun.w += iGunWSpeed;
			if (g_ButtonFire) {
				/*Zenitka.gun.condition = FreeForFire; */

				if ((Zenitka.gun.counter > iFireFreq) && (Zenitka.gun.condition == Firing)) {
					ZenitkaFire();
					Zenitka.gun.state = 1;
					Zenitka.gun.counter = 0;
				}
				if (Zenitka.gun.condition == FreeForFire) {
					ZenitkaFire();
					Zenitka.gun.state = 1;
					Zenitka.gun.counter = 0;
				}
			}
		}

		if (!g_ButtonFire)
			Zenitka.gun.condition = FreeForFire;

		DrawRotatedSprite(g_SurfaceGun[Zenitka.gun.state], Zenitka.gun.x, Zenitka.gun.y, 0.5, 0.8, (float)Zenitka.gun.w/128.0*3.14);
		DrawSprite(g_SurfaceHouse[Zenitka.state], Zenitka.x, Zenitka.y);

		Zenitka.gun.state = 0;
	}
}

void BulletMove() {
	int i;
	for (i = 0; i < MaxBullets; i++) {
		if (Bullet[i].condition == Flying) {
			DrawSprite(g_SurfaceBullet[0], Bullet[i].x - Bullet[i].radius, Bullet[i].y - Bullet[i].radius);

			Bullet[i].x += Bullet[i].speedx;
			Bullet[i].y += Bullet[i].speedy;
		}
	}
}

int BulletGet(int x1, int y1, int x2, int y2, int bul) {
	return ((x1 < Bullet[bul].x) && (x2 > Bullet[bul].x) && (y1 < Bullet[bul].y) && (y2 > Bullet[bul].y));
}

void HWreckageCreate(int h) {
	int i, j;
	iCurrentWreckage = iCurrentWreckage+1;

	if (iCurrentWreckage >= MaxWreckages)
		iCurrentWreckage = 0;

	i = iCurrentWreckage*MaxFragments;

	for (j = 0; j < MaxFragments; j++) {
//		Wreckage[i+j].width = g_Sprites[j < 5 ? SPRITE_HELIWRECKAGE0 + j : SPRITE_PLANEWRECKAGE0 + j - 5]->w;
//		Wreckage[i+j].height = g_Sprites[j < 5 ? SPRITE_HELIWRECKAGE0 + j : SPRITE_PLANEWRECKAGE0 + j - 5]->h;

		Wreckage[i+j].x = Helicopter[h].x + Helicopter[h].width / 2;
		Wreckage[i+j].y = Helicopter[h].y + Helicopter[h].height / 2;
		Wreckage[i+j].speedy = -rand() % 20;
		Wreckage[i+j].anglespeed = rand() % 20;
		Wreckage[i+j].condition = Flying;
		Wreckage[i+j].fragmenttip = j;
		if (Helicopter[h].direction == HDirectionLeft) {
			Wreckage[i+j].angle = 0;
			Wreckage[i+j].speedx = -rand() % 5;
		} else {
			Wreckage[i+j].angle = 100;
			Wreckage[i+j].speedx = rand() % 5;
		}
	}
}

void PWreckageCreate(int p) {
	int i, j;

	iCurrentWreckage = iCurrentWreckage + 1;

	if (iCurrentWreckage >= MaxWreckages)
		iCurrentWreckage = 0;

	i = iCurrentWreckage*MaxFragments;

	for (j = 0; j < MaxFragments; j++) {
//		Wreckage[i+j].width = g_SurfaceWreckage[MaxFragments+j]->w;
//		Wreckage[i+j].height = g_SurfaceWreckage[MaxFragments+j]->h;

		Wreckage[i+j].x = Plane[p].x + Plane[p].width / 2;
		Wreckage[i+j].y = Plane[p].y + Plane[p].height / 2;
		Wreckage[i+j].speedy = -rand() % 20;
		Wreckage[i+j].anglespeed = rand() % 30;
		Wreckage[i+j].condition = Flying;
		Wreckage[i+j].fragmenttip = MaxFragments+j;

		if (Plane[p].direction == PDirectionLeft) {
			Wreckage[i+j].angle = 0;
			Wreckage[i+j].speedx = -rand() % 10;
		} else {
			Wreckage[i+j].angle = 100;
			Wreckage[i+j].speedx = rand() % 10;
		}
	}
}

void WreckageMove() {
	int i;
	for (i = 0; i < MaxFragments*MaxWreckages; i++) {
		if (Wreckage[i].condition == Flying) {
			Wreckage[i].speedy = Wreckage[i].speedy + G;

			Wreckage[i].y += Wreckage[i].speedy;
			Wreckage[i].x += Wreckage[i].speedx;
			Wreckage[i].angle = Wreckage[i].angle + Wreckage[i].anglespeed;

			DrawRotatedSprite(g_SurfaceWreckage[Wreckage[i].fragmenttip], Wreckage[i].x, Wreckage[i].y, 0.5, 0.5, Wreckage[i].angle/128.0*3.14);
		} else if (Wreckage[i].condition == DownOnGround) {
			DrawRotatedSprite(g_SurfaceWreckage[Wreckage[i].fragmenttip], Wreckage[i].x, Wreckage[i].y, 0.5, 0.5, Wreckage[i].angle/128.0*3.14);
		}
	}
}

void HelicopterExplosion(int i) {
	Helicopter[i].state = Helicopter[i].direction;
	//PlaySound(Helicopter[i].x,'Explosion');
	HWreckageCreate(i);

	Helicopter[i].condition = FreeForFlight;

	Statistics.HKilled++;
	Statistics.KP10s++;
}

void PlaneExplosion(int i) {
	//PlaySound(Plane[i].x,'Explosion');
	PWreckageCreate(i);
	Plane[i].condition = FreeForFlight;

	Statistics.HKilled++;	/* XXX: !!! */
	Statistics.KP10s++;
}

void BombClash(int r, int b) {
	//PlaySound(Bomb[r].x,'rockclash');
	Statistics.KP10s++;
	Statistics.PKilled++;

	Bomb[r].speedw = rand() % 30;
	Bomb[r].condition = BombFall;
	Bomb[r].speedy = Bullet[b].speedy - rand() % 4;
	Bomb[r].speedx = Bullet[b].speedx - (2 - rand() % 4);
}

void MeatCreate(int p, int b) {
	int i,j;
	iCurrentMeat = iCurrentMeat+1;
	if (iCurrentMeat > MaxWreckages)
		iCurrentMeat = 1;

	i = (iCurrentMeat-1)*MaxPieces;

	if (b == 0) {
		for (j = 0; j < MaxPieces; j++) {
			Meat[i+j].x = Paratrooper[p].x;
			Meat[i+j].y = Paratrooper[p].y;
			Meat[i+j].speedy = -rand() % 15;
			Meat[i+j].angle = 0;
			Meat[i+j].anglespeed = rand() % 8;
			Meat[i+j].condition = Flying;
			Meat[i+j].piecetip = j;
			Meat[i+j].speedx = 2 - rand() % 4;
		}
	} else {
		for (j = 0; j < MaxPieces; j++) {
			Meat[i+j].x = Paratrooper[p].x + Paratrooper[p].width / 2;
			Meat[i+j].y = Paratrooper[p].y + Paratrooper[p].height / 2;
			Meat[i+j].angle = 0;
			Meat[i+j].anglespeed = rand() % 5;
			Meat[i+j].condition = Flying;
			Meat[i+j].piecetip = j;
			Meat[i+j].speedy = Bullet[b].speedy - rand() % 4;
			Meat[i+j].speedx = Bullet[b].speedx - (2 - rand() % 4);
		}
	}
}

void ParatrooperKilled(int i, int b) {
	if (Paratrooper[i].condition == ParaFly) {
		//PlaySound(ParaTrooper[i].x,'aaarch');
	}	

	if (Paratrooper[i].condition == ParaFall) {
		//PlaySound(ParaTrooper[i].x,'shlep');
	}

	Paratrooper[i].condition = ParaFree;
	MeatCreate(i,b);

	Statistics.KP10s++;
	Statistics.PKilled++;
}

void ParatrooperBallonPopped(int i) {
	Paratrooper[i].x = Paratrooper[i].x;
	Paratrooper[i].realy = Paratrooper[i].y + Paratrooper[i].height/2;
	Paratrooper[i].y = (int)(Paratrooper[i].realy + 0.5);

	Paratrooper[i].condition = ParaFall;

	// XXX: !!!!!!!!!!!
	Paratrooper[i].width = g_SurfaceParafall->w;
	Paratrooper[i].height = g_SurfaceParafall->h;

	//PlaySound(ParaTrooper[i].x,'pop');
}

void LandTrooperCreate(int p) {
	int i;

	Paratrooper[p].condition = ParaFree;

	for (i = 0; i < MaxLandTroopers && Landtrooper[i].condition != ParaFree; i++);
	if (i == MaxLandTroopers)
		return;

	Landtrooper[i].x = Paratrooper[p].x;
	Landtrooper[i].state = 0;
	Landtrooper[i].condition = Landing;
	Landtrooper[i].y = iPoleHeight - g_SurfaceParaland[Landtrooper[i].state]->h;

	//PlaySound(Landtrooper[i].x,'landing');	{landing}
}

void LandTrooperKilled(int l) {
	int i, j;
//	if (LandTrooper[L].condition==smoking)
//		PlaySound(LandTrooper[L].x,'shlep');

	Landtrooper[l].condition = ParaFree;

	Statistics.KP10s++;
	Statistics.PKilled++;

	iCurrentMeat = iCurrentMeat+1;
	if (iCurrentMeat >= MaxWreckages)
		iCurrentMeat = 1;

	i = (iCurrentMeat-1)*MaxPieces;

	for (j = 0; j < MaxPieces; j++) {
		Meat[i+j].x = Landtrooper[l].x;
		Meat[i+j].y = Landtrooper[l].y;
		Meat[i+j].speedy = -rand() % 15;
		Meat[i+j].angle = 0;
		Meat[i+j].anglespeed = rand() % 8;
		Meat[i+j].condition = Flying;
		Meat[i+j].piecetip = j;
		Meat[i+j].speedx = 2 - rand() % 4;
	}
}

void BombEnd() {
	int i;
	float c;

	FM.movie = 4;
	FM.counter = 0;
	for (i = 0; i < MaxBrick; i++) {
		c = rand() % 40;
		Brick[i].x = iPoleWidth / 2;
		Brick[i].y = iPoleHeight - 10;
		Brick[i].width = 16;
		Brick[i].height = 6;
		Brick[i].angle = rand() % 128 + 128;

		Brick[i].speedy = (int)(c*sin(Brick[i].angle/128.0*3.14) + 0.5);
		Brick[i].speedx = (int)(c*cos(Brick[i].angle/128.0*3.14) + 0.5);
		Brick[i].anglespeed = rand() % 5;
	}

	//playsound(iPoleWidth div 2,'finExplosion');
	//StopMusic('mainloop');

	bTheEnd = 1;
}

void Clash() {
	int i,j;

	/* bullet */
	for (i = 0; i < MaxBullets; i++) {
		if (Bullet[i].condition == Flying) {
			if ((Bullet[i].x > iPoleWidth) || (Bullet[i].y < 0) || (Bullet[i].x < 0)) 
				Bullet[i].condition = FreeForFire;

			/* to heli */
			for (j = 0; j < MaxHelicopters; j++)
				if (Helicopter[j].condition == Flying && BulletGet(Helicopter[j].x, Helicopter[j].y, Helicopter[j].width + Helicopter[j].x, Helicopter[j].height/2 + Helicopter[j].y, i))
					HelicopterExplosion(j);

			/* to plane */
			for (j = 0; j < MaxPlanes; j++)
				if (Plane[j].condition == Flying && BulletGet(Plane[j].x, Plane[j].y, Plane[j].width + Plane[j].x, Plane[j].height/2 + Plane[j].y, i))
					PlaneExplosion(j);

			/* to Bomb */
			for (j = 0; j < MaxBombs; j++)
				if (Bomb[j].condition == BombFly && BulletGet(Bomb[j].x - Bomb[j].width / 3, Bomb[j].y - Bomb[j].width / 3,
						(int)(Bomb[j].x + Bomb[j].width*2/3.0 + 0.5), (int)(Bomb[j].y + Bomb[j].width*2/3.0 + 0.5), i))
					BombClash(j, i);

			/* to Paratrooper */
			for (j = 0; j < MaxParaTroopers; j++)
				if (BulletGet(Paratrooper[j].x, Paratrooper[j].y, Paratrooper[j].width + Paratrooper[j].x, Paratrooper[j].height + Paratrooper[j].y, i)) {
					if (Paratrooper[j].condition == ParaFly || Paratrooper[j].condition == ParaOpen) {
						if (Bullet[i].y < (Paratrooper[j].y + 2*Paratrooper[j].height/3)) /* to parashute */
							ParatrooperBallonPopped(j);
						else /* to body */
							ParatrooperKilled(j,i);
					} else if (Paratrooper[j].condition == ParaFall)
						ParatrooperKilled(j,i);
				}
		}
	}

	/* helicopter flyaway */
	for (i = 0; i < MaxHelicopters; i++)
		if (Helicopter[i].condition == Flying && (((Helicopter[i].direction == HDirectionRight) && (Helicopter[i].x > iPoleWidth)) || ((Helicopter[i].direction == HDirectionLeft) && (Helicopter[i].x + Helicopter[i].width < 0))) )
			Helicopter[i].condition = FreeForFlight;

	/* plane flyaway */
	for (i = 0; i < MaxPlanes; i++)
		if (Plane[i].condition == Flying && (((Plane[i].direction == PDirectionRight) && (Plane[i].x > iPoleWidth)) || ((Plane[i].direction == PDirectionLeft) && (Plane[i].x + Plane[i].width < 0))) )
			 Plane[i].condition = FreeForFlight;

	for (i = 0; i < MaxBombs; i++) {
		switch(Bomb[i].condition) {
		case BombFly:
//			if ((Bomb[i].x > iPoleWidth) || (Bomb[i].y < 0) || (Bomb[i].x < 0))
//				Bomb[i].condition = FreeForFire;

			if (Bomb[i].y > iPoleHeight) {
				BombEnd();
				// PlaySound(Bomb[i].x,'finExplosion');
				Bomb[i].condition = BombFree;
			}

			break;

		case BombFall:
			/* to landtrooper */
			for (j = 0; j < MaxLandTroopers; j++) {
				if (Landtrooper[j].condition == Smoking)
					if ((Landtrooper[j].x < Bomb[i].x) && (Landtrooper[j].width+Landtrooper[j].x > Bomb[i].x) && (Landtrooper[j].y < Bomb[i].y) && (Landtrooper[j].height + Landtrooper[j].y > Bomb[i].y))
						LandTrooperKilled(j);
			}

			/* to ground */
			if (Bomb[i].y > iPoleHeight) {
				Bomb[i].realy = iPoleHeight;
				Bomb[i].speedx /= 2;
				Bomb[i].speedw /= 2;
				Bomb[i].speedy /= -2;
				if (abs(Bomb[i].speedy) > 1) {
					// PlaySound(Bomb[i].x,'Bum')
				} else {
					Bomb[i].condition = DownOnGround;
				}
			}
			break;
		}
	}

	/* paratrooper land */
	for (i = 0; i < MaxParaTroopers; i++) {
		switch (Paratrooper[i].condition) {
		case ParaFly:
			if (Paratrooper[i].y + Paratrooper[i].height > iPoleHeight)
				LandTrooperCreate(i);
			break;

		case ParaFall:
			if ((Paratrooper[i].y > iPoleHeight - 150) && (Paratrooper[i].y < iPoleHeight - 140)) {
				//PlaySound(ParaTrooper[i].x,'FallDown');
			}

			if (Paratrooper[i].y > iPoleHeight)
				if (Paratrooper[i].condition == ParaFall)
					ParatrooperKilled(i, 0);
			break;
		}
	}

	/* wreckage fall */
	for (i = 0; i < MaxWreckages * MaxFragments; i++) {
		if (Wreckage[i].condition == Flying) {
			if (Wreckage[i].y > iPoleHeight) {
				Wreckage[i].y = iPoleHeight;
				Wreckage[i].speedx /= 2;
				Wreckage[i].anglespeed /= 2;
				Wreckage[i].speedy = -(Wreckage[i].speedy / 2);
				if (abs(Wreckage[i].speedy) > 2){
					//PlaySound(wreckage[i].x,'Bum')
				} else
					Wreckage[i].condition = DownOnGround;
			}
		}
	}
}

void BombMove() {
	int i;
	for (i = 0; i < MaxBombs; i++) {
		switch(Bomb[i].condition) {
		case BombFly:
			Bomb[i].y = (int)(Bomb[i].realy + 0.5);
			Bomb[i].x = (int)(Bomb[i].realx + 0.5);

			DrawRotatedSprite(g_SurfaceBomb, Bomb[i].x, Bomb[i].y, 0.5, 0.5, Bomb[i].w/128.0*3.14);

			if (Bomb[i].direction == PDirectionRight)
				Bomb[i].targetw = 54;

			if (Bomb[i].direction == PDirectionLeft)
				Bomb[i].targetw = 74;

			if (Bomb[i].direction == PDirectionLeft)
				Bomb[i].w -= Bomb[i].speedw;

			if (Bomb[i].direction == PDirectionRight)
				Bomb[i].w += Bomb[i].speedw;

			if ((Bomb[i].w < Bomb[i].targetw + Bomb[i].speedw) && (Bomb[i].w > Bomb[i].targetw - Bomb[i].speedw))
				Bomb[i].speedw = 0;

			Bomb[i].speedx = (int)(cos((float)Bomb[i].w/128.0*3.14)*iBombSpeed);
			Bomb[i].speedy = (int)(sin((float)Bomb[i].w/128.0*3.14)*iBombSpeed);
			Bomb[i].realy += Bomb[i].speedy;
			Bomb[i].realx += Bomb[i].speedx;
			break;

		case BombFall:
			Bomb[i].y = round(Bomb[i].realy);
			Bomb[i].x = round(Bomb[i].realx);

			DrawRotatedSprite(g_SurfaceBomb, Bomb[i].x, Bomb[i].y, 0.5, 0.5, Bomb[i].w/128.0*3.14);

			Bomb[i].speedy = Bomb[i].speedy + G;
			Bomb[i].realy += Bomb[i].speedy;
			Bomb[i].realx += Bomb[i].speedx;
			Bomb[i].w += Bomb[i].speedw;
			break;

		case DownOnGround:
			DrawRotatedSprite(g_SurfaceBomb, Bomb[i].x, Bomb[i].y, 0.5, 0.5, Bomb[i].w/128.0*3.14);
			break;
		}
	}
}

void PrepareToTheEnd() {
	int i;
	for (i = 0; i < MaxHelicopters; i++)
		if (Helicopter[i].condition == Flying)
			HelicopterExplosion(i);	// XXX: SHOULD NOT COUNT SCORE

	for (i = 0; i < MaxParaTroopers; i++)
		 if (( Paratrooper[i].condition == ParaFly) || (Paratrooper[i].condition == ParaOpen))
			ParatrooperKilled(i,0);

	//StopMusic('mainloop');
	bTheEnd = 1;
}

void LandtrooperMove() {
	int i;
	for (i = 0; i < MaxLandTroopers; i++) {
		switch (Landtrooper[i].condition) {
		case Landing:
			DrawSprite(g_SurfaceParaland[Landtrooper[i].state], Landtrooper[i].x, Landtrooper[i].y);
			Landtrooper[i].count++;
			if (Landtrooper[i].count >= Landtrooper[i].landingfreq) {
				Landtrooper[i].count = 0;

				Landtrooper[i].state++;
				if (Landtrooper[i].state > MaxLandingState) {
					Landtrooper[i].y = iPoleHeight - g_SurfaceParasmoke[0]->h;
					Landtrooper[i].condition = Smoking;
					Landtrooper[i].state = 0;
					if (i >= MaxLandTroopers - 1)
						PrepareToTheEnd();
				} else
					Landtrooper[i].y = iPoleHeight - g_SurfaceParaland[Landtrooper[i].state]->h;
			}
			break;

		case Smoking:
			DrawSprite(g_SurfaceParasmoke[Landtrooper[i].state], Landtrooper[i].x, Landtrooper[i].y);
			Landtrooper[i].count++;
			if (Landtrooper[i].count >= Landtrooper[i].smokingfreq) {
				Landtrooper[i].count = 0;

				Landtrooper[i].state++;
				if (Landtrooper[i].state>MaxSmokingState)
					Landtrooper[i].state = 0;

				Landtrooper[i].y = iPoleHeight - g_SurfaceParasmoke[Landtrooper[i].state]->h;
			}
			break;
		}
	}
}

void NextLevel() {
	switch (iLevel) {
	case 1:
	case 2:
	case 3:
		iHelicoptersFreq -= 9;
		iParatroopersFreq -= 9;
		iHelicoptersCount = 0;
		iParatroopersCount = 0;
		iParaFlySpeed += 0.1;
		Tractor.interval += 500;
		break;

	case 4:
		iHelicoptersFreq = 20;
		Tractor.interval = 500;
		iPlanesCount = 0;
		iPlanesFreq = 50;
		break;

	case 5:
		iHelicoptersCount = 0;
		iHelicoptersFreq = 20;
		iParatroopersFreq -= 9;
		iHelicoptersCount = 0;
		iParatroopersCount = 0;
		iPlanesFreq = 0;
		Tractor.interval = 2000;
		break;

	case 6:
		iHelicopterSpeed++;
		iParaFlySpeed += 0.2;
		iParaFallSpeedBorder++;
		iPlanesCount = 0;
		iPlanesFreq = 130;
		break;

	case 7:
		iParaFlySpeed += 0.3;
		iPlanesCount = 0;
		iPlanesFreq = 50;
		break;

	default:
		iParaFlySpeed += 0.1;
	}

	iLevel++;
}

void TractorMove() {
	int i;

	if (Tractor.counter == 0)
		NextLevel();

	if (Tractor.counter == Tractor.interval) {
		Tractor.x = Tractor.x + Tractor.speed;
		if (Tractor.x > iPoleWidth) {
			Tractor.x = -400;
			Tractor.counter = 0;
		}

		Tractor.count++;

		if (Tractor.count == Tractor.freq) {
			Tractor.count = 0;
			//PlaySound(Tractor.x,'TrTrTr');
			Tractor.state = Tractor.state + 1;
			if (Tractor.state >= TractorMaxStates)
				Tractor.state =0;
		}

		DrawSprite(g_SurfaceTractor[Tractor.state], Tractor.x, iPoleHeight - Tractor.height);

		for (i = 0; i < MaxWreckages*MaxFragments; i++) {
			if (Wreckage[i].condition == DownOnGround) {
				if ((Wreckage[i].x >= Tractor.x + Tractor.width - 20) && (Wreckage[i].x <= Tractor.x + Tractor.width + Tractor.speed)) {
					Wreckage[i].x += Tractor.speed;
					Wreckage[i].angle += 1;
					if (Wreckage[i].y > iPoleHeight - 20) {
						Wreckage[i].x--;
						Wreckage[i].y--;
					}
				}
			}
		}

		for (i = 0; i < MaxBombs; i++) {
			if (Bomb[i].condition == DownOnGround) {
				if ((Bomb[i].x >= Tractor.x + Tractor.width - 20) && (Bomb[i].x <= Tractor.x + Tractor.width + Tractor.speed)) {
					Bomb[i].x += Tractor.speed;
					Bomb[i].w++;
					if (Bomb[i].y > iPoleHeight - 20) {
						Bomb[i].x--;
						Bomb[i].y--;
					}
				}
			}
		}
	} else 
		Tractor.counter++;
}

void FinalMovie() {
	int i,c;
	c = 0;

	BackgroundMove();
	Clash();
	//ShowStatistics();
	WreckageMove();

	switch (FM.movie) {
	case 0:
		//frmgame.DXTimer.interval = 10;
		BulletMove();
		ZenitkaMove();
		MeatMove();
		LandtrooperMove();
		FM.counter++;
		if (FM.counter == 180) {
			for (i = 0; i < MaxLandTroopers; i++) {
				Landtrooper[i].condition = Run;
				Landtrooper[i].y = Landtrooper[i].y + 4;
				if (Landtrooper[i].x > Zenitka.x + 10) {
					Landtrooper[i].dir = HDirectionLeft;
					Landtrooper[i].state = FirstRunLeftState;
				} else {
					Landtrooper[i].dir = HDirectionRight;
					Landtrooper[i].state = FirstRunRightState;
				}
			}
			FM.movie = 1;
			FM.counter = 0;
		}
		break;

	case 1:
		BulletMove();
		ZenitkaMove();
		MeatMove();

		for (i = 0; i < MaxLandTroopers; i++) {
			Landtrooper[i].count++;
			if (Landtrooper[i].count >= Landtrooper[i].runfreq) {
				Landtrooper[i].count = 0;
				Landtrooper[i].state++;
				if ((Landtrooper[i].dir == HDirectionRight) && (Landtrooper[i].state > MaxRunRightState))
					Landtrooper[i].state = FirstRunRightState;
				if ((Landtrooper[i].dir == HDirectionLeft) && (Landtrooper[i].state > MaxRunLeftState))
					Landtrooper[i].state = FirstRunLeftState;
			}

			if ((Landtrooper[i].x > Zenitka.x + 10) && (Landtrooper[i].x < Zenitka.x + 20)) {
				Zenitka.state = 1;
				c++;
			} else {
				if (Landtrooper[i].dir == HDirectionRight)
					Landtrooper[i].x += 3 + rand() % 2;
				if (Landtrooper[i].dir == HDirectionLeft)
					Landtrooper[i].x -= 3 + rand() % 2;
			}

			if (c == 4) {
				for (c = 0; c < MaxLandTroopers; c++)
					Landtrooper[c].condition = ParaFree;

				Zenitka.state = 0;
				c = 0;
				FM.movie = 2;
				FM.counter = 0;
				//playsound(iPoleWidth div 2,'fight');
			}

			DrawSprite(g_SurfacePararun[Landtrooper[i].state], Landtrooper[i].x, Landtrooper[i].y);
		}
		break;

	case 2:
		ZenitkaMove();
		FM.counter++;
		if (FM.counter > 150) {
			if (Zenitka.gun.w < 0)
				Zenitka.gun.w++;

			if (Zenitka.gun.w > 0)
				Zenitka.gun.w--;

			if (Zenitka.gun.w == 0) {
				FM.movie = 3;
				FM.counter = 0;
				c = 0;
				Zenitka.state = 1;
				Bullet[1].x = iPoleWidth / 2;
				Bullet[1].y = iPoleHeight - Zenitka.height - 30;
				for (i = 0; i < MaxLandTroopers; i++) {
					Landtrooper[i].condition = Run;
					Landtrooper[i].count = 0;
					if (Landtrooper[i].dir == HDirectionLeft)
						Landtrooper[i].state = FirstRunLeftState;
					if (Landtrooper[i].dir == HDirectionRight)
						Landtrooper[i].state = FirstRunRightState;
				}
				//playsound(iPoleWidth div 2,'urr');
				//playsound(iPoleWidth div 2,'bomba_up');
			}
		}
		break;

	case 3:
		FM.counter++;
		c = (100 - FM.counter) / 10;
		Bullet[1].y = Bullet[1].y - c;

		if (c > 0) {
			DrawSprite(g_SurfaceBullet[1], Bullet[1].x - Bullet[1].radius, Bullet[1].y - Bullet[1].radius);
			ZenitkaMove();
		} else {
			ZenitkaMove();
			DrawSprite(g_SurfaceBullet[2], Bullet[1].x - Bullet[1].radius, Bullet[1].y - Bullet[1].radius);
		}

		if (c==0) {
			//playsound(iPoleWidth div 2,'bomba_down');
		}

		for (i = 0; i < MaxLandTroopers; i++) {
			Landtrooper[i].count++;
			if (Landtrooper[i].count >= Landtrooper[i].runfreq) {
				Landtrooper[i].count = 0;
				Landtrooper[i].state++;
				if ((Landtrooper[i].dir == HDirectionRight) && (Landtrooper[i].state > MaxRunRightState))
					Landtrooper[i].state = FirstRunRightState;
				if ((Landtrooper[i].dir == HDirectionLeft) && (Landtrooper[i].state > MaxRunLeftState))
					Landtrooper[i].state = FirstRunLeftState;
			}

			if (Landtrooper[i].dir == HDirectionRight)
				Landtrooper[i].x += 3 + rand() % 3;
			if (Landtrooper[i].dir == HDirectionLeft)
				Landtrooper[i].x -= 3 + rand() % 3;

			DrawSprite(g_SurfacePararun[Landtrooper[i].state], Landtrooper[i].x, Landtrooper[i].y);
		}

		if (Bullet[1].y > iPoleHeight - Zenitka.height) {
			FM.movie = 4;
			FM.counter = 0;
			for (i = 0; i < MaxBrick; i++) {
				c = rand() % 40;
				Brick[i].x = iPoleWidth / 2;
				Brick[i].y = iPoleHeight - 10;
				Brick[i].width = 16;
				Brick[i].height = 6;
				Brick[i].angle = rand() % 128 + 128;

				Brick[i].speedy = round(c*sin(Brick[i].angle/128.0*3.14));
				Brick[i].speedx = round(c*cos(Brick[i].angle/128.0*3.14));
				Brick[i].anglespeed = rand() % 5;
			}
			//playsound(iPoleWidth div 2,'finExplosion');
			c = 0;
		}
		break;

	case 4:
		ZenitkaMove();

		FM.counter++;
		if (FM.counter == 5) {
			//playsound(iPoleWidth div 2,'finExplosion');
		}
		if (FM.counter == 70)
			FM.movie = 5;
		for (i = 0; i < MaxBrick; i++) {
			Brick[i].speedy = Brick[i].speedy + G;
			Brick[i].y += Brick[i].speedy;
			Brick[i].x += Brick[i].speedx;
			Brick[i].angle += Brick[i].anglespeed;

			DrawRotatedSprite(g_SurfaceBrick, Brick[i].x, Brick[i].y, 0.5, 0.5, Brick[i].angle/128.0*3.14);
		}
		break;

	case 5:
		exit(0);
		//PlayMusic('fin');
		break;
	}
	GrassMove();
}

void DrawFrame() {
	if (bTheEnd) {
		FinalMovie();
	} else {
		if (Tractor.counter != Tractor.interval) {
			PlaneCreate();
			HelicopterCreate();
		}

		BackgroundMove();
		ZenitkaMove();
		Clash();
		BulletMove();
		HelicopterMove();
		BombMove();
		PlaneMove();
		ParatrooperMove();
		WreckageMove();
		MeatMove();
		TractorMove();
		LandtrooperMove();
//		ShowStatistics();
		GrassMove();
	}
}

void cleanup() {
	CleanupBackground();
	CleanupSprites();

	SDL_Quit();
}

int Loop() {
	SDL_Event event;

	while (SDL_PollEvent(&event))
		switch(event.type) {
		case SDL_QUIT:
			return 0;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
			case SDLK_q:
				return 0;
			case SDLK_LEFT:
				g_ButtonLeft = event.type == SDL_KEYDOWN;
				break;
			case SDLK_RIGHT:
				g_ButtonRight = event.type == SDL_KEYDOWN;
				break;
			case SDLK_SPACE:
			case SDLK_LSHIFT:
			case SDLK_RSHIFT:
				g_ButtonFire = event.type == SDL_KEYDOWN;
				break;
			default:
				break;
			}
			break;

		default:
			break;
		}

	return 1;
}

int main(int argc, char **argv) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		return 1;
	}

	atexit(cleanup);

	if (!LoadBackground("day.jpg") || !LoadSprites()) {
		warning("Not all images loaded!\n");
		return 1;
	}

	if ((g_SurfaceScreen = SDL_SetVideoMode(640, 480, 0, SDL_HWSURFACE)) == 0) {
		warning("Couldn't set video mode: %s\n", SDL_GetError());
		return 1;
	}

	InitGame();

	int nframe = 0;
	int startticks = 0;
	int endticks = SDL_GetTicks();
	int fpslastticks = endticks;
	while (Loop()) {
		startticks = endticks;

		DrawFrame();
		SDL_UpdateRect(g_SurfaceScreen, 0, 0, 0, 0);

		endticks = SDL_GetTicks();

		if (endticks - startticks < 30)
			SDL_Delay(30 - endticks + startticks);

		endticks = SDL_GetTicks();

		if (++nframe == 50) {
			printf("FPS: %0.2f\n", 50.0*1000.0/(double)(SDL_GetTicks() - fpslastticks));
			fpslastticks = SDL_GetTicks();
			nframe = 0;
		}
	}

	return 0;
}
