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

#ifndef __ZENITKA_MAIN_H__
#define __ZENITKA_MAIN_H__

enum Direction {
	DIRECTION_LEFT,
	DIRECTION_RIGHT
};

/* types / consts */

struct TStatistics {
	int	HKilled;
	int	PKilled;
	int	BFired;
	int	Counter;
	int	KP10s;
	float	FPTime;
	int	All;
	int	lasttime;
	int	fpsH;
	int	fpsL;
	int	accuracy;
	int	FPSCounter;
	int	FPS;
};

#define StatFreq 10

struct TStatistics Statistics;

struct TGun {
	int x;
	int y;
	int w;
	int height;
	int width;
	int state;
	int condition;
	int counter;
};

struct TZenitka {
	int x;
	int y;
	int centerx;
	int centery;
	int state;
	int height;
	int width;
	struct TGun gun;
};

struct TBullet {
	int x;
	int y;
	int condition;
	int state;
	int speedx;
	int speedy;
	int radius;
};

#define FreeForFire 0
#define Flying 1
#define Firing 2
#define MaxBullets 20

struct TBackGround {
	int state;
	int freq;
	int exist;
	int count;
};

struct TTractor {
	int x;
	int y;
	int width;
	int height;
	int speed;
	int state;
	int freq;
	int counter;
	int count;
	int interval;
};

#define TractorMaxStates 4

struct THelicopter {
	int x;
	int y;
	int width;
	int height;
	int condition;
	int state;
	int direction;
	int count;
	int vintFreq;
};

#define HMaxStates 3
#define HMaxExplStates 4
#define HDirectionLeft 0
#define HDirectionRight 3
#define MaxHelicopters 6
#define FreeForFlight 0
#define DeadDown 3

struct TPlane {
	int x;
	int y;
	int width;
	int height;
	int condition;
	int state;
	int direction;
	int count;
};

#define MaxPlanes 6
#define PDirectionLeft 0
#define PDirectionRight 1
#define Explosion 2

struct TFragment {
	int x;
	int y;
	int width;
	int height;
	int speedy;
	int speedx;
	int angle;
	int anglespeed;
	int condition;
	int fragmenttip;
};

#define DownOnGround 10
#define MaxFragments 5
#define MaxWreckages 3

struct TParatrooper {
	int x;
	int y;
	float realy;
	int width;
	int height;
	int speedx;
	float speedy;
	int condition;
	int state;
	int count;
	int openfreq;
	int flyfreq;
};

#define MaxParaTroopers 8
#define ParaFree 0
#define ParaOpen 1
#define ParaFly 2
#define ParaFall 3
#define MaxParaOpenState 5
#define MaxParaFlyState 7
#define ParaFallState 8

struct TLandtrooper {
	int x;
	int y;
	int width;
	int height;
	int speedx;
	int speedy;
	int condition;
	int state;
	int count;
	int dir;
	int landingfreq;
	int smokingfreq;
	int runfreq;
};

#define MaxLandTroopers 4
#define MaxLandingState 8
#define MaxSmokingState 3
#define FirstRunRightState 0
#define MaxRunRightState 7
#define FirstRunLeftState 8
#define MaxRunLeftState 15

#define Landing 1
#define Smoking 2
#define Run 3

struct TPiece {
	int x;
	int y;
	int width;
	int height;
	int speedy;
	int speedx;
	int angle;
	int anglespeed;
	int condition;
	int piecetip;
};

#define MaxPieces 5
#define MaxMeats 3

struct TBomb {
	int x;
	int y;
	float realy;
	float realx;
	int width;
	int height;
	int speedx;
	int speedy;
	int speedw;
	int condition;
	int w;
	int targetw;
	int direction;
};

#define MaxBombs 4
#define BombFree 0
#define BombFly 2
#define BombFall 3

struct TFinalMovie {
	int counter;
	int movie;
};

struct TBrick {
	int x;
	int y;
	int width;
	int height;
	int speedy;
	int speedx;
	int angle;
	int anglespeed;
};

#define MaxBrick 400
#define Sky 0
#define OffsetGrass 1
#define MaxGrasses 4
#define GrassHeight 7
#define G 1

/* var */
struct TFinalMovie FM;
struct TBackGround BackGround;
int  iPoleHeight;
int  iPoleWidth;

int iBorder1;
int iBorder2;
int iBorder3;
int iBorder4;

int iLevel;
int iHelicoptersFreq;
int iHelicoptersCount;
int iPlanesFreq;
int iPlanesCount;
int iParatroopersFreq;
int iParatroopersCount;
int iBombFreq;
int iBombCount;

int iFireFreq;
int iGunWSpeed;
float iParaFlySpeed;
float iParaFallSpeed;
float iParaFallSpeedBorder;
int iHelicopterSpeed;
int iPlaneSpeed;
int iBulletSpeed;
int iBombSpeed;

int iImage;

int iCurrentWreckage;
int iCurrentMeat;
int iColor;
int bTheEnd;

struct TBrick Brick[MaxBrick];
struct TTractor Tractor;
struct TZenitka Zenitka;
struct TBullet Bullet[MaxBullets];
struct THelicopter Helicopter[MaxHelicopters];
struct TPlane Plane[MaxPlanes];
struct TFragment Wreckage[MaxWreckages*MaxFragments];
struct TPiece Meat[MaxMeats*MaxPieces];
struct TParatrooper Paratrooper[MaxParaTroopers];
struct TLandtrooper Landtrooper[MaxLandTroopers];
struct TBomb Bomb[MaxBombs];

#endif
