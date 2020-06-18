#ifndef HUD_H_
#define HUD_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <SDL/SDL.h>
#include <sstream>
#include <string>
#include <stdexcept>
#include <SDL/SDL_ttf.h>

class HUD
{
public:
	HUD();
	virtual ~HUD();
	int roundUP(double x);
	int nextpoweroftwo(int x);
	void SDL_GL_RenderText(char *text, 
            TTF_Font *font,
            SDL_Color color,
            SDL_Rect *location);
	void glEnable2D();
	void glDisable2D();
	void static renderText(SDL_Surface *screen,TTF_Font* font,int r, int g, int b,
			char *text,int x, int y);
};

#endif /*HUD_H_*/
