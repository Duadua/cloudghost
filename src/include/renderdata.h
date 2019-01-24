#pragma once

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLFunctions_3_3_Core>

struct Vertex {
	QVector3D position;
	QVector3D normal;
	QVector2D tex_coord;
};

class RenderData {
public:
	RenderData();
	~RenderData();

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

 
