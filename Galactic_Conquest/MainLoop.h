#ifndef MAINLOOP_
#define MAINLOOP_
#define TRUE  1	
#define FALSE 0	


#include <GL/gl.h>
#include "Starsystem.h"
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

class MainLoop {
	
public:
	MainLoop();
	int Loop();
	void handleEvent();
	GLuint getTextureIndex( const char *filename );
	
//	void Init();
//	
//	void reshape(int width, int height);
//	
//	void draw(void);
};

#endif /*MAINLOOP_*/
