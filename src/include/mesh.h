#pragma once

#include "reflex.h"
#include "renderdata.h"
#include <string>

class Mesh : public CObject{
	DECLARE_CLASS(Mesh)
public:
	Mesh();
	~Mesh();

	void draw(const std::string& shader);

	void add_render_data(SPTR_RenderData rd);

	GET_SET(bool, use_default_mt);

	GET_SET(std::vector<SPTR_RenderData>, render_datas);

private:

	bool use_default_mt;

	std::vector<SPTR_RenderData> render_datas;

};
DECLARE_AUTO_PTR(Mesh)
