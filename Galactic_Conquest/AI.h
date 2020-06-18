#ifndef AI_H_
#define AI_H_

#include "Starsystem.h"
#include "Team.h"

class AI
{
public:
	AI();
	AI(Team *player, Starsystem *starsystems, Team *opposingPlayer, int max_starsystems);
	int getTeam();
	void makeMoves();
	virtual ~AI();
	struct Move 
	{
		int starSystemIndex;
		float distance;
		int enemyShips;
		int homeShips;
		int starSystemTeam;
	};
	Team *player;
	Team *opposingPlayer;
	Starsystem *starsystems;
	int max_starsystems;
};

#endif /*AI_H_*/
