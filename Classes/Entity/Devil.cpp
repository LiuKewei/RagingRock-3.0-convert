#include "Devil.h"


Devil::Devil()
: m_tmpPos(Point(0,0))
{
}

Devil::~Devil()
{
}

bool Devil::init()
{
	return true;
}


Point Devil::getDevilTmpPos()
{
	return this->m_tmpPos;
}
void Devil::setDevilTmpPos(Point p)
{
	this->m_tmpPos = p;
}

std::vector<Point> Devil::getDevilPosVec()
{
	return this->m_devilPosVec;
}