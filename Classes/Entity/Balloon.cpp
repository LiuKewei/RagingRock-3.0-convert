#include "Balloon.h"


Balloon::Balloon()
	: m_maxCnt(5)
	, m_counter(0)
	, m_isbombed(false)
	, m_successCnt(0)
{
}

Balloon::~Balloon()
{
}

bool Balloon::init()
{
	return true;
}


void Balloon::setMaxCnt(unsigned int maxCnt)
{
	this->m_maxCnt = maxCnt;
}
unsigned int Balloon::getMaxCnt()
{
	return this->m_maxCnt;
}

void Balloon::setCounter(unsigned int counter)
{
	this->m_counter = counter;
}
unsigned int Balloon::getCounter()
{
	return this->m_counter;
}

void Balloon::setSuccessCnt(unsigned int successCnt)
{
	this->m_successCnt = successCnt;
}
unsigned int Balloon::getSuccessCnt()
{
	return this->m_successCnt;
}

bool Balloon::isbombed()
{
	return this->m_isbombed;
}
void Balloon::setbombed(bool isbombed)
{
	this->m_isbombed = isbombed;
}