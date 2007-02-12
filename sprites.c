#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rotozoom.h>
#include <math.h>

#include "sprites.h"
#include "global.h"

SDL_Surface	*g_SurfaceScreen = 0;

SDL_Surface	*g_SurfaceBackground = 0;

SDL_Surface	*g_SurfaceBomb = 0;
SDL_Surface	*g_SurfaceBrick = 0;
SDL_Surface	*g_SurfaceBullet[3] = { 0, 0, 0 };
SDL_Surface	*g_SurfaceGrass[3] = { 0, 0, 0 };
SDL_Surface	*g_SurfaceGun[2] = { 0, 0 };
SDL_Surface	*g_SurfaceHeli[6] = { 0, 0, 0, 0, 0, 0 };
SDL_Surface	*g_SurfaceHouse[2] = { 0, 0 };
SDL_Surface	*g_SurfaceMeat[6] = { 0, 0, 0, 0, 0, 0 };
SDL_Surface	*g_SurfaceParafall = 0;
SDL_Surface	*g_SurfaceParafly[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
SDL_Surface	*g_SurfaceParaland[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
SDL_Surface	*g_SurfaceParaopen[6] = { 0, 0, 0, 0, 0, 0 };
SDL_Surface	*g_SurfacePararun[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
SDL_Surface	*g_SurfaceParasmoke[4] = { 0, 0, 0, 0 };
SDL_Surface	*g_SurfacePlane[2] = { 0, 0 };
SDL_Surface	*g_SurfaceTractor[4] = { 0, 0, 0, 0 };
SDL_Surface	*g_SurfaceWreckage[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

/* sprite rendering */
void DrawSprite(SDL_Surface *source, float x, float y) {
	SDL_Rect rect;
	rect.x = (int)roundf(x);
	rect.y = (int)roundf(y);
	rect.w = source->w;
	rect.h = source->h;

	SDL_BlitSurface(source, 0, g_SurfaceScreen, &rect);
}

void DrawRotatedSprite(SDL_Surface *source, float x, float y, float cx, float cy, float angle) {
	SDL_Rect rect;
	SDL_Surface *temp;

	float sina = sin(angle);
	float cosa = cos(angle);

	float dx = (float)source->w * cx;
	float dy = (float)source->h * cy;

	if ((temp = rotozoomSurface(source, -angle/M_PI*180.0, 1.0, 0)) == 0) {
		warning("Cannot render rotated sprite: %s\n", SDL_GetError());
		return;
	}

	if (sina >= 0.0)
		x -= sina*(float)source->h;
	else
		y -= -sina*(float)source->w;

	if (cosa < 0.0) {
		x -= -cosa*(float)source->w;
		y -= -cosa*(float)source->h;
	}

	x -= dx*cosa - dy*sina;
	y -= dx*sina + dy*cosa;

	rect.x = (int)roundf(x);
	rect.y = (int)roundf(y);
	rect.w = temp->w;
	rect.h = temp->h;
	SDL_BlitSurface(temp, 0, g_SurfaceScreen, &rect);

	SDL_FreeSurface(temp);
}

/* background loading/cleanup */
void CleanupBackground() {
	if (g_SurfaceBackground) {
		SDL_FreeSurface(g_SurfaceBackground);
		g_SurfaceBackground = 0;
	}
}

int LoadBackground(char *filename) {
	char		buf[FILENAME_MAX];

	CleanupBackground();

	if (snprintf(buf, sizeof(buf), "%s/backgrounds/%s", DATADIR, filename) >= sizeof(buf))
		warning("Cannot load background %s/backgrounds/%s (path too long)\n", DATADIR, filename);

	if ((g_SurfaceBackground = IMG_Load(buf)) == 0)
		warning("Cannot load background %s/backgrounds/%s ()\n", DATADIR, filename, SDL_GetError());

	return g_SurfaceBackground == 0 ? 0 : 1;
}

/* single sprite loading/cleanup */
void CleanupSprite(SDL_Surface **surface) {
	if (*surface) {
		SDL_FreeSurface(*surface);
		*surface = 0;
	}
}

int LoadSprite(SDL_Surface **surface, char *filename) {
	char		buf[FILENAME_MAX];

	CleanupSprite(surface);

	if (snprintf(buf, sizeof(buf), "%s/sprites/%s", DATADIR, filename) >= sizeof(buf))
		warning("Cannot load sprite %s/sprites/%s (path too long)\n", DATADIR, filename);

	if ((*surface = IMG_Load(buf)) == 0)
		warning("Cannot load sprite %s/sprites/%s (%s)\n", DATADIR, filename, SDL_GetError());

	return (*surface == 0) ? 0 : 1;
}

/* all game sprites loading/cleanup */
int LoadSprites() {
	int nloaded = 0;

	nloaded += LoadSprite(&g_SurfaceBomb, "bomb.png");

	nloaded += LoadSprite(&g_SurfaceBrick, "brick.png");

	nloaded += LoadSprite(&g_SurfaceBullet[0], "bullet0.png");
	nloaded += LoadSprite(&g_SurfaceBullet[1], "bullet1.png");
	nloaded += LoadSprite(&g_SurfaceBullet[2], "bullet2.png");

	nloaded += LoadSprite(&g_SurfaceGrass[0], "grass0.png");
	nloaded += LoadSprite(&g_SurfaceGrass[1], "grass1.png");
	nloaded += LoadSprite(&g_SurfaceGrass[2], "grass2.png");

	nloaded += LoadSprite(&g_SurfaceGun[0], "gun0.png");
	nloaded += LoadSprite(&g_SurfaceGun[1], "gun1.png");

	nloaded += LoadSprite(&g_SurfaceHeli[0], "helil0.png");
	nloaded += LoadSprite(&g_SurfaceHeli[1], "helil1.png");
	nloaded += LoadSprite(&g_SurfaceHeli[2], "helil2.png");
	nloaded += LoadSprite(&g_SurfaceHeli[3], "helir0.png");
	nloaded += LoadSprite(&g_SurfaceHeli[4], "helir1.png");
	nloaded += LoadSprite(&g_SurfaceHeli[5], "helir2.png");

	nloaded += LoadSprite(&g_SurfaceHouse[0], "house0.png");
	nloaded += LoadSprite(&g_SurfaceHouse[1], "house1.png");

	nloaded += LoadSprite(&g_SurfaceMeat[0], "meat0.png");
	nloaded += LoadSprite(&g_SurfaceMeat[1], "meat1.png");
	nloaded += LoadSprite(&g_SurfaceMeat[2], "meat2.png");
	nloaded += LoadSprite(&g_SurfaceMeat[3], "meat3.png");
	nloaded += LoadSprite(&g_SurfaceMeat[4], "meat4.png");

	nloaded += LoadSprite(&g_SurfaceParafall, "parafall.png");

	nloaded += LoadSprite(&g_SurfaceParafly[0], "parafly0.png");
	nloaded += LoadSprite(&g_SurfaceParafly[1], "parafly1.png");
	nloaded += LoadSprite(&g_SurfaceParafly[2], "parafly2.png");
	nloaded += LoadSprite(&g_SurfaceParafly[3], "parafly3.png");
	nloaded += LoadSprite(&g_SurfaceParafly[4], "parafly4.png");
	nloaded += LoadSprite(&g_SurfaceParafly[5], "parafly5.png");
	nloaded += LoadSprite(&g_SurfaceParafly[6], "parafly6.png");
	nloaded += LoadSprite(&g_SurfaceParafly[7], "parafly7.png");

	nloaded += LoadSprite(&g_SurfaceParaland[0], "paraland0.png");
	nloaded += LoadSprite(&g_SurfaceParaland[1], "paraland1.png");
	nloaded += LoadSprite(&g_SurfaceParaland[2], "paraland2.png");
	nloaded += LoadSprite(&g_SurfaceParaland[3], "paraland3.png");
	nloaded += LoadSprite(&g_SurfaceParaland[4], "paraland4.png");
	nloaded += LoadSprite(&g_SurfaceParaland[5], "paraland5.png");
	nloaded += LoadSprite(&g_SurfaceParaland[6], "paraland6.png");
	nloaded += LoadSprite(&g_SurfaceParaland[7], "paraland7.png");
	nloaded += LoadSprite(&g_SurfaceParaland[8], "paraland8.png");

	nloaded += LoadSprite(&g_SurfaceParaopen[0], "paraopen0.png");
	nloaded += LoadSprite(&g_SurfaceParaopen[1], "paraopen1.png");
	nloaded += LoadSprite(&g_SurfaceParaopen[2], "paraopen2.png");
	nloaded += LoadSprite(&g_SurfaceParaopen[3], "paraopen3.png");
	nloaded += LoadSprite(&g_SurfaceParaopen[4], "paraopen4.png");
	nloaded += LoadSprite(&g_SurfaceParaopen[5], "paraopen5.png");

	nloaded += LoadSprite(&g_SurfacePararun[0], "pararunr0.png");
	nloaded += LoadSprite(&g_SurfacePararun[1], "pararunr1.png");
	nloaded += LoadSprite(&g_SurfacePararun[2], "pararunr2.png");
	nloaded += LoadSprite(&g_SurfacePararun[3], "pararunr3.png");
	nloaded += LoadSprite(&g_SurfacePararun[4], "pararunr4.png");
	nloaded += LoadSprite(&g_SurfacePararun[5], "pararunr5.png");
	nloaded += LoadSprite(&g_SurfacePararun[6], "pararunr6.png");
	nloaded += LoadSprite(&g_SurfacePararun[7], "pararunr7.png");
	nloaded += LoadSprite(&g_SurfacePararun[8], "pararunl0.png");
	nloaded += LoadSprite(&g_SurfacePararun[9], "pararunl1.png");
	nloaded += LoadSprite(&g_SurfacePararun[10], "pararunl2.png");
	nloaded += LoadSprite(&g_SurfacePararun[11], "pararunl3.png");
	nloaded += LoadSprite(&g_SurfacePararun[12], "pararunl4.png");
	nloaded += LoadSprite(&g_SurfacePararun[13], "pararunl5.png");
	nloaded += LoadSprite(&g_SurfacePararun[14], "pararunl6.png");
	nloaded += LoadSprite(&g_SurfacePararun[15], "pararunl7.png");

	nloaded += LoadSprite(&g_SurfaceParasmoke[0], "parasmoke0.png");
	nloaded += LoadSprite(&g_SurfaceParasmoke[1], "parasmoke1.png");
	nloaded += LoadSprite(&g_SurfaceParasmoke[2], "parasmoke2.png");
	nloaded += LoadSprite(&g_SurfaceParasmoke[3], "parasmoke3.png");

	nloaded += LoadSprite(&g_SurfacePlane[0], "planel.png");
	nloaded += LoadSprite(&g_SurfacePlane[1], "planer.png");

	nloaded += LoadSprite(&g_SurfaceTractor[0], "tractor0.png");
	nloaded += LoadSprite(&g_SurfaceTractor[1], "tractor1.png");
	nloaded += LoadSprite(&g_SurfaceTractor[2], "tractor2.png");
	nloaded += LoadSprite(&g_SurfaceTractor[3], "tractor3.png");

	nloaded += LoadSprite(&g_SurfaceWreckage[0], "heliwreckage0.png");
	nloaded += LoadSprite(&g_SurfaceWreckage[1], "heliwreckage1.png");
	nloaded += LoadSprite(&g_SurfaceWreckage[2], "heliwreckage2.png");
	nloaded += LoadSprite(&g_SurfaceWreckage[3], "heliwreckage3.png");
	nloaded += LoadSprite(&g_SurfaceWreckage[4], "heliwreckage4.png");
	nloaded += LoadSprite(&g_SurfaceWreckage[5], "planewreckage0.png");
	nloaded += LoadSprite(&g_SurfaceWreckage[6], "planewreckage1.png");
	nloaded += LoadSprite(&g_SurfaceWreckage[7], "planewreckage2.png");
	nloaded += LoadSprite(&g_SurfaceWreckage[8], "planewreckage3.png");
	nloaded += LoadSprite(&g_SurfaceWreckage[9], "planewreckage4.png");

	return (nloaded == 83);
}

void CleanupSprites() {
	CleanupSprite(&g_SurfaceBomb);

	CleanupSprite(&g_SurfaceBrick);

	CleanupSprite(&g_SurfaceBullet[0]);
	CleanupSprite(&g_SurfaceBullet[1]);
	CleanupSprite(&g_SurfaceBullet[2]);

	CleanupSprite(&g_SurfaceGrass[0]);
	CleanupSprite(&g_SurfaceGrass[1]);
	CleanupSprite(&g_SurfaceGrass[2]);

	CleanupSprite(&g_SurfaceGun[0]);
	CleanupSprite(&g_SurfaceGun[1]);

	CleanupSprite(&g_SurfaceHeli[0]);
	CleanupSprite(&g_SurfaceHeli[1]);
	CleanupSprite(&g_SurfaceHeli[2]);
	CleanupSprite(&g_SurfaceHeli[3]);
	CleanupSprite(&g_SurfaceHeli[4]);
	CleanupSprite(&g_SurfaceHeli[5]);

	CleanupSprite(&g_SurfaceHouse[0]);
	CleanupSprite(&g_SurfaceHouse[1]);

	CleanupSprite(&g_SurfaceMeat[0]);
	CleanupSprite(&g_SurfaceMeat[1]);
	CleanupSprite(&g_SurfaceMeat[2]);
	CleanupSprite(&g_SurfaceMeat[3]);
	CleanupSprite(&g_SurfaceMeat[4]);

	CleanupSprite(&g_SurfaceParafall);

	CleanupSprite(&g_SurfaceParafly[0]);
	CleanupSprite(&g_SurfaceParafly[1]);
	CleanupSprite(&g_SurfaceParafly[2]);
	CleanupSprite(&g_SurfaceParafly[3]);
	CleanupSprite(&g_SurfaceParafly[4]);
	CleanupSprite(&g_SurfaceParafly[5]);
	CleanupSprite(&g_SurfaceParafly[6]);
	CleanupSprite(&g_SurfaceParafly[7]);

	CleanupSprite(&g_SurfaceParaland[0]);
	CleanupSprite(&g_SurfaceParaland[1]);
	CleanupSprite(&g_SurfaceParaland[2]);
	CleanupSprite(&g_SurfaceParaland[3]);
	CleanupSprite(&g_SurfaceParaland[4]);
	CleanupSprite(&g_SurfaceParaland[5]);
	CleanupSprite(&g_SurfaceParaland[6]);
	CleanupSprite(&g_SurfaceParaland[7]);
	CleanupSprite(&g_SurfaceParaland[8]);

	CleanupSprite(&g_SurfaceParaopen[0]);
	CleanupSprite(&g_SurfaceParaopen[1]);
	CleanupSprite(&g_SurfaceParaopen[2]);
	CleanupSprite(&g_SurfaceParaopen[3]);
	CleanupSprite(&g_SurfaceParaopen[4]);
	CleanupSprite(&g_SurfaceParaopen[5]);

	CleanupSprite(&g_SurfacePararun[0]);
	CleanupSprite(&g_SurfacePararun[1]);
	CleanupSprite(&g_SurfacePararun[2]);
	CleanupSprite(&g_SurfacePararun[3]);
	CleanupSprite(&g_SurfacePararun[4]);
	CleanupSprite(&g_SurfacePararun[5]);
	CleanupSprite(&g_SurfacePararun[6]);
	CleanupSprite(&g_SurfacePararun[7]);
	CleanupSprite(&g_SurfacePararun[8]);
	CleanupSprite(&g_SurfacePararun[9]);
	CleanupSprite(&g_SurfacePararun[10]);
	CleanupSprite(&g_SurfacePararun[11]);
	CleanupSprite(&g_SurfacePararun[12]);
	CleanupSprite(&g_SurfacePararun[13]);
	CleanupSprite(&g_SurfacePararun[14]);
	CleanupSprite(&g_SurfacePararun[15]);

	CleanupSprite(&g_SurfaceParasmoke[0]);
	CleanupSprite(&g_SurfaceParasmoke[1]);
	CleanupSprite(&g_SurfaceParasmoke[2]);
	CleanupSprite(&g_SurfaceParasmoke[3]);

	CleanupSprite(&g_SurfacePlane[0]);
	CleanupSprite(&g_SurfacePlane[1]);

	CleanupSprite(&g_SurfaceTractor[0]);
	CleanupSprite(&g_SurfaceTractor[1]);
	CleanupSprite(&g_SurfaceTractor[2]);
	CleanupSprite(&g_SurfaceTractor[3]);

	CleanupSprite(&g_SurfaceWreckage[0]);
	CleanupSprite(&g_SurfaceWreckage[1]);
	CleanupSprite(&g_SurfaceWreckage[2]);
	CleanupSprite(&g_SurfaceWreckage[3]);
	CleanupSprite(&g_SurfaceWreckage[4]);
	CleanupSprite(&g_SurfaceWreckage[5]);
	CleanupSprite(&g_SurfaceWreckage[6]);
	CleanupSprite(&g_SurfaceWreckage[7]);
	CleanupSprite(&g_SurfaceWreckage[8]);
	CleanupSprite(&g_SurfaceWreckage[9]);
}
