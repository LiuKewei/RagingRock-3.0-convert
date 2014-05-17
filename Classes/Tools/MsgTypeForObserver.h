#ifndef __MSG_TYPE_FOR_OBSERVER_H__
#define __MSG_TYPE_FOR_OBSERVER_H__


class MsgTypeForObserver
{
public:
	const static char* c_DevilPosUpdate;
	const static char* c_DevilFightingStart;
	const static char* c_DevilFightingStop;
	const static char* c_DevilPosPush;



	const static char* c_BalloonStart;
	const static char* c_BalloonStop;

	static int getRand(int start,int end);
};

#endif // __MSG_TYPE_FOR_OBSERVER_H__