#ifndef __POLY_SPRITE_H__
#define __POLY_SPRITE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

class PolySprite: public cocos2d::Sprite
{
public:
	PolySprite() : vertexs_(NULL), uvs_(NULL), indices_(NULL), verCnt_(0) {}
	virtual ~PolySprite();

	static PolySprite* create(const char *pFile,
		const cocos2d::Point *uvs,
		int verCnt,
		const int *indices);

	//���ظ���draw
	void draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated);

private:
	//��ʼ��������Ϣ
	bool initWithUV(const cocos2d::Point *uvs,
		const int *indices,
		int verCnt);

	//�����е�
	cocos2d::Point getCenter();
	void translate(const cocos2d::Point&);
	void drawPoly();
	void releasePoly();

private:
	//����ζ���
	cocos2d::ccVertex2F *vertexs_;
	//������������
	cocos2d::ccVertex2F *uvs_;
	//����������
	unsigned short *indices_;
	//������ɫ
	unsigned char *colors_;
	//������Ŀ
	int verCnt_;
};

#endif // __POLY_SPRITE_H__