#ifndef __MSG_TYPE_FOR_OBSERVER_H__
#define __MSG_TYPE_FOR_OBSERVER_H__


enum
{
	Z_ORDER_ZERO,
	Z_ORDER_ONE,
	Z_ORDER_TWO,
	Z_ORDER_THREE,
	Z_ORDER_FOUR,
	Z_ORDER_FIVE,
	Z_ORDER_SIX,
	Z_ORDER_SEVEN,
	Z_ORDER_EIGHT,
	Z_ORDER_NINE,
	Z_ORDER_TEN,

	Z_ORDER_MAX,
};

class MsgTypeForObserver
{
public:
	//
	const static char* c_DevilPosUpdate;
	const static char* c_DevilFightingStart;
	const static char* c_DevilFightingStop;
	const static char* c_DevilPosPush;

	//
	const static char* c_BalloonStart;
	const static char* c_BalloonStop;

	//
	const static char* c_BattleStart;
	const static char* c_BattleStop;

	//
	const static char* c_BrickStart;
	const static char* c_BrickStop;

	static int getRand(int start,int end);
};

inline int MsgTypeForObserver::getRand(int start,int end)
{  
	float i = CCRANDOM_0_1()*(end-start+1)+start;  //产生一个从start到end间的随机数  
	return (int)i;  
}

#endif // __MSG_TYPE_FOR_OBSERVER_H__