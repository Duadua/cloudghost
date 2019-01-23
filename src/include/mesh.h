#pragma once

#include <QVector>

class RenderData;

class Mesh {
public:
	Mesh();
	~Mesh();

private:
	QVector<RenderData*> render_data;
	
};
