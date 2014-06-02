#ifndef __CONST_H__
#define __CONST_H__

enum
{
	Z_ORDER_ZERO,
	Z_ORDER_ONE,
	Z_ORDER_TWO,
	Z_ORDER_THREE,
	Z_ORDER_FOUR,

	Z_ORDER_MAX,
};


//AppDelegate.cpp
const unsigned int c_designSize[2] = {640, 960};
const unsigned int c_resourceSize[2] = {640, 960};
const unsigned int c_gravity[2] = {0.0f, -1000.0f};
const std::string c_resSearchPaths[] = {"./fonts","./Balloon"};

//SnagForestLayer.cpp
const float c_ballradius = 10.0f;
const float c_ballDensity = 0.0f;
const float c_ballFriction = 2.0f;
const float c_ballRestitution = 0.9f;

const float c_snagDensity = 1.0f;
const float c_snagFriction = 0.1f;
const float c_snagRestitution = 0.5f;

const unsigned int c_snagHeightStart = 840;
const unsigned int c_ballHeightBegin = 190;
const unsigned int c_ballHeightStart = 64;
const unsigned int c_ballHeightStop = 2;

//Finger.cpp
const unsigned int c_fingerWidth = 5;
const unsigned int c_verticesFaultToleranceAbility = 15;

//DevilLayer.cpp
const unsigned int c_fingersCapability = 3;
const unsigned int c_fingerPointsLimit = 30;

#endif // __CONST_H__