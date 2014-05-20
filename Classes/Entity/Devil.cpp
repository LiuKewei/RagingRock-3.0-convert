#include "Devil.h"


Devil::Devil()
	: m_currentMaxIndex(0)
	, m_devilPosCnt(0)
{
	m_devilPosVec = new std::vector<Point>();
}

Devil::~Devil()
{
	CC_SAFE_DELETE(m_devilPosVec);
}

bool Devil::init()
{
	return true;
}

std::vector<Point>* Devil::getDevilPosVec()
{
	return this->m_devilPosVec;
}

void Devil::setDevilMaxIndexInCurrent(unsigned int index)
{
	this->m_currentMaxIndex = index;
}

unsigned int Devil::getDevilMaxIndexInCurrent()
{
	return this->m_currentMaxIndex;
}

void Devil::setDevilPosCnt(unsigned int devilPosCnt)
{
	this->m_devilPosCnt = devilPosCnt;
}
unsigned int Devil::getDevilPosCnt()
{
	return this->m_devilPosCnt;
}