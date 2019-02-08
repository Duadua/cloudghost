#pragma once

#include "reflex.h"
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLFunctions_3_3_Core>

struct Vertex {
	QVector3D position;
	QVector3D normal;
	QVector2D tex_coord;
};

class RenderData : public CObject{
	DECLARE_CLASS(RenderData)
public:
	RenderData();
	virtual ~RenderData();

	void init(const QVector<Vertex>& v, const QVector<unsigned int>& i);
	void draw();

private:
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	
	QVector<Vertex> vertices;
	QVector<GLuint> indices;		// Ë÷Òý

	QOpenGLFunctions_3_3_Core * core;
};
DECLARE_AUTO_PTR(RenderData)

 
