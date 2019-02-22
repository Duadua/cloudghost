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

	GET_SET(std::string, material_name);

private:
	uint vao;
	uint vbo;
	uint ebo;
	
	std::vector<MVertex> vertices;
	std::vector<uint> indices;		// 索引

	std::string material_name;

	QOpenGLFunctions_3_3_Core * core;
};
DECLARE_AUTO_PTR(RenderData)

 
