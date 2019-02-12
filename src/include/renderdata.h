#pragma once

#include "reflex.h"
#include "tools/meshloader.h"
#include <QOpenGLFunctions_3_3_Core>

class RenderData : public CObject{
	DECLARE_CLASS(RenderData)
public:
	RenderData();
	virtual ~RenderData();

	void init(const std::vector<MVertex>& v, const std::vector<uint>& i);
	void draw();

private:
	uint vao;
	uint vbo;
	uint ebo;
	
	std::vector<MVertex> vertices;
	std::vector<uint> indices;		// Ë÷Òý

	QOpenGLFunctions_3_3_Core * core;
};
DECLARE_AUTO_PTR(RenderData)

 
