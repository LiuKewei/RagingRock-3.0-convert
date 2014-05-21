#include "Finger.h"

Finger::~Finger()
{
	CC_SAFE_RELEASE_NULL(m_texture);

	delete[] m_vertices;
	delete[] m_coordinates;

	m_rowedPath->clear();
	delete m_rowedPath;
	m_rowedPath = NULL;
}

bool Finger::init()
{
	return true;
}

std::list<Point>* Finger::getRowedPath()
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

		m_reset = false;

		m_willPop = true;


		m_rowedPath = new std::list<Point>();

		bRet = true;
	} while (0);
	return bRet;
}

void Finger::drawFunc(const kmMat4 &transform, bool transformUpdated)
{
	if ((m_reset && m_rowedPath->size() > 0) || (this->getAutoDim() && m_willPop)){
		m_rowedPath->pop_front();
		if (m_rowedPath->size() < 3){
			m_rowedPath->clear();
			m_reset = false;
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

void Finger::push(Point v)
{
	this->m_willPop = false;
	if (this->m_reset)
	{
		return;
	}
	this->m_rowedPath->push_back(v);
	if (this->m_rowedPath->size() > m_pointLimit)
	{
		this->m_rowedPath->pop_front();
	}
	this->populateVertices();
}

void Finger::pop(int n)
{
	while (this->m_rowedPath->size() > 0 && n > 0)
	{
		this->m_rowedPath->pop_front();
		--n;
	}

	if (this->m_rowedPath->size() > 2)
	{
		this->populateVertices();
	}
}

void Finger::populateVertices()
{
	std::list<Point>::iterator iter = m_rowedPath->begin();
	unsigned int i = 0;
	float dd = m_width / m_rowedPath->size();

	m_vertices[0] = *iter;
	Point pre = *iter;
	Point it;

	++iter;
	while ((iter != m_rowedPath->end()) && (i < m_rowedPath->size() - 2))
	{
		it = *iter;
		f1(pre, it, m_width - i * dd, m_vertices + 2 * i + 1, m_vertices + 2 * i + 2);
		PointSet(m_coordinates + 2 * i + 1, 0.5f, 1.0f);
		PointSet(m_coordinates + 2 * i + 2, 0.5f, 0.0f);
		++i;
		pre = it;
		++iter;
	}

	PointSet(m_coordinates + 1, 0.25f, 1.0f);
	PointSet(m_coordinates + 2, 0.25f, 0.0f);

	if (m_rowedPath->size() > 1)
	{
		m_vertices[2 * m_rowedPath->size() - 3] = it;
		PointSet(m_coordinates + 2 * m_rowedPath->size() - 3, 0.75f, 0.5f);
	}

}