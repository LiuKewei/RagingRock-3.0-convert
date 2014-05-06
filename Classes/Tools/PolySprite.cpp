#include "PolySprite.h"

#include "cocos2d.h"

using namespace cocos2d;

PolySprite* PolySprite::create(const char *pFile,
                                 const Point *uvs,
                                 int verCnt,
                                 const int *indices)
{
    PolySprite *pobSprite = new PolySprite();
    //创建精灵
    if (pobSprite &&
        pobSprite->initWithFile(pFile) &&
        pobSprite->initWithUV(uvs, indices, verCnt)) {
        
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return false;
}

PolySprite::~PolySprite()
{
    releasePoly();
}

//初始化顶点信息
bool PolySprite::initWithUV(const Point *uvs,
                             const int *indices,
                             int verCnt)
{
    //内存分配
    vertexs_ = new ccVertex2F[verCnt];
    uvs_     = new ccVertex2F[verCnt];
    indices_ = new unsigned short[(verCnt-2)*3];
    colors_  = new unsigned char[verCnt*4];
    
    //失败处理
    if(!vertexs_ || !uvs_ || !indices_ || !colors_) {
        releasePoly();
        return false;
    }
    
    //贴图大小
	Size rc = this->getTexture()->getContentSize();
    
    for(int i = 0; i < verCnt; ++i) {
        //根据纹理坐标以及纹理大小计算顶点坐标
        vertexs_[i].x = uvs[i].x*rc.width;
        //cocos2dx纹理坐标以左上角为原点
        vertexs_[i].y = (1.0-uvs[i].y)*rc.height;
        
        uvs_[i].x = uvs[i].x;
        uvs_[i].y = uvs[i].y;
    }
    
    for(int i = 0; i < (verCnt-2)*3; ++i)
        indices_[i] = indices[i];
    
    memset(colors_, 255, sizeof(unsigned char)*verCnt*4);
    
    verCnt_ = verCnt;
    
    translate(getCenter());
    
    return true;
}

//计算中点
Point PolySprite::getCenter()
{
    if(!vertexs_) return Point(0,0);
    
    float minx = vertexs_[0].x,
          maxx = vertexs_[0].x,
          miny = vertexs_[0].y,
          maxy = vertexs_[0].y;
    
    //计算所有顶点坐标的中心点坐标
    for(int i = 0; i < verCnt_; ++i) {
        minx = minx>vertexs_[i].x?vertexs_[i].x:minx;
        maxx = maxx;
        
        miny = miny>vertexs_[i].y?vertexs_[i].y:miny;
        maxy = maxy;
    }
    
    return Point((minx+maxx)*0.5, (miny+maxy)*0.5);
}

void PolySprite::translate(const cocos2d::Point& pos)
{
    //设置锚点
    //Size rc = m_pobTexture->getContentSize();
	Size rc = this->getTexture()->getContentSize();
    setAnchorPoint(Point(pos.x/rc.width, pos.y/rc.height));
}

void PolySprite::drawPoly()
{
    CC_NODE_DRAW_SETUP();
    
	ccGLBlendFunc(this->getBlendFunc().src, this->getBlendFunc().dst);
    
	if (this->getTexture() != NULL) {
		ccGLBindTexture2D(this->getTexture()->getName());
    }
    else {
        ccGLBindTexture2D(0);
    }
    
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex);
    
    //顶点，纹理，颜色
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertexs_);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, uvs_);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, colors_);
    //根据索引draw三角形
    glDrawElements(GL_TRIANGLES, (verCnt_-2)*3, GL_UNSIGNED_SHORT, indices_);
    
    CC_INCREMENT_GL_DRAWS(1);
}

void PolySprite::releasePoly()
{
    CC_SAFE_DELETE(vertexs_);
    CC_SAFE_DELETE(uvs_);
    CC_SAFE_DELETE(indices_);
    CC_SAFE_DELETE(colors_);
}

void PolySprite::draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated)
{
    drawPoly();
}