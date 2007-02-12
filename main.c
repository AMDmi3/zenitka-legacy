#include <stdio.h>
#include <math.h>

#include "main.h"
#include "sprites.h"
#include "global.h"

int	g_ButtonLeft = 0;
int	g_ButtonRight = 0;
int	g_ButtonFire = 0;

void InitGame() {
	/* XXX: non-portable */
	sranddev();

	/* frame_delay = 30 */
	iPoleWidth = 640;
	iPoleHeight = 480;

	BackGround.State = 0;
	BackGround.Freq = 20;
	BackGround.Count = 0;

	Zenitka.height = 100;
	Zenitka.width = 100;
	Zenitka.State = 0;
	Zenitka.Gun.width = 20;
	Zenitka.Gun.height = 80;
	Zenitka.Gun.w = 0;
	Zenitka.Gun.State = 0;
	Zenitka.Gun.Condition = FreeForFire;

	Zenitka.x = iPoleWidth/2 - Zenitka.width/2;
	Zenitka.y = iPoleHeight - Zenitka.height;
	Zenitka.centerX = Zenitka.x + Zenitka.width/2;
	/* XXX: wtf?! */
	Zenitka.centerY = Zenitka.x + Zenitka.width/2;
	Zenitka.Gun.x = iPoleWidth/2;
	Zenitka.Gun.y = iPoleHeight - Zenitka.height + 30;

	/* TRACTOR */
	Tractor.height = 91;
	Tractor.width = 140;
	Tractor.y = iPoleWidth-Tractor.height;
	Tractor.x = -200;
	Tractor.State = 1;
	Tractor.speed = 4;
	Tractor.Freq = 3;
	Tractor.Counter = 1 /*0*/;
	Tractor.Count = 0;
	Tractor.Interval = 500;

	int i;
	for (i = 0; i < MaxBullets; i++) {
		Bullet[i].condition = FreeForFire;
		Bullet[i].Radius = 7;
	}

	for (i = 0; i < MaxHelicopters; i++) {
		Helicopter[i].condition = FreeForFlight;
		Helicopter[i].width = 122;
		Helicopter[i].height = 64;
		Helicopter[i].State = 0;
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
		Wreckage[i].Condition = FreeForFlight;
	}

	for (i = 0; i < MaxParaTroopers; i++) {
		Paratrooper[i].condition = ParaFree;
		Paratrooper[i].speedX = 0;
		Paratrooper[i].speedY = 0;
		Paratrooper[i].width = 0;
		Paratrooper[i].height = 0;
		Paratrooper[i].State = 0;
		Paratrooper[i].count = 0; 
		Paratrooper[i].OpenFreq = 6;
		Paratrooper[i].FlyFreq = 5 + rand() % 3; /* XXX: is this correct for delphi's rand(3)? */
	}

	for (i = 0; i < MaxLandTroopers; i++) {
		Landtrooper[i].condition = ParaFree;
		Landtrooper[i].speedX = 0;
		Landtrooper[i].speedY = 0;
		Landtrooper[i].width = 18;
		Landtrooper[i].height = 35;
		Landtrooper[i].state = 0;
		Landtrooper[i].count = 0;
		Landtrooper[i].LandingFreq = 3;
		Landtrooper[i].SmokingFreq = 8;
		Landtrooper[i].RunFreq = 3;
	}

	for (i = 0; i < MaxBombs; i++) {
		Bomb[i].condition = BombFree;
		Bomb[i].speedX = 0;
		Bomb[i].speedY = 0;
		Bomb[i].speedW = 10;
		Bomb[i].width = 50;
		Bomb[i].height = 20;
		Bomb[i].W = 0;
	}

	Statistics.HKilled = 0;
	Statistics.PKilled = 0;
	Statistics.BFired = 0;
	Statistics.Counter = 0;
	Statistics.KP10s = 0;
	Statistics.All = 0;

	FM.Counter = 0;
	FM.Movie = 0;

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
		Tractor.Interval = 2000;
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
		i = 0;
		while (Helicopter[i].condition != FreeForFlight) {
			i++;
			if (i >= MaxHelicopters)
				return;
		}

		if (rand() & 1) {
			Helicopter[i].Direction = HDirectionRight;
			Helicopter[i].x = -Helicopter[i].width;
			Helicopter[i].y = 5;
			Helicopter[i].condition = Flying;
			Helicopter[i].State = HMaxStates;
		} else {
			Helicopter[i].Direction = HDirectionLeft;
			Helicopter[i].x = iPoleWidth;
			Helicopter[i].y = 100;
			Helicopter[i].condition = Flying;
			Helicopter[i].State = 0;
		}
	}
	Statistics.All++;
}

void PlaneCreate() {
	int i;
	iPlanesCount++;
	if (iPlanesCount == iPlanesFreq) {
		iPlanesCount = 0;
		i = 0;
		while (Plane[i].condition != FreeForFlight) {
			i = i+1;
			if (i >= MaxPlanes)
				return;
		}
		switch (rand() & 1) {
		case 0:
			Plane[i].Direction = PDirectionRight;
			Plane[i].x = -Plane[i].width;
			Plane[i].y = 50;
			Plane[i].condition = Flying;
			break;
		case 1:
			Plane[i].Direction = PDirectionLeft;
			Plane[i].x = iPoleWidth;
			Plane[i].y = 150;
			Plane[i].condition = Flying;
			break;
		}
		Statistics.All++;
	}
}

void BombCreate(int p) {
	int i = 0;

	while ((Bomb[i].condition != BombFree) && (Bomb[i].condition != DownOnGround)) {
		i++;
		if (i>MaxBombs)
			return;
	}

	switch (Plane[p].Direction) {
	case PDirectionLeft:
		Bomb[i].x = Plane[p].x+83;
		Bomb[i].y = Plane[p].y+35;
		Bomb[i].W = 126;

		break;
	case PDirectionRight:
		Bomb[i].x = Plane[p].x+83;
		Bomb[i].y = Plane[p].y+35;
		Bomb[i].W = 2;
		break;
	}

	Bomb[i].speedX = (int)(cos((float)Bomb[i].W/128.0*3.14)*iBombSpeed + 0.5);
	Bomb[i].speedY = (int)(sin((float)Bomb[i].W/128.0*3.14)*iBombSpeed + 0.5);
	Bomb[i].speedW = 2;

	Bomb[i].condition = BombFly;
	Bomb[i].RealY = Bomb[i].y;
	Bomb[i].RealX = Bomb[i].x;

	Bomb[i].Direction = Plane[p].Direction;
	//PlaySound(Bomb[i].x,'gunfire');
	//playsound(Bomb[i].x,'bomba_down');	{bomba_up}
	Statistics.All++;
}

void PlaneMove() {
	int i;
	for (i = 0; i < MaxPlanes; i++) {
		if (Plane[i].condition == Flying) {
			switch (Plane[i].Direction) {
			case PDirectionRight:
				Plane[i].x = Plane[i].x+iPlaneSpeed;
				if ((Plane[i].x<29+iPlaneSpeed) && (Plane[i].x>30))
					BombCreate(i);

				break;

			case PDirectionLeft:
				Plane[i].x = Plane[i].x-iPlaneSpeed;
				if ((Plane[i].x+Plane[i].width > iPoleWidth-29-iPlaneSpeed) && (Plane[i].x+Plane[i].width<iPoleWidth-30))
					BombCreate(i);

				break;
			}

			DrawSprite(g_SurfacePlane[Plane[i].Direction], Plane[i].x, Plane[i].y);
		}
	}
}

void ParatrooperCreate(int h) {
	int i;
	iParatroopersCount++;

	if (iParatroopersCount == iParatroopersFreq) {
		iParatroopersCount = 0;
		if (((Helicopter[h].x > iBorder1) && (Helicopter[h].x<iBorder2)) || ((Helicopter[h].x>iBorder3) && (Helicopter[h].x<iBorder4))) {
			i = 0;
			while (Paratrooper[i].condition != ParaFree) {
				i++;
				if (i >= MaxParaTroopers)
					return;
			}

			switch (Helicopter[h].Direction) {
			case HDirectionLeft:
				Paratrooper[i].x = Helicopter[h].x+40;
				Paratrooper[i].y = Helicopter[h].y+28;
				Paratrooper[i].speedX = -iHelicopterSpeed;
				break;

			case HDirectionRight:
				Paratrooper[i].x = Helicopter[h].x+62;
				Paratrooper[i].y = Helicopter[h].y+28;
				Paratrooper[i].speedX = iHelicopterSpeed;
			}

			Paratrooper[i].State = 0;
			Paratrooper[i].condition = ParaOpen;
			Paratrooper[i].speedY = -6;
			Paratrooper[i].Realy = Paratrooper[i].y; 

			//PlaySound(ParaTrooper[i].x,'jump');	{jump}

			Statistics.All++;
		}
	}
}

void ParatrooperMove() {
	int i;
	for (i = 0; i < MaxParaTroopers; i++) {
		switch (Paratrooper[i].condition) {
		case ParaOpen:
			Paratrooper[i].y = (int)(Paratrooper[i].Realy + 0.5);

			DrawSprite(g_SurfaceParaopen[Paratrooper[i].State], Paratrooper[i].x, Paratrooper[i].y);

			Paratrooper[i].x = Paratrooper[i].x + Paratrooper[i].speedX;
			Paratrooper[i].Realy = Paratrooper[i].Realy + Paratrooper[i].speedY;

			if (Paratrooper[i].speedY < iParaFallSpeedBorder)
				Paratrooper[i].speedY += G;

			Paratrooper[i].count++;
			if (Paratrooper[i].count >= Paratrooper[i].OpenFreq) {
				Paratrooper[i].count = 0;

				if (Paratrooper[i].speedX != 0)
					Paratrooper[i].speedX = Paratrooper[i].speedX/2;

				Paratrooper[i].width = g_SurfaceParaopen[Paratrooper[i].State]->w;
				Paratrooper[i].height = g_SurfaceParaopen[Paratrooper[i].State]->h;

				Paratrooper[i].State++;

				if (Paratrooper[i].State > MaxParaOpenState) {
					Paratrooper[i].condition = ParaFly;
					Paratrooper[i].State = 0;

					Paratrooper[i].width = g_SurfaceParafly[0]->w;
					Paratrooper[i].height = g_SurfaceParafly[0]->h;
				}
			}
			break;

		case ParaFly:
			Paratrooper[i].y = (int)(Paratrooper[i].Realy + 0.5);

			DrawSprite(g_SurfaceParafly[Paratrooper[i].State], Paratrooper[i].x, Paratrooper[i].y);

			Paratrooper[i].Realy = Paratrooper[i].Realy + iParaFlySpeed;

			Paratrooper[i].count++;

			if (Paratrooper[i].count >= Paratrooper[i].FlyFreq) {
				Paratrooper[i].count = 0;
				Paratrooper[i].State++;

				if (Paratrooper[i].State == MaxParaFlyState) {
					Paratrooper[i].State = 0;
				}
			}
			break;

		case ParaFall:
			Paratrooper[i].y = (int)(Paratrooper[i].Realy + 0.5);

			DrawSprite(g_SurfaceParafall, Paratrooper[i].x, Paratrooper[i].y);

			Paratrooper[i].Realy = Paratrooper[i].Realy + iParaFallSpeed;

			Paratrooper[i].count++;
		}
	}
}

void HelicopterMove() {
	int i;
	for (i = 0; i < MaxHelicopters; i++) {
		if (Helicopter[i].condition == Flying) {
			Helicopter[i].State++;
			if (Helicopter[i].State >= Helicopter[i].Direction + HMaxStates) {
				Helicopter[i].State = Helicopter[i].Direction;
			}

			switch (Helicopter[i].Direction) {
			case HDirectionRight:
				Helicopter[i].x = Helicopter[i].x + iHelicopterSpeed;
				break;

			case HDirectionLeft:
				Helicopter[i].x = Helicopter[i].x - iHelicopterSpeed;
				break;
			}

			DrawSprite(g_SurfaceHeli[Helicopter[i].State], Helicopter[i].x, Helicopter[i].y);

			ParatrooperCreate(i);
		}
	}
}

void GrassMove() {
	BackGround.Count++;
	if (BackGround.Count == BackGround.Freq) {
		BackGround.Count = 0;
		BackGround.State++;
		if (BackGround.State >= MaxGrasses)
			BackGround.State = 0;
	}
	DrawSprite(g_SurfaceGrass[BackGround.State == 3 ? 1 : BackGround.State], 0, iPoleHeight - GrassHeight);
}

void BackgroundMove() {
	DrawSprite(g_SurfaceBackground, 0, 0);
}

void MeatMove() {
	int i, t;
	for (i = 0; i < MaxPieces*MaxMeats; i++) {
		if (Meat[i].Condition == Flying) {
			Meat[i].SpeedY = Meat[i].SpeedY + G;
			if (abs(Meat[i].SpeedX) > 3)
				Meat[i].SpeedX = Meat[i].SpeedX / 2;
			Meat[i].Y = Meat[i].Y+Meat[i].SpeedY;
			Meat[i].X = Meat[i].X+Meat[i].SpeedX;
			Meat[i].Angle = Meat[i].Angle+Meat[i].AngleSpeed;

			t = Meat[i].PieceTip;

			DrawRotatedSprite(g_SurfaceMeat[t], Meat[i].X, Meat[i].Y, 0.5, 0.5, Meat[i].Angle/128.0*3.15);
		} else if (Meat[i].Condition == DownOnGround) {
			t = Meat[i].PieceTip;

			DrawRotatedSprite(g_SurfaceMeat[t], Meat[i].X, Meat[i].Y, 0.5, 0.5, Meat[i].Angle/128.0*3.15);
		}
	}
}

void ZenitkaFire() {
	int i;
	/* if (Zenitka.gun.condition=FreeForFire) { */
	Zenitka.Gun.Condition = Firing;
	i = 0;
	while (Bullet[i].condition != FreeForFire) {
		i = i+1;
	}	/* Возможен глюк если чувак умудриться выпулить 15 пуль */
	Bullet[i].condition = Flying;
	Bullet[i].x = iPoleWidth / 2 + (int)(sin((float)Zenitka.Gun.w/128.0*3.14)*60.0 + 0.5);
	Bullet[i].y = iPoleHeight - Zenitka.height + 30 - (int)(cos((float)Zenitka.Gun.w/128.0*3.14)*60.0 + 0.5);
	Bullet[i].w = Zenitka.Gun.w;
	Bullet[i].SpeedX = (int)((float)sin(Bullet[i].w/128.0*3.14)*(float)iBulletSpeed + 0.5);
	Bullet[i].SpeedY = -(int)((float)cos(Bullet[i].w/128.0*3.14)*(float)iBulletSpeed + 0.5);
	//PlaySound(iPoleWidth div 2,'GunFire');
	Statistics.BFired++;
	//}
}

void ZenitkaMove() {
	int AngleBorder=64-8;

	Zenitka.Gun.counter++;

	if (FM.Movie != 4) {
		if (FM.Movie < 2) {
			if (g_ButtonLeft)
				if (Zenitka.Gun.w > -AngleBorder)
					Zenitka.Gun.w = Zenitka.Gun.w - iGunWSpeed;
			if (g_ButtonRight)
				if (Zenitka.Gun.w < AngleBorder)
					Zenitka.Gun.w = Zenitka.Gun.w + iGunWSpeed;
			if (g_ButtonFire) {
				/*Zenitka.gun.condition = FreeForFire; */

				if ((Zenitka.Gun.counter > iFireFreq) && (Zenitka.Gun.Condition == Firing)) {
					ZenitkaFire();
					Zenitka.Gun.State = 1;
					Zenitka.Gun.counter = 0;
				}
				if (Zenitka.Gun.Condition == FreeForFire) {
					ZenitkaFire();
					Zenitka.Gun.State = 1;
					Zenitka.Gun.counter = 0;
				}
			}
		}

		if (!g_ButtonFire)
			Zenitka.Gun.Condition = FreeForFire;

		DrawRotatedSprite(g_SurfaceGun[Zenitka.Gun.State], Zenitka.Gun.x, Zenitka.Gun.y, 0.5, 0.8, (float)Zenitka.Gun.w/128.0*3.14);
		DrawSprite(g_SurfaceHouse[Zenitka.State], Zenitka.x, Zenitka.y);

		Zenitka.Gun.State = 0;
	}
}

void BulletMove() {
	int i;
	for (i = 0; i < MaxBullets; i++) {
		if (Bullet[i].condition == Flying) {
			DrawSprite(g_SurfaceBullet[0], Bullet[i].x - Bullet[i].Radius, Bullet[i].y - Bullet[i].Radius);

			Bullet[i].x = Bullet[i].x + Bullet[i].SpeedX;
			Bullet[i].y = Bullet[i].y + Bullet[i].SpeedY;
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
//		Wreckage[i+j].Width = g_Sprites[j < 5 ? SPRITE_HELIWRECKAGE0 + j : SPRITE_PLANEWRECKAGE0 + j - 5]->w;
//		Wreckage[i+j].Height = g_Sprites[j < 5 ? SPRITE_HELIWRECKAGE0 + j : SPRITE_PLANEWRECKAGE0 + j - 5]->h;

		Wreckage[i+j].X = Helicopter[h].x + Helicopter[h].width / 2;
		Wreckage[i+j].Y = Helicopter[h].y + Helicopter[h].height / 2;
		Wreckage[i+j].SpeedY = -rand() % 20;
		Wreckage[i+j].AngleSpeed = rand() % 20;
		Wreckage[i+j].Condition = Flying;
		Wreckage[i+j].FragmentTip = j;
		if (Helicopter[h].Direction == HDirectionLeft) {
			Wreckage[i+j].Angle = 0;
			Wreckage[i+j].SpeedX = -rand() % 5;
			Wreckage[i+j].X = Wreckage[i+j].X;
		} else {
			Wreckage[i+j].Angle = 100;
			Wreckage[i+j].SpeedX = rand() % 5;
			Wreckage[i+j].X = Wreckage[i+j].X;
		}
	}
}

void PWreckageCreate(int p) {
	int i, j;

	iCurrentWreckage = iCurrentWreckage+1;

	if (iCurrentWreckage >= MaxWreckages)
		iCurrentWreckage = 0;

	i = iCurrentWreckage*MaxFragments;

	for (j = 0; j < MaxFragments; j++) {
//		Wreckage[i+j].Width = g_SurfaceWreckage[MaxFragments+j]->w;
//		Wreckage[i+j].Height = g_SurfaceWreckage[MaxFragments+j]->h;

		Wreckage[i+j].X = Plane[p].x+Plane[p].width / 2;
		Wreckage[i+j].Y = Plane[p].y+Plane[p].height / 2;
		Wreckage[i+j].SpeedY = -rand() % 20;
		Wreckage[i+j].AngleSpeed = rand() % 30;
		Wreckage[i+j].Condition = Flying;
		Wreckage[i+j].FragmentTip = MaxFragments+j;

		if (Plane[p].Direction == PDirectionLeft) {
			Wreckage[i+j].Angle = 0;
			Wreckage[i+j].SpeedX = -rand() % 10;
		} else {
			Wreckage[i+j].Angle = 100;
			Wreckage[i+j].SpeedX = rand() % 10;
		}
	}
}

void WreckageMove() {
	int i, t;
	for (i = 0; i < MaxFragments*MaxWreckages; i++) {
		if (Wreckage[i].Condition == Flying) {
			Wreckage[i].SpeedY = Wreckage[i].SpeedY + G;

			Wreckage[i].Y = Wreckage[i].Y+Wreckage[i].SpeedY;
			Wreckage[i].X = Wreckage[i].X+Wreckage[i].SpeedX;
			Wreckage[i].Angle = Wreckage[i].Angle+Wreckage[i].AngleSpeed;

			t = Wreckage[i].FragmentTip;

			DrawRotatedSprite(g_SurfaceWreckage[t], Wreckage[i].X, Wreckage[i].Y, 0.5, 0.5, Wreckage[i].Angle/128.0*3.14);
		} else if (Wreckage[i].Condition == DownOnGround) {
			t = Wreckage[i].FragmentTip;

			DrawRotatedSprite(g_SurfaceWreckage[t], Wreckage[i].X, Wreckage[i].Y, 0.5, 0.5, Wreckage[i].Angle/128.0*3.14);
		}
	}
}

void HelicopterExplosion(int i) {
	Helicopter[i].State = Helicopter[i].Direction;
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

	Bomb[r].speedW = rand() % 30;
	Bomb[r].condition = BombFall;
	Bomb[r].speedY = Bullet[b].SpeedY - rand() % 4;
	Bomb[r].speedX = Bullet[b].SpeedX - (2 - rand() % 4);
}

void MeatCreate(int p, int b) {
	int i,j;
	iCurrentMeat = iCurrentMeat+1;
	if (iCurrentMeat > MaxWreckages)
		iCurrentMeat = 1;

	i = (iCurrentMeat-1)*MaxPieces;

	if (b == 0) {
		for (j = 0; j < MaxPieces; j++) {
			Meat[i+j].X = Paratrooper[p].x;
			Meat[i+j].Y = Paratrooper[p].y;
			Meat[i+j].SpeedY = -rand() % 15;
			Meat[i+j].Angle = 0;
			Meat[i+j].AngleSpeed = rand() % 8;
			Meat[i+j].Condition = Flying;
			Meat[i+j].PieceTip = j;
			Meat[i+j].SpeedX = 2 - rand() % 4;
		}
	} else {
		for (j = 0; j < MaxPieces; j++) {
			Meat[i+j].X = Paratrooper[p].x + Paratrooper[p].width / 2;
			Meat[i+j].Y = Paratrooper[p].y + Paratrooper[p].height / 2;
			Meat[i+j].Angle = 0;
			Meat[i+j].AngleSpeed = rand() % 5;
			Meat[i+j].Condition = Flying;
			Meat[i+j].PieceTip = j;
			Meat[i+j].SpeedY = Bullet[b].SpeedY - rand() % 4;
			Meat[i+j].SpeedX = Bullet[b].SpeedX - (2 - rand() % 4);
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
	Paratrooper[i].Realy = Paratrooper[i].y + Paratrooper[i].height/2;
	Paratrooper[i].y = (int)(Paratrooper[i].Realy + 0.5);

	Paratrooper[i].condition = ParaFall;

	// XXX: !!!!!!!!!!!
	Paratrooper[i].width = g_SurfaceParafall->w;
	Paratrooper[i].height = g_SurfaceParafall->h;

	//PlaySound(ParaTrooper[i].x,'pop');
}

void LandTrooperCreate(int p) {
	Paratrooper[p].condition = ParaFree;

	int i = 0;
	while (Landtrooper[i].condition != ParaFree) {
		i = i+1;
		if (i >= MaxLandTroopers)
			return;
	}

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
		Meat[i+j].X = Landtrooper[l].x;
		Meat[i+j].Y = Landtrooper[l].y;
		Meat[i+j].SpeedY = -rand() % 15;
		Meat[i+j].Angle = 0;
		Meat[i+j].AngleSpeed = rand() % 8;
		Meat[i+j].Condition = Flying;
		Meat[i+j].PieceTip = j;
		Meat[i+j].SpeedX = 2 - rand() % 4;
	}
}

void BombEnd() {
	int i;
	float c;

	FM.Movie = 4;
	FM.Counter = 0;
	for (i = 0; i < MaxBrick; i++) {
		c = rand() % 40;
		Brick[i].X = iPoleWidth / 2;
		Brick[i].Y = iPoleHeight - 10;
		Brick[i].Width = 16;
		Brick[i].Height = 6;
		Brick[i].Angle = rand() % 128 + 128;

		Brick[i].SpeedY = (int)(c*sin(Brick[i].Angle/128.0*3.14) + 0.5);
		Brick[i].SpeedX = (int)(c*cos(Brick[i].Angle/128.0*3.14) + 0.5);
		Brick[i].AngleSpeed = rand() % 5;
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
				if (Helicopter[j].condition == Flying && BulletGet(Helicopter[j].x, Helicopter[j].y, Helicopter[j].width + Helicopter[j].x, Helicopter[j].height/2 + Helicopter[j].y, i)) {
					/* Bullet[i].condition = FreeForFire; */
					HelicopterExplosion(j);
				}

			/* to plane */
			for (j = 0; j < MaxPlanes; j++)
				if (Plane[j].condition == Flying && BulletGet(Plane[j].x, Plane[j].y, Plane[j].width + Plane[j].x, Plane[j].height/2 + Plane[j].y, i)) {
					/* Bullet[i].condition = FreeForFire; */
					PlaneExplosion(j);
				}

			/* to Bomb */
			for (j = 0; j < MaxBombs; j++)
				if (Bomb[j].condition == BombFly && BulletGet(Bomb[j].x - Bomb[j].width / 3, Bomb[j].y - Bomb[j].width / 3,
						(int)(Bomb[j].x + Bomb[j].width*2/3.0 + 0.5), (int)(Bomb[j].y + Bomb[j].width*2/3.0 + 0.5), i)) {
					BombClash(j, i);
				}

			/* to Paratrooper */
			for (j = 0; j < MaxParaTroopers; j++)
				if (BulletGet(Paratrooper[j].x, Paratrooper[j].y, Paratrooper[j].width + Paratrooper[j].x, Paratrooper[j].height + Paratrooper[j].y, i)) {
					switch(Paratrooper[j].condition) {
					case ParaFly:
					case ParaOpen:
						if (Bullet[i].y < (Paratrooper[j].y + 2*Paratrooper[j].height/3)) {
							/* to parashute */
							/* Bullet[i].condition = FreeForFire; */
							ParatrooperBallonPopped(j);
						} else {
							/* to body */
							/* Bullet[i].condition = FreeForFire; */
							ParatrooperKilled(j,i);
						}
						break;

					case ParaFall:
						/* Bullet[i].condition = FreeForFire; */
						ParatrooperKilled(j,i);
						break;
					}
				}
		}
	}

	/* helicopter flyaway */
	for (i = 0; i < MaxHelicopters; i++)
		if (Helicopter[i].condition == Flying && (((Helicopter[i].Direction == HDirectionRight) && (Helicopter[i].x > iPoleWidth)) || ((Helicopter[i].Direction == HDirectionLeft) && (Helicopter[i].x + Helicopter[i].width < 0))) )
			Helicopter[i].condition = FreeForFlight;

	/* plane flyaway */
	for (i = 0; i < MaxPlanes; i++)
		if (Plane[i].condition == Flying && (((Plane[i].Direction == PDirectionRight) && (Plane[i].x > iPoleWidth)) || ((Plane[i].Direction == PDirectionLeft) && (Plane[i].x + Plane[i].width < 0))) )
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
				Bomb[i].RealY = iPoleHeight;
				Bomb[i].speedX = Bomb[i].speedX / 2;
				Bomb[i].speedW = Bomb[i].speedW / 2;
				Bomb[i].speedY = -(Bomb[i].speedY / 2);
				if (abs(Bomb[i].speedY) > 1) {
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
			if (Paratrooper[i].y+Paratrooper[i].height > iPoleHeight)
				LandTrooperCreate(i);
			break;

		case ParaFall:
			if ((Paratrooper[i].y > iPoleHeight - 150) && (Paratrooper[i].y < iPoleHeight - 140)) {
				//PlaySound(ParaTrooper[i].x,'FallDown');
			}

			if (Paratrooper[i].y>iPoleHeight)
				if (Paratrooper[i].condition==ParaFall)
					ParatrooperKilled(i,0);
			break;
		}
	}

	/* wreckage fall */
	for (i = 0; i < MaxWreckages*MaxFragments; i++) {
		if (Wreckage[i].Condition == Flying) {
			if (Wreckage[i].Y > iPoleHeight) {
				Wreckage[i].Y = iPoleHeight;
				Wreckage[i].SpeedX = Wreckage[i].SpeedX / 2;
				Wreckage[i].AngleSpeed = Wreckage[i].AngleSpeed / 2;
				Wreckage[i].SpeedY = -(Wreckage[i].SpeedY / 2);
				if (abs(Wreckage[i].SpeedY) > 2){
					//PlaySound(wreckage[i].x,'Bum')
				} else
					Wreckage[i].Condition = DownOnGround;
			}
		}
	}
}

void BombMove() {
	int i;
	for (i = 0; i < MaxBombs; i++) {
		switch(Bomb[i].condition) {
		case BombFly:
			Bomb[i].y = (int)(Bomb[i].RealY + 0.5);
			Bomb[i].x = (int)(Bomb[i].RealX + 0.5);

			DrawRotatedSprite(g_SurfaceBomb, Bomb[i].x, Bomb[i].y, 0.5, 0.5, Bomb[i].W/128.0*3.14);

			if (Bomb[i].Direction == PDirectionRight)
				Bomb[i].targetW = 54;

			if (Bomb[i].Direction == PDirectionLeft)
				Bomb[i].targetW = 74;

			if (Bomb[i].Direction == PDirectionLeft)
				Bomb[i].W = Bomb[i].W - Bomb[i].speedW;

			if (Bomb[i].Direction == PDirectionRight)
				Bomb[i].W = Bomb[i].W + Bomb[i].speedW;

			if ((Bomb[i].W < Bomb[i].targetW + Bomb[i].speedW) && (Bomb[i].W > Bomb[i].targetW - Bomb[i].speedW))
				Bomb[i].speedW = 0;

			Bomb[i].speedX = (int)(cos((float)Bomb[i].W/128.0*3.14)*iBombSpeed);
			Bomb[i].speedY = (int)(sin((float)Bomb[i].W/128.0*3.14)*iBombSpeed);
			Bomb[i].RealY = Bomb[i].RealY + Bomb[i].speedY;
			Bomb[i].RealX = Bomb[i].RealX + Bomb[i].speedX;
			break;

		case BombFall:
			Bomb[i].y = round(Bomb[i].RealY);
			Bomb[i].x = round(Bomb[i].RealX);

			DrawRotatedSprite(g_SurfaceBomb, Bomb[i].x, Bomb[i].y, 0.5, 0.5, Bomb[i].W/128.0*3.14);

			Bomb[i].speedY = Bomb[i].speedY + G;
			Bomb[i].RealY = Bomb[i].RealY + Bomb[i].speedY;
			Bomb[i].RealX = Bomb[i].RealX + Bomb[i].speedX;
			Bomb[i].W = Bomb[i].W + Bomb[i].speedW;
			break;

		case DownOnGround:
			DrawRotatedSprite(g_SurfaceBomb, Bomb[i].x, Bomb[i].y, 0.5, 0.5, Bomb[i].W/128.0*3.14);
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
			if (Landtrooper[i].count >= Landtrooper[i].LandingFreq) {
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
			if (Landtrooper[i].count >= Landtrooper[i].SmokingFreq) {
				Landtrooper[i].count = 0;

				Landtrooper[i].state++;
				if (Landtrooper[i].state>MaxSmokingState) {
					Landtrooper[i].state = 0;
				}
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
		iHelicoptersFreq = iHelicoptersFreq - 9;
		iParatroopersFreq = iParatroopersFreq - 9;
		iHelicoptersCount = 0;
		iParatroopersCount = 0;
		iParaFlySpeed = iParaFlySpeed + 0.1;
		Tractor.Interval = Tractor.Interval + 500;
		break;

	case 4:
		iHelicoptersFreq = 20;
		Tractor.Interval = 500;
		iPlanesCount = 0;
		iPlanesFreq = 50;
		break;

	case 5:
		iHelicoptersCount = 0;
		iHelicoptersFreq = 20;
		iParatroopersFreq = iParatroopersFreq - 9;
		iHelicoptersCount = 0;
		iParatroopersCount = 0;
		iPlanesFreq = 0;
		Tractor.Interval = 2000;
		break;

	case 6:
		iHelicopterSpeed++;
		iParaFlySpeed = iParaFlySpeed + 0.2;
		iParaFallSpeedBorder = iParaFallSpeedBorder + 1;
		iPlanesCount = 0;
		iPlanesFreq = 130;
		break;

	case 7:
		iParaFlySpeed = iParaFlySpeed + 0.3;
		iPlanesCount = 0;
		iPlanesFreq = 50;
		break;

	default:
		iParaFlySpeed = iParaFlySpeed + 0.1;
	}

	iLevel++;
}

void TractorMove() {
	int i;

	if (Tractor.Counter == 0)
		NextLevel();

	if (Tractor.Counter == Tractor.Interval) {
		Tractor.x = Tractor.x + Tractor.speed;
		if (Tractor.x > iPoleWidth) {
			Tractor.x = -400;
			Tractor.Counter = 0;
		}

		Tractor.Count++;

		if (Tractor.Count == Tractor.Freq) {
			Tractor.Count = 0;
			//PlaySound(Tractor.x,'TrTrTr');
			Tractor.State = Tractor.State + 1;
			if (Tractor.State >= TractorMaxStates)
				Tractor.State =0;
		}

		DrawSprite(g_SurfaceTractor[Tractor.State], Tractor.x, iPoleHeight - Tractor.height);

		for (i = 0; i < MaxWreckages*MaxFragments; i++) {
			if (Wreckage[i].Condition == DownOnGround) {
				if ((Wreckage[i].X >= Tractor.x + Tractor.width - 20) && (Wreckage[i].X <= Tractor.x + Tractor.width + Tractor.speed)) {
					Wreckage[i].X = Wreckage[i].X + Tractor.speed;
					Wreckage[i].Angle = Wreckage[i].Angle + 1;
					if (Wreckage[i].Y > iPoleHeight - 20) {
						Wreckage[i].Y = Wreckage[i].Y - 1;
						Wreckage[i].X = Wreckage[i].X - 1;
					}
				}
			}
		}

		for (i = 0; i < MaxBombs; i++) {
			if (Bomb[i].condition == DownOnGround) {
				if ((Bomb[i].x >= Tractor.x + Tractor.width - 20) && (Bomb[i].x <= Tractor.x + Tractor.width + Tractor.speed)) {
					Bomb[i].x = Bomb[i].x + Tractor.speed;
					Bomb[i].W = Bomb[i].W + 1;
					if (Bomb[i].y > iPoleHeight - 20) {
						Bomb[i].y = Bomb[i].y - 1;
						Bomb[i].x = Bomb[i].x - 1;
					}
				}
			}
		}
	} else 
		Tractor.Counter++;
}

void FinalMovie() {
	int i,c;
	c = 0;

	BackgroundMove();
	Clash();
	//ShowStatistics();
	WreckageMove();

	switch (FM.Movie) {
	case 0:
		//frmgame.DXTimer.Interval = 10;
		BulletMove();
		ZenitkaMove();
		MeatMove();
		LandtrooperMove();
		FM.Counter++;
		if (FM.Counter == 180) {
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
			FM.Movie = 1;
			FM.Counter = 0;
		}
		break;

	case 1:
		BulletMove();
		ZenitkaMove();
		MeatMove();

		for (i = 0; i < MaxLandTroopers; i++) {
			Landtrooper[i].count++;
			if (Landtrooper[i].count >= Landtrooper[i].RunFreq) {
				Landtrooper[i].count = 0;
				Landtrooper[i].state++;
				if ((Landtrooper[i].dir == HDirectionRight) && (Landtrooper[i].state > MaxRunRightState))
					Landtrooper[i].state = FirstRunRightState;
				if ((Landtrooper[i].dir == HDirectionLeft) && (Landtrooper[i].state > MaxRunLeftState))
					Landtrooper[i].state = FirstRunLeftState;
			}

			if ((Landtrooper[i].x > Zenitka.x + 10) && (Landtrooper[i].x < Zenitka.x + 20)) {
				Zenitka.State = 1;
				c++;
			} else {
				if (Landtrooper[i].dir == HDirectionRight)
					Landtrooper[i].x = Landtrooper[i].x + 3 + rand() % 2;
				if (Landtrooper[i].dir == HDirectionLeft)
					Landtrooper[i].x = Landtrooper[i].x - 3 - rand() % 2;
			}

			if (c == 4) {
				for (c = 0; c < MaxLandTroopers; c++)
					Landtrooper[c].condition = ParaFree;

				Zenitka.State = 0;
				c = 0;
				FM.Movie = 2;
				FM.Counter = 0;
				//playsound(iPoleWidth div 2,'fight');
			}

			DrawSprite(g_SurfacePararun[Landtrooper[i].state], Landtrooper[i].x, Landtrooper[i].y);
		}
		break;

	case 2:
		ZenitkaMove();
		FM.Counter++;
		if (FM.Counter > 150) {
			if (Zenitka.Gun.w < 0)
				Zenitka.Gun.w++;

			if (Zenitka.Gun.w > 0)
				Zenitka.Gun.w--;

			if (Zenitka.Gun.w == 0) {
				FM.Movie = 3;
				FM.Counter = 0;
				c = 0;
				Zenitka.State = 1;
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
		FM.Counter++;
		c = (100 - FM.Counter) / 10;
		Bullet[1].y = Bullet[1].y - c;

		if (c > 0) {
			DrawSprite(g_SurfaceBullet[1], Bullet[1].x - Bullet[1].Radius, Bullet[1].y - Bullet[1].Radius);
			ZenitkaMove();
		} else {
			ZenitkaMove();
			DrawSprite(g_SurfaceBullet[2], Bullet[1].x - Bullet[1].Radius, Bullet[1].y - Bullet[1].Radius);
		}

		if (c==0) {
			//playsound(iPoleWidth div 2,'bomba_down');
		}

		for (i = 0; i < MaxLandTroopers; i++) {
			Landtrooper[i].count++;
			if (Landtrooper[i].count >= Landtrooper[i].RunFreq) {
				Landtrooper[i].count = 0;
				Landtrooper[i].state++;
				if ((Landtrooper[i].dir == HDirectionRight) && (Landtrooper[i].state > MaxRunRightState))
					Landtrooper[i].state = FirstRunRightState;
				if ((Landtrooper[i].dir == HDirectionLeft) && (Landtrooper[i].state > MaxRunLeftState))
					Landtrooper[i].state = FirstRunLeftState;
			}

			if (Landtrooper[i].dir == HDirectionRight)
				Landtrooper[i].x = Landtrooper[i].x + 3 + rand() % 3;
			if (Landtrooper[i].dir == HDirectionLeft)
				Landtrooper[i].x = Landtrooper[i].x - 3 - rand() % 3;

			DrawSprite(g_SurfacePararun[Landtrooper[i].state], Landtrooper[i].x, Landtrooper[i].y);
		}

		if (Bullet[1].y > iPoleHeight - Zenitka.height) {
			FM.Movie = 4;
			FM.Counter = 0;
			for (i = 0; i < MaxBrick; i++) {
				c = rand() % 40;
				Brick[i].X = iPoleWidth / 2;
				Brick[i].Y = iPoleHeight - 10;
				Brick[i].Width = 16;
				Brick[i].Height = 6;
				Brick[i].Angle = rand() % 128 + 128;

				Brick[i].SpeedY = round(c*sin(Brick[i].Angle/128.0*3.14));
				Brick[i].SpeedX = round(c*cos(Brick[i].Angle/128.0*3.14));
				Brick[i].AngleSpeed = rand() % 5;
			}
			//playsound(iPoleWidth div 2,'finExplosion');
			c = 0;
		}
		break;

	case 4:
		ZenitkaMove();

		FM.Counter++;
		if (FM.Counter == 5) {
			//playsound(iPoleWidth div 2,'finExplosion');
		}
		if (FM.Counter == 70)
			FM.Movie = 5;
		for (i = 0; i < MaxBrick; i++) {
			Brick[i].SpeedY = Brick[i].SpeedY + G;
			Brick[i].Y = Brick[i].Y + Brick[i].SpeedY;
			Brick[i].X = Brick[i].X + Brick[i].SpeedX;
			Brick[i].Angle = Brick[i].Angle + Brick[i].AngleSpeed;

			DrawRotatedSprite(g_SurfaceBrick, Brick[i].X, Brick[i].Y, 0.5, 0.5, Brick[i].Angle/128.0*3.14);
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
		if (Tractor.Counter != Tractor.Interval) {
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
		warning("Not all images loadede!\n");
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
