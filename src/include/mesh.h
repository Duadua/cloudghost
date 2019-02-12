#pragma once

#include "reflex.h"
#include "renderdata.h"
#include "tools/meshloader.h"
#include <string>

class Mesh : public CObject{
	DECLARE_CLASS(Mesh)
public:
	Mesh();
	~Mesh();

	bool load(const std::string& src, SourceType source_type = SourceType::BY_FILE);
	void draw();

private:

	std::vector<SPTR_RenderData> render_datas;

};
DECLARE_AUTO_PTR(Mesh)
