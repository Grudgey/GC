#include "AI.h"
#include <iostream>

AI::AI()
{
	
}

AI::AI(Team *player, Starsystem *starsystems, Team *opposingPlayer, int max_starsystems)
{
	this->player = player;
	this->opposingPlayer = opposingPlayer;
	this->starsystems = starsystems;
	this->max_starsystems = max_starsystems;
}

AI::~AI()
{
}

void AI::makeMoves()
{
	for(int i = 0; i < player->ships.size(); i++)
	{
		std::vector<Move> moves;
		
		for(int j = 0; j < max_starsystems; j++)
		{
			if(&starsystems[j] != player->ships[i].currentSystem
					&& starsystems[j].team != player->getTeam())
			{
				Move move;
				move.distance = starsystems[j].coordinates.distancex2(player->ships[i].getCoordinates());
				move.starSystemIndex = j;
				move.starSystemTeam = starsystems[j].team;
				moves.push_back(move);
			}
		}
		if(moves.size() > 0)
		{
			float leastDistance;
			leastDistance = moves[0].distance;
			int moveIndex = 0;
			std::cout << "Distance " << moves[0].distance << std::endl;
			for(int j = 1; j < moves.size(); j++)
			{
				std::cout << "Distance " << moves[j].distance << std::endl;
				if(moves[j].distance < leastDistance)
				{
					leastDistance = moves[j].distance;
					moveIndex = j;
				}
			}
			std::cout << "Least distance " << leastDistance << std::endl;
			std::cout << "Ordering ship " << std::endl;
			player->selectedShips.clear();
			player->selectedShips.push_back(i);
			player->orderMoveShip(&starsystems[moves[moveIndex].starSystemIndex]);
		}
	}
}

int AI::getTeam()
{
	return player->getTeam();
}
