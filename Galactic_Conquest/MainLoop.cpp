#include <stdio.h>
#include <cmath>
#include <iostream>
#include "MainLoop.h"
#include "GameVariables.h"
#include "Render.h"
#include "Team.h"
#include "HUD.h"
#include "Point.h"
#include "Matrix.h"
#include "ArcBall.h"
#include "Quaternion.h"
#include <string>
#include "MilkshapeModel.h"
#include "AI.h"

using namespace std;

static Point ss[3];
Model *ship_models[2];
Model *starsystem_models[starsystemModelCount];
static const char *textureName[textureCount] = {"shiphull.bmp","mercury.bmp","sun.bmp","earth.bmp","mars.bmp","pluto.bmp","venus.bmp"};
static Quaternion worldRotation = Quaternion(0, 0, 0, 1);
static Render r;
static Team player1;
static Team player2;
static int aiEnabled;
static int max_starsystems;
static int ships_per_starsystem;
static int xx = 0;
static int yy = 0;
static ArcBall arcBall;
static GLfloat current_zoom,last_zoom = 0.0f;
static bool zooming;
static bool mouseHeldDown = false;
static Team *currentPlayer;
static Team *opposingPlayer;
static AI *computerPlayer;
static int activeStarsystem = -1;
static SDL_Surface *drawContext;
static int uniqueColour[] = { 0, 0, 0 };
static unsigned char pixels[3];
static int winningTeam;

MainLoop::MainLoop() {

}

int MainLoop::Loop() {
	return 1;
}

int* getNextUniqueColour() {
	uniqueColour[0]++;
	if (uniqueColour[0] > 255) {
		uniqueColour[0] = 0;
		uniqueColour[1]++;
		if (uniqueColour[1] > 255) {
			uniqueColour[1] = 0;
			uniqueColour[2]++;
		}
	}
	return uniqueColour;
}

void resolveConflict(Starsystem *starsystems) {
	for (int i = 0; i < max_starsystems; i++) {
		if (starsystems[i].inConflict) {
			GLfloat base_probability = 1/((GLfloat)starsystems[i].enemyShips + (GLfloat)starsystems[i].homeShips);
			GLfloat Prob_destroyAttackingShip = base_probability*starsystems[i].homeShips;
			srand((int)SDL_GetTicks());
			int randomInt = rand()%1000;
			GLfloat randomFloat = (GLfloat)randomInt/1000;

			cout << "Prob base" << base_probability << endl;
			cout << "Prob_destroyAttackingShip " << Prob_destroyAttackingShip << endl;
			cout << "Dice roll" << randomFloat << endl;

			if(randomFloat < Prob_destroyAttackingShip)
			{
				if(starsystems[i].team == currentPlayer->getTeam())
				{
					opposingPlayer->resolveConflict(&starsystems[i]);
				}
				else if(starsystems[i].team != currentPlayer->getTeam())
				{
					currentPlayer->resolveConflict(&starsystems[i]);
				}
			}
			else if(starsystems[i].team == currentPlayer->getTeam())
			{
				currentPlayer->resolveConflict(&starsystems[i]);
			}
			else if(starsystems[i].team != currentPlayer->getTeam())
			{
				opposingPlayer->resolveConflict(&starsystems[i]);
			}
		}
	}
}

int loadModels(){
	ship_models[0] = new MilkshapeModel();
	ship_models[1] = new MilkshapeModel();

	if ( ship_models[0]->loadModelData( "ship1.ms3d" ) == false ||
			ship_models[1]->loadModelData( "ship2.ms3d" ) == false)
	{
		cout << "Error loading ship models" << endl;
		return 0;
	}
	ship_models[0]->reloadTextures();
	ship_models[1]->reloadTextures();

	for(int i = 0; i < starsystemModelCount; i++)
	{
		starsystem_models[i] = new MilkshapeModel();
	}
			if ( starsystem_models[0]->loadModelData( "starsystem1.ms3d" ) == false ||
					starsystem_models[1]->loadModelData( "starsystem2.ms3d" ) == false ||
					starsystem_models[2]->loadModelData( "starsystem3.ms3d" ) == false ||
					starsystem_models[3]->loadModelData( "starsystem1_blue.ms3d" ) == false ||
					starsystem_models[4]->loadModelData( "starsystem2_blue.ms3d" ) == false ||
					starsystem_models[5]->loadModelData( "starsystem3_blue.ms3d" ) == false ||
					starsystem_models[6]->loadModelData( "starsystem1_green.ms3d" ) == false ||
					starsystem_models[7]->loadModelData( "starsystem2_green.ms3d" ) == false ||
					starsystem_models[8]->loadModelData( "starsystem3_green.ms3d" ) == false)
			{
				cout << "Error loading model starsystem1.ms3d" << endl;
				return 0;
			}
			cout << "Loading sun texture" << endl;

			for(int i = 0; i < starsystemModelCount; i++)
			{
				starsystem_models[i]->reloadTextures();
			}

}

GLuint MainLoop::getTextureIndex( const char *filename ){
	GLuint result = 0;
	for(int i = 0; i < textureCount; i++){
		if(strcmp(textureName[i],filename) == 0) {
			result = i+1;
			break;
		}
	}
	return result;
}

void loadTextures(){
	GLuint texture[textureCount];
	SDL_Surface *surface[textureCount];
	GLenum texture_format;
	GLint  nOfColors;
	glGenTextures( textureCount, &texture[0] );
	cout << "Loading texture" << endl;
	for(int i = 0; i < textureCount; i++){
		if((surface[i] = SDL_LoadBMP(textureName[i]))){

		        nOfColors = surface[i]->format->BytesPerPixel;
		        if (nOfColors == 4)
		        {
		        	if (surface[i]->format->Rmask == 0x000000ff)
		            {
		                texture_format = GL_RGBA;
		            }
		            else
		            {
		            	texture_format = GL_BGRA;
		            }
		        }
		        else if (nOfColors == 3)
		        {
		        	if (surface[i]->format->Rmask == 0x000000ff)
		        	{
		        		texture_format = GL_RGB;
		        	}
		        	else
		        	{
		        		texture_format = GL_BGR;
		        	}
		        }
		        glBindTexture( GL_TEXTURE_2D, texture[i] );
		        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		        glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface[i]->w, surface[i]->h, 0,
		                              texture_format, GL_UNSIGNED_BYTE, surface[i]->pixels );
		}
		    else {
		        cout << "Couldn't load bitmap!!" << endl;
		    }
			if ( surface ) {
				SDL_FreeSurface( surface[i] );
			}
	}
}

void selectUniqueColour(Uint16 x, Uint16 y, Uint8 button, Starsystem *starsystems) {
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0f, 0.0f, 0.0f);// Clear The Screen And The Depth Buffer

	if(button == SDL_BUTTON_LEFT)
	{
		currentPlayer->drawShips(true);
		opposingPlayer->drawShips(true);
	}
	if(button == SDL_BUTTON_RIGHT)
	{
		for (int i = 0; i < max_starsystems; i++) {
			starsystems[i].drawUniqueStarsystem();
		}
	}

	glReadPixels(x, y, 1, 1, GL_RGB,
	GL_UNSIGNED_BYTE, pixels);
	cout << "UID at pixel " << (int)pixels[0] << " " << (int)pixels[1] << " " << (int)pixels[2] << endl;
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void mouseClick(SDL_Event event, Uint8 *keys, SDLMod mod, Starsystem *starsystems) {
	//take away height from y to get cartesian coordinates since sdls
	//y coordinates are the opposite to cartesian
	Uint16 x = event.motion.x;
	Uint16 y = windowHeight-event.motion.y;
	xx = x;
	yy = y;
	glReadPixels(x, y, 1, 1, GL_RGB,
		GL_UNSIGNED_BYTE, pixels);
		cout << "Colur pixel " << (int)pixels[0] << " " << (int)pixels[1] << " " << (int)pixels[2] << endl;
	cout << x << " " << y << endl;
//	cout << "mouse down" << endl;

	if(event.button.button == SDL_BUTTON_MIDDLE){
//		cout << "Zooming" << endl;
		zooming = true;
		mouseHeldDown = true;
		last_zoom = y;
	}

	else if (mod == KMOD_LCTRL && !mouseHeldDown )
	{
		activeStarsystem = -1;
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			cout << "Selecting multiple " << endl;
			selectUniqueColour(x, y, event.button.button, starsystems);
			currentPlayer->selectShip((int)pixels[0], (int)pixels[1],
					(int)pixels[2], true);
		}
	}

	else if (mod != KMOD_LSHIFT && !mouseHeldDown) {
//		cout << "dsfsdf1" << endl;
		//Selecting ship
		if (event.button.button == SDL_BUTTON_LEFT) {
			activeStarsystem = -1;
			cout << "Selecting ship" << endl;
			selectUniqueColour(x, y, event.button.button, starsystems);
			currentPlayer->selectShip((int)pixels[0], (int)pixels[1],
					(int)pixels[2], false);
		}

		//Selecting starsystem
		else if (event.button.button == SDL_BUTTON_RIGHT) {
			cout << "Selecting star system" << endl;
			selectUniqueColour(x, y, event.button.button, starsystems);
			for (int i = 0; i < max_starsystems; i++) {
				if (starsystems[i].uniqueColour[0] == (int)pixels[0]
						&& starsystems[i].uniqueColour[1] == (int)pixels[1]
						&& starsystems[i].uniqueColour[2] == (int)pixels[2]) {
					activeStarsystem = i;
					cout << "Ordering ship" << endl;
					currentPlayer->orderMoveShip(&starsystems[activeStarsystem]);
				}
			}
		} else {
			std::cout << "Select ship first" << std::endl;
		}
	}

	//dragging world
	else if (event.button.button == SDL_BUTTON_LEFT && mod == KMOD_LSHIFT) {
		cout << "begin dragging " << endl;
		mouseHeldDown = true;
		x = windowWidth-event.motion.x;
		GLfloat scaledX = (2.0 * x - windowWidth)/windowWidth; //	scale the x & y
		GLfloat scaledY = -(2.0 * y - windowHeight)/windowHeight; //	to range [-1..1]
		arcBall.StartDrag(scaledX, scaledY);
	}
}

void mouseRelease(SDL_Event event, Uint8 *keys, SDLMod mod) {
	//take away height from y to get cartesian coordinates since sdls
	//y coordinates are the opposite to cartesian
	Uint16 x = event.motion.x;
	Uint16 y = windowHeight-event.motion.y;
	if (event.button.button == SDL_BUTTON_LEFT && mouseHeldDown) {
		cout << "mouse released" << endl;
		cout << x << " " << y << endl;
		mouseHeldDown = false;
		x = windowWidth-event.button.x;
		GLfloat scaledX = (2.0 * x - windowWidth)/windowWidth; //	scale the x & y
		GLfloat scaledY = -(2.0 * y - windowHeight)/windowHeight; //	to range [-1..1]
		worldRotation = arcBall.currentRotation; //	update the world rotation
		arcBall.EndDrag(scaledX, scaledY);
	}
	else if(event.button.button == SDL_BUTTON_MIDDLE && mouseHeldDown){
		cout << "mouse released" << endl;
		mouseHeldDown = false;
		zooming = false;
		current_zoom = 0.0f;
	}
}

void mouseDrag(SDL_Event event, Uint8 *keys, SDLMod mod) {
	Uint16 x = event.motion.x;
	Uint16 y = windowHeight-event.motion.y;
	cout << x << " " << y << endl;
		if(zooming){
			current_zoom = y - last_zoom;
			cout << "Current zoom " << current_zoom << endl;
			last_zoom = y;
		}
		else if(event.button.button == SDL_BUTTON_LEFT){
			x = windowWidth-event.button.x;
			cout << "Dragging world" << endl;
			GLfloat scaledX = (2.0 * x - windowWidth)/windowWidth; //	scale the x & y
			GLfloat scaledY = -(2.0 * y - windowHeight)/windowHeight; //	to range [-1..1]
			arcBall.UpdateDrag(scaledX, scaledY); //	update the drag
			worldRotation = arcBall.currentRotation; //	update the world rotation
		}
}

//checks all system pairs to determine one with greatest distance between them,
//these 2 systems represent the starting systems for each team
//this is inefficient as it will check pairs of systems twice, might be optimised
void maximumDistance(int *starting_systems, Starsystem *starsystems) {
	GLfloat greatest_distance = 0;
	GLfloat current_distance;
	for (int i = 0; i < max_starsystems; i++) {
		for (int j = 0; j < max_starsystems; j++) {
			//if distance between these 2 systems is greater than current greatest distance
			//then save them and update greatest distance
			if (i != j) {
				current_distance
						= starsystems[i].coordinates.distancex2(starsystems[j].coordinates);
				if (current_distance > greatest_distance) {
					greatest_distance = current_distance;
					starting_systems[0] = i;
					starting_systems[1] = j;
				}
			}
		}
	}
}

void initStarsystems(Starsystem *starsystems) {
	int x, y, z;
	int* uniqueColour;
	bool minDistance;
	bool maxDistance;
	for (int i = 0; i < max_starsystems; i++) {
		minDistance = false;
		maxDistance = false;
		while((minDistance && maxDistance) == false){
			x = (rand()%maximumOriginDistance/4)-maximumOriginDistance/8;
			y = (rand()%maximumOriginDistance/4)-maximumOriginDistance/8;
			z = (rand()%maximumOriginDistance/4)-maximumOriginDistance/8;
			cout << "x " << x << " y " << y << " z " << z << endl;
			check if its close enough to origin
			if((x*x+y*y+z*z) < maximumOriginDistance){
				maxDistance = true;
				cout << "Close enough to origin " << endl;
			}
			minDistance = true;
			for(int j = 0; j < i; j++){
				cout << " Enterning " << endl;
				GLfloat x2 = starsystems[j].coordinates.coords[0];
				GLfloat y2 = starsystems[j].coordinates.coords[1];
				GLfloat z2 = starsystems[j].coordinates.coords[2];
				cout << "x2 " << x2 << " y2 " << y2 << " z2 " << z2 << endl;
				if(((x - x2)*(x - x2) + (y - y2)*(y - y2) + (z - z2)*(z - z2)) < minimumNeighbourDistance){
					cout << "DIstance between starsystems is less than min distance " <<
					((x - x2)*(x - x2) + (y - y2)*(y - y2) + (z - z2)*(z - z2)) << endl;
					minDistance = false;
				}
				else {
					cout << "DIstance between starsystems is greater than min distance " <<
					((x - x2)*(x - x2) + (y - y2)*(y - y2) + (z - z2)*(z - z2)) << endl;
				}
			}
			}
		cout << "Starsystem created!" << endl;
		cout << "Mindistance is " << minDistance << endl;
		cout << "Maxdistance is " << maxDistance << endl;
		cout << "x " << x << " y " << y << " z " << z << endl;
		uniqueColour = getNextUniqueColour();
		int starSystemModel = rand()%3;
		starsystems[i] = Starsystem(0, (GLfloat)x, (GLfloat)y, (GLfloat)z, starsystemRadius,
				uniqueColour[0], uniqueColour[1], uniqueColour[2],
				starsystem_models[starSystemModel], starsystem_models[starSystemModel+3], starsystem_models[starSystemModel+6]);
		cout << "Loading sun texture3" << endl;
	}
}

#undef main
int main(int argc, char **argv) {
    
    std::cout << "Hello, World!\n";
	int* uniqueColour;
	bool turnFinished = false;
	bool gameOver = false;
	cout << "Please enter number of star systems." << endl;
	int input;
	cin >> input;
	cin.clear();
	max_starsystems = input;
	cout << "Please enter number of ships per star system." << endl;
	cin >> input;
	cin.clear();
	ships_per_starsystem = input;
	cout << "Enable AI? 1 = Yes, 0 = No" << endl;
	cin >> input;
	Starsystem starsystems[max_starsystems];
	int environmentVolume = 20000*max_starsystems;
	maximumOriginDistance = sqrt(environmentVolume/(0.75*PI));
	Uint32 flags;
	Uint8 *keys;
	SDLMod mod;
	TTF_Font *font;
	cout << argv[1] << endl;
	SDL_Init(SDL_INIT_VIDEO);
	if(argc > 1)
	{
		if(strcmp(argv[1],"-fullscreen") == 0)
		{
			flags = SDL_OPENGL | SDL_FULLSCREEN | SDL_HWSURFACE | SDL_ANYFORMAT;
			const SDL_VideoInfo *Info = SDL_GetVideoInfo();
			colourDepth = (int)Info->vfmt->BitsPerPixel;
			windowWidth = Info->current_w ;
			windowHeight = Info->current_h ;
		}
		else
		{
			flags = SDL_OPENGL;
		}
	}
	else
	{
		flags = SDL_OPENGL;
	}
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
	drawContext = SDL_SetVideoMode(windowWidth, windowHeight, colourDepth, flags);

	if (TTF_Init()) {
		cout << "ttf error " << TTF_GetError() << endl;
		atexit(TTF_Quit);

		return 0;
	}
	if (!(font = TTF_OpenFont("cour.ttf", 16))) {
		cout << "could not open font" << endl;
		TTF_Quit();
		SDL_Quit();
		return 0;
	}

	Team player1 = Team(0.0f, 0.0f, 1.0f, 1, ships_per_starsystem);
	Team player2 = Team(0.2f, 0.8f, 0.0f, 2, ships_per_starsystem);
	r.init();
	r.reshape(drawContext->w, drawContext->h);
	windowHeight = drawContext->h;
	windowWidth = drawContext->w;
	r.setViewMatrix(starting_position[0], starting_position[1], starting_position[2]);
	cout << "Initialised" << endl;
	currentPlayer = &player1;
	opposingPlayer = &player2;
	currentPlayer->setOpposingPlayer(opposingPlayer);
	opposingPlayer->setOpposingPlayer(currentPlayer);
	computerPlayer = new AI(&player2, starsystems, &player1, max_starsystems);
	loadTextures();
	if(loadModels() == 0){
		return 0;
	}
	initStarsystems(starsystems);
	int starting_systems[2];
	maximumDistance(starting_systems, starsystems);
	starsystems[starting_systems[0]].team = 1;
	starsystems[starting_systems[1]].team = 2;
	while (true) {
		gameOver = true;
		for (int i = 0; i < max_starsystems; i++) {
			if (starsystems[i].team == currentPlayer->getTeam()
					&& currentPlayer->getShipCount()
							< (currentPlayer->getStarsystemCount()
									*ships_per_starsystem)
									&& !starsystems[i].inConflict)
			{

				uniqueColour = getNextUniqueColour();
				if(currentPlayer->getTeam() == 1){
					currentPlayer->spawnShip(&starsystems[i], uniqueColour[0],
						uniqueColour[1], uniqueColour[2],ship_models[0]);
				}
				if(currentPlayer->getTeam() == 2){
					currentPlayer->spawnShip(&starsystems[i], uniqueColour[0],
						uniqueColour[1], uniqueColour[2],ship_models[1]);
				}
			}
			if (currentPlayer->getShipCount() != 0 &&
					currentPlayer->getStarsystemCount() != 0) {
				gameOver = false;
			}
			else
			{
				winningTeam = opposingPlayer->getTeam();
			}
		}

		cout << "turn ended" << endl;
		turnFinished = false;
		bool movesMade = false;
		while (!turnFinished) {
			if(!gameOver)
			{
				//computer players turn

				if(computerPlayer->getTeam() == currentPlayer->getTeam() && aiEnabled == 1)
				{
					if(!movesMade)
					{
						movesMade = true;
						computerPlayer->makeMoves();
					}
					if(movesMade && opposingPlayer->shipsDoneAnimating())
					{
						turnFinished = true;
						activeStarsystem = -1;
						resolveConflict(starsystems);
						currentPlayer->endTurn();

						if (currentPlayer->getTeam() == 1)
						{
							currentPlayer = &player2;
							opposingPlayer = &player1;
						}
						else
						{
							currentPlayer = &player1;
							opposingPlayer = &player2;
						}
					}
				}
				//human players turn
				else
				{
					SDL_Event event;

					//handle input from human player
					while (SDL_PollEvent(&event) ) {
						keys = SDL_GetKeyState(NULL);
						mod = SDL_GetModState();
						if (event.type == SDL_QUIT) {
							printf("quit");
							TTF_Quit();
							SDL_Quit();
							return 0;
						}

						if (event.type == SDL_KEYDOWN) {
							//turn finished
							if (event.key.keysym.sym == SDLK_SPACE && opposingPlayer->shipsDoneAnimating()) {
								turnFinished = true;
								activeStarsystem = -1;
								resolveConflict(starsystems);
								currentPlayer->endTurn();

								if (currentPlayer->getTeam() == 1)
								{
									currentPlayer = &player2;
									opposingPlayer = &player1;
								}
								else
								{
									currentPlayer = &player1;
									opposingPlayer = &player2;
								}
							}
						}
						if (event.type == SDL_MOUSEBUTTONDOWN) {
							cout << "mouse clicked" << endl;
							mouseClick(event, keys, mod, starsystems);
						}
						if (event.type == SDL_MOUSEBUTTONUP) {
							cout << "mouse released" << endl;
							mouseRelease(event, keys, mod);
						}
						//only call if mouse is held down and mouse is moved
						if (mouseHeldDown && event.type == SDL_MOUSEMOTION) {
							mouseDrag(event, keys, mod);
						}

					}

						if (keys[SDLK_a] && (mod != KMOD_LSHIFT)) {
							r.yaw(-0.5f);
							//				r.move(2);
						}

						if (keys[SDLK_d] && (mod != KMOD_LSHIFT)) {
							r.yaw(0.5f);
							cout << "d hed down" << endl;
						}
						if (keys[SDLK_LEFT] && (mod == KMOD_LSHIFT)) {

						}

						if (keys[SDLK_w] && (mod != KMOD_LSHIFT)) {
							r.zoom(0.5f);
						}
						if (keys[SDLK_s] && (mod != KMOD_LSHIFT)) {
							r.zoom(-0.5f);
						}
						if (keys[SDLK_UP] && (mod != KMOD_LSHIFT)) {
							r.pitch(-0.5f);
						}
						if (keys[SDLK_DOWN] && (mod != KMOD_LSHIFT)) {
							r.pitch(0.5f);
						}
						if (keys[SDLK_DOWN] && (mod == KMOD_LSHIFT)) {
							r.anglex -= 0.5f;
						}
						if (keys[SDLK_RIGHT] && (mod != KMOD_LSHIFT)) {
							r.roll(0.5f);
						}
						if (keys[SDLK_LEFT] && (mod != KMOD_LSHIFT)) {
							r.roll(-0.5f);
						}

						if (keys[SDLK_PAGEUP]&& (mod != KMOD_LSHIFT)) {
							r.zmove += 0.001;
						}
						if (keys[SDLK_PAGEUP] && (mod == KMOD_LSHIFT)) {
							r.anglez += 0.5f;
						}

						if (keys[SDLK_PAGEDOWN] && (mod != KMOD_LSHIFT)) {
							r.zmove -= 0.001;
						}
						if (keys[SDLK_PAGEDOWN] && (mod == KMOD_LSHIFT)) {
							r.anglez -= 0.5f;
						}

						if (keys[SDLK_ESCAPE]) {
							TTF_Quit();
							SDL_Quit();
							return 0;
						}
				}
			}
			else
			{
				SDL_Event event;

				//handle input from human player
				while (SDL_PollEvent(&event) )
				{
					keys = SDL_GetKeyState(NULL);
					mod = SDL_GetModState();
					if (event.type == SDL_QUIT) {
						printf("quit");
						TTF_Quit();
						SDL_Quit();
						return 0;
					}
				}
			}
			r.redisplay(drawContext,(worldRotation.GetMatrix()).coords,current_zoom);
			for (int i = 0; i < max_starsystems; i++) {
				if (activeStarsystem != i) {
					starsystems[i].drawStarsystem(false);
				}
			}
			if(activeStarsystem != -1)
			{
				starsystems[activeStarsystem].drawStarsystem(true);
			}
			player1.drawShips(false);
			player2.drawShips(false);
			currentPlayer->printHUDInfo(drawContext, font);

			if(gameOver)
			{
				std::ostringstream o;
				o << winningTeam;
				std::string info = "Team " + o.str() + " wins!" ;
				char *text = const_cast<char*>(info.c_str());
				HUD::renderText(drawContext, font, 255, 255, 255, text, (int)drawContext->w*0.44, (int)drawContext->h*0.2);
			}
			SDL_GL_SwapBuffers();
		}
	}
	TTF_Quit();
	SDL_Quit();
	return 0;
}
