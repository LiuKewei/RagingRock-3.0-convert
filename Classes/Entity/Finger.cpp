#include "Finger.h"

Finger::~Finger()
{
	CC_SAFE_RELEASE_NULL(m_texture);

	delete[] m_vertices;
	delete[] m_coordinates;

	delete m_rowedPath;
	m_rowedPath = NULL;
}

bool Finger::init()
{
	return true;
}

std::queue<Point>* Finger::getRowedPath()
{
	return this->m_rowedPath;
}

Finger* Finger::createWithMaximumPoint(int limit)
{
	Finger* finger = new Finger;
	if (finger && finger->initWithMaximumPoint(limit)){
		finger->autorelease();
		return finger;
	}
	CC_SAFE_RELEASE_NULL(finger);
	return NULL;
}

bool Finger::initWithMaximumPoint(int limit)
{
	bool bRet = false;
	do
	{
		this->m_texture = NULL;

		this->setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE));

		this->m_pointLimit = limit;
		this->m_width = 5;

		m_vertices = new Point[2 * limit + 5];
		m_coordinates = new Point[2 * limit + 5];

		PointSet(m_coordinates + 0, 0.0f, 0.5f);

		m_rowedPath = new std::queue<Point>();

		bRet = true;
	} while (0);
	return bRet;
}

void Finger::drawFunc(const kmMat4 &transform, bool transformUpdated)
{
	if ((m_reset && m_rowedPath->size() > 0) || (this->getAutoDim() && m_willPop)){
		m_rowedPath->pop();
		if (m_rowedPath->size() < 3){
			while (!m_rowedPath->empty())
			{
				m_rowedPath->pop();
			}
		}
	}

	if (m_rowedPath->size() < 3)
		return;


	m_willPop = true;

	CC_NODE_DRAW_SETUP();

	GL::blendFunc(CC_BLEND_SRC, CC_BLEND_DST);

	if (this->getTexture() != NULL) {
		GL::bindTexture2D(this->getTexture()->getName());
	}
	else {
		GL::bindTexture2D(0);
	}
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(m_vertices[0]), m_vertices);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, sizeof(m_coordinates[0]), m_coordinates);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * m_rowedPath->size() - 2);
}

void Finger::draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated)
{
	m_customCommand.init(0);
	m_customCommand.func = CC_CALLBACK_0(Finger::drawFunc, this, transform, transformUpdated);
	renderer->addCommand(&m_customCommand);
}

