#include "Team.h"
#include "GameVariables.h"
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include "HUD.h"

using namespace std;

Team::Team(GLfloat r, GLfloat g, GLfloat b, int team, int ships_per_starsystem) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->team = team;
	ship_count = 0;
	starsystem_count = 1;
	turn = 0;
	this->ships_per_starsystem = ships_per_starsystem;
}

//default contructor
Team::Team() {

}

Team::~Team() {
}

void Team::spawnShip(Starsystem *s, int unique_r, int unique_g,
		int unique_b,Model *pModel) {
		
	float nx,nz;
		bool noCollision;
			do {
				x2 = ((rand()%200)-100);
				x1 = ((rand()%200)-100);
				x2 = x2/100;
				x1 = x1/100;
				cout << "x2 " << x2 << endl;
				cout << "x1 " << x1 << endl;
				nx = ((x1 * x1) - (x2 * x2)) / ((x1 * x1) + (x2 * x2));
				nz = (2 * x1 * x2) / ((x1 * x1) + (x2 * x2));
				nx = (nx * s->radius*orbitalDistance) + s->x;
				nz = (nz * s->radius*orbitalDistance) + s->z;
				
				Point otherCoordinates(nx, s->y, nz, 1.0f);
				
				noCollision = true;
				for(int i = 0; i < ships.size(); i++){
					if(ships[i].checkForCollision(otherCoordinates, 
							ships[0].boundingSpheresRadii, ships[0].boundingSpheresCentres))
					{
						cout << "Collision detected" << endl;
						noCollision = false;
					}
					else {
						cout << "No collission " << endl;
					}
					
				}
			} while (!noCollision);

		std::cout << " ship created for team" << std::endl;
		ships.push_back(Ship(r, g, b, 0.0f, nx, s->y, nz, unique_r, unique_g,
				unique_b,s,pModel));
		s->homeShips++;
		ship_count++;
		std::cout << "UID " << unique_r << " " << unique_g << " " << unique_b << std::endl;
}

void Team::selectShip(int r, int g, int b,  bool selectingMultiple) {
	for (int i = 0; i < ships.size(); i++) {
		//if ship was clicked
		if (ships[i].uniqueColour[0] == r && ships[i].uniqueColour[1] == g
				&& ships[i].uniqueColour[2] == b) {
//			cout << "Ship clicked" << endl;
			//if lctr is held down
			if(selectingMultiple)
			{
//				cout << "lctrl is held down" << endl;
				//if other ships have been selected yet
				if(selectedShips.size() > 0)
				{
//					cout << "other ships have been selected" << endl;
					bool alreadySelected = false;
					//double check that ship clicked hasnt already been selected
					for(int j = 0; j < selectedShips.size(); j++)
					{
						//if it hasnt then add ship to selection stack
						cout << "UID " << ships[i].uniqueColour[0] << " " << ships[i].uniqueColour[1]
						<< " " << ships[i].uniqueColour[2] << endl;
						if(ships[selectedShips[j]].uniqueColour[0] == ships[i].uniqueColour[0] 
								&& ships[selectedShips[j]].uniqueColour[1] == ships[i].uniqueColour[1]
								&& ships[selectedShips[j]].uniqueColour[2] == ships[i].uniqueColour[2])
						{
//							cout << "ship already selected" << endl;
							alreadySelected = true;
						}
					}
					if(alreadySelected == false)
					{
//						cout << "adding ship to stack" << endl;
						selectedShips.push_back(i);
					}
				}
				//if no other ships have been selected
				else 
				{
//					cout << "adding first ship to stack" << endl;
					selectedShips.push_back(i);
				}
			}
			//lctrl is not held down but ship has been selected
			else 
			{
//				cout << "lctrl is not held down but ship has been selected" << endl;
				selectedShips.clear();
				selectedShips.push_back(i);
				break;
			}
			break;
		}
		//no ship was clicked
		else
		{
			//basically if lctrl isnt held down
			if(!selectingMultiple)
			{
				selectedShips.clear();
			}
		}
	}
//	cout << "Printing stack info" << endl;
	for(int i = 0; i < selectedShips.size(); i++)
	{
//		cout << "Ship INdex " << selectedShips[i] << endl;
//		cout << "UID " << ships[selectedShips[i]].uniqueColour[0] << endl;
//		cout << "UID " << ships[selectedShips[i]].uniqueColour[1] << endl;
//		cout << "UID " << ships[selectedShips[i]].uniqueColour[2] << endl;
	} 
}

void Team::setOpposingPlayer(Team *opposingPlayer)
{
	this->opposingPlayer = opposingPlayer;
}

void Team::endTurn() {
	moveShips();
	selectedShips.clear();
	//	activeStarsystem = -1;
	turn++;
}

void Team::arriveAtSystem(Starsystem *s) {
//		cout << " dx2 " << ships[i].getCoordinates().distancex2(s->coordinates) << endl;
//		cout << " orbital distance " << (s->radius*s->radius*orbitalDistance*orbitalDistance) << endl;
			
			if(s->team == opposingPlayer->team && s->homeShips == 0){
				s->inConflict = false;
				s->homeShips = 1;
				this->starsystem_count++;
				opposingPlayer->starsystem_count--;
				s->team = team;
			}
			else if(s->team != team && s->homeShips == 0)
			{
				s->inConflict = false;
				s->homeShips = 1;
				this->starsystem_count++;
				s->team = team;
			}
			else if(s->team == team){
				s->homeShips++;
			}
			else if(s->team != team && s->homeShips > 0){
				cout << "system now in conflict" << endl;
				s->inConflict = true;
				s->enemyShips++;
			}
}

bool Team::shipsDoneAnimating(){
	for(int i = 0;  i < ships.size(); i++)
	{
		if(!ships[i].doneMoving)
		{
			return false;
		}
	}
	return true;
}

void Team::resolveConflict(Starsystem *s){
	cout << "Info about ss " << endl;
	cout << "Enemy ships " << s->enemyShips << endl;
	cout << "Homeships " << s->homeShips << endl;
	//scan through other players ships and find one that is at the starsystem in conflict
	for (int i = 0; i < this->ships.size(); i++) {
		//when found erase it and update all info about the team and starsystem
		if(this->ships[i].systemDestination == s && this->ships[i].hasArrived)
		{
			cout << "Ship found at ss in conflict" << endl;
			cout << "Player " << this->team << " lost a ship" << endl;
			this->ships.erase(this->ships.begin()+i,this->ships.begin()+i+1);
			this->ship_count--;
			
			//if starsystem didnt belong to ship being destroyed its an enemy so decrement its
			//enemy ships
			int starSystemsTeam = s->team;
			if(starSystemsTeam != this->team){
				cout << "SS belonged to player " << this->team << endl;
				s->enemyShips--;
			}
			//if starsystem did belong to ship being destroyed then its a friendly ship so 
			//decrement homeships
			else {
				cout << "SS belonged to player " << this->team << endl;
				s->homeShips--;
			}
			//now check if there are any ships left
			//if homeships is 0 then its undefended and is captured by opposing player
			if(s->homeShips == 0){
				cout << "SS captured " << endl;
				//update starsystem info and teams info
				//enemyships become home ships
				s->team = opposingPlayer->team;
				this->starsystem_count--;
				opposingPlayer->starsystem_count++;
				s->homeShips = s->enemyShips;
				s->enemyShips = 0;
				cout << "Conflict ended" << endl;
				s->inConflict = false;
			}
			else if(s->enemyShips == 0){
				cout << "SS successfully defended " << endl;
				cout << "Conflict ended" << endl;
				s->inConflict = false;
			}
		}
	}
}

void Team::drawShips(bool selectionMode) {
	for (int i = 0; i < ships.size(); i++) 
	{
		if (selectionMode) 
		{
			ships[i].drawUniqueShip();
		} 
		else 
		{
			//if ship index matches one in the selectedships stack then highlight
			if(selectedShips.size() > 0)
			{
				for(int j = 0; j < selectedShips.size(); j++)
								{
//					cout << "Ships in stack " << selectedShips[j] << endl;
								}
				bool inSelectedShips = false;
				for(int j = 0; j < selectedShips.size(); j++)
				{
//					cout << "Current ship being drawn " << i << endl;
					if (selectedShips[j] == i) {
//						cout << "Drawing " << i << endl;
						inSelectedShips = true;
						
						break;
					}
				}
				ships[i].drawShip(inSelectedShips);
			}
			else
			{
				ships[i].drawShip(false);
			}
		}
	}
}



int Team::getTeam() {
	return team;
}

int Team::getShipCount() {
	return ship_count;
}

int Team::getStarsystemCount(){
	return starsystem_count;
}

void Team::orderMoveShip(Starsystem *s) {
	if (selectedShips.size() > 0)
	{
		if(selectedShips.size() == 1)
		{

			cout << "Moving ship to ss with es " << s->enemyShips << " and hs " << s->homeShips << endl;
			int i = selectedShips[0];
			if(!ships[i].currentSystem->inConflict)
			{
				ships[i].moveVector = (s->coordinates
						- ships[i].getCoordinates());
				
				ships[i].updateSystemDestination(s);
				//distance between ship centre and starsystem
				GLfloat vectorDistance  = ships[i].moveVector.Length();
				//finding distance from ship centre to starsystems orbit
				GLfloat orbitDistance = vectorDistance - s->radius*orbitalDistance;
				//ratio of the 2 distances used to scale the original vector
				GLfloat vectorRatio = orbitDistance/vectorDistance;
				//finding out how many turns it is away
				ships[i].arrivalTurns = (int)vectorDistance/turnFactor;
				//decreasing magnitude of vector depending on how many turns distance is and
				//scaling by the ratio so ship orbits starsystem
				ships[i].moveVector = (ships[i].moveVector/ships[i].arrivalTurns)*vectorRatio;
				Point c = (ships[i].moveVector)*ships[selectedShips[0]].arrivalTurns
						+ ships[i].getCoordinates();
				
				ships[i].destinationCoords.coords[0] = c.coords[0];
				ships[i].destinationCoords.coords[1] = c.coords[1];
				ships[i].destinationCoords.coords[2] = c.coords[2];
			}
			else
			{
				cout << "Cant move ship because its in conflict" << endl;
			}
		}
		else
		{
			int mostTurns = 0;
			//set up all ships trajectory and take note of ship with most turns away
			for(int i = 0; i < selectedShips.size(); i++)
			{
				if(!ships[i].currentSystem->inConflict)
				{
					int j = selectedShips[i];
					ships[j].moveVector = (s->coordinates
							- ships[j].getCoordinates());
								
					ships[j].updateSystemDestination(s);
					//distance between ship centre and starsystem
					GLfloat vectorDistance  = ships[j].moveVector.Length();
					//finding distance from ship centre to starsystems orbit
					GLfloat orbitDistance = vectorDistance - s->radius*orbitalDistance;
					//ratio of the 2 distances used to scale the original vector
					GLfloat vectorRatio = orbitDistance/vectorDistance;
					//finding out how many turns it is away
					ships[j].arrivalTurns = (int)vectorDistance/turnFactor;
					if(ships[j].arrivalTurns > mostTurns)
					{
						mostTurns = ships[j].arrivalTurns;
					}
					//decreasing magnitude of vector depending on how many turns distance is and
					//scaling by the ratio so ship orbits starsystem
					ships[j].moveVector = (ships[j].moveVector/ships[j].arrivalTurns)*vectorRatio;
					Point c = (ships[j].moveVector)*ships[selectedShips[0]].arrivalTurns
							+ ships[j].getCoordinates();
								
					ships[j].destinationCoords.coords[0] = c.coords[0];
					ships[j].destinationCoords.coords[1] = c.coords[1];
					ships[j].destinationCoords.coords[2] = c.coords[2];
				}
				else
				{
					cout << "Cant move ship because its in conflict" << endl;
				}
			}
			cout << "most turns is " << mostTurns << endl;
			//now go through all ships and scale their trajectory so they move with furthest ship
			for(int i = 0; i < selectedShips.size(); i++)
			{
				if(!ships[i].currentSystem->inConflict)
				{
					GLfloat arrivalTurns = (GLfloat)ships[i].arrivalTurns;
					ships[i].moveVector = ships[i].moveVector*(arrivalTurns/(GLfloat)mostTurns);
					ships[i].arrivalTurns = mostTurns;
					cout << "ship " << i << " turns away is" << ships[i].arrivalTurns << endl;
				}
			}
		}
	}
	else 
	{
		std::cout << "Select ship first" << std::endl;
	}
}

//moveships called at the start of every turn for every ship
//movement started so is moving, point updated, arrival turns decremented
//if no more turns then ship has arrived
void Team::moveShips() {
	for(int i = 0; i < ships.size(); i++) {
		if(ships[i].arrivalTurns > 0){
			//this will only happen once since it can only leave when its not moving
			//at which point it will start moving
			ships[i].setCoordinates(ships[i].moveVector
					+ ships[i].getCoordinates());
			ships[i].leaveCurrentSystem();
			ships[i].startMovementAnimation();
			ships[i].arrivalTurns--;
			cout << "Arrival turns " << ships[i].arrivalTurns << endl;
		}
		//arriving at system so not moving and has arrived
		if(!ships[i].hasArrived && ships[i].arrivalTurns == 0){
			arriveAtSystem(ships[i].systemDestination);
			ships[i].updateCurrentSystem(ships[i].systemDestination);
			ships[i].hasArrived = true;
			ships[i].moving = false;
			cout << "Team " << team << " ships arrived " << endl;
		}
	}
}

void Team::printHUDInfo(SDL_Surface *drawContext, TTF_Font *font) {

	std::ostringstream o;
	o << team;
	std::string info = "Player " + o.str() + " turn";
	char *text = const_cast<char*>(info.c_str());
	HUD::renderText(drawContext, font, 255, 255, 255, text, drawContext->w/1.2, drawContext->h/1.1);
	o.str("");

	o << turn;
	info = "Turn: " + o.str();
	text = const_cast<char*>(info.c_str());
	HUD::renderText(drawContext, font, 255, 255, 255, text, drawContext->w/1.2, drawContext->h/1.15);
	o.str("");

	o << ship_count << "/" << starsystem_count*ships_per_starsystem;
	info = "POP: " + o.str();
	text = const_cast<char*>(info.c_str());
	HUD::renderText(drawContext, font, 255, 255, 255, text, drawContext->w/1.2, drawContext->h/1.2);
	o.str("");

	delete text;
}
