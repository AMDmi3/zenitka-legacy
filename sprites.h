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
