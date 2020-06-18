#ifndef GAMEVARIABLES_H_
#define GAMEVARIABLES_H_
#define PI 3.14159

static int windowWidth = 800;
static int windowHeight = 600;
static int colourDepth;
const int average_distance = 10;
const float orbitalDistance = 1.5;
//lower this number is the more turns it takes to get to destination
const float turnFactor = 20;
const int textureCount = 7;
const int starsystemModelCount = 9;
const int minimumNeighbourDistance = 1500;
static int maximumOriginDistance;
const int starsystemRadius = 8;
const float starting_position[3] = {0.0f,0.0f,-50.0f};

#endif /*GAMEVARIABLES_H_*/
