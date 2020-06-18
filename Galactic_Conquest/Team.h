#ifndef TEAM_H_
#define TEAM_H_
#include "Ship.h"
#include "Starsystem.h"
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

class Team
{	
public:

	Team(GLfloat r,GLfloat g, GLfloat b, int team, int ships_per_starsystem);
	Team();
	virtual ~Team();
	std::vector<Ship> ships;
	std::vector<int> selectedShips;
	void spawnShip(Starsystem*,int unique_r, int unique_g,
			int unique_b,Model *pModel);
	Starsystem* getSS();
	void initStarsystems();
	void selectShip(int r, int g, int b, bool selectingMultiple);
	void selectStarsystem(int i);
	void drawShips(bool selectionMode);
	void endTurn();
	int getTeam();
	void arriveAtSystem(Starsystem *s);
	void resolveConflict(Starsystem *s);
	void setOpposingPlayer(Team *opposingPlayer);
	void orderMoveShip(Starsystem *s);
	void moveShips();
	void createFleet();
	void disbandFleet();
	void orderMoveFleet(Starsystem *s);
	void moveFleets();
	bool shipsDoneAnimating();
	int getShipCount();
	int getStarsystemCount();
	int getActiveStarsystem();
	void printHUDInfo(SDL_Surface *drawContext,TTF_Font *font);
private:
	GLfloat r,g,b;
	int team;
	Team *opposingPlayer;
	int ship_count;
	int starsystem_count;
	int ships_per_starsystem;
	int turn;
	float x1,x2;
};


#endif /*TEAM_H_*/
