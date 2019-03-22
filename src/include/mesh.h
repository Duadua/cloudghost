#pragma once

#include "reflex.h"
#include "renderdata.h"
#include <string>

struct RenderDataInstance {
	SPTR_RenderData rd;
	std::string material;
	RenderDataInstance() : material("") {}
	RenderDataInstance(SPTR_RenderData t_rd, const std::string& t_mt = "") : rd(t_rd), material(t_mt) {}
};

DECLARE_AUTO_PTR(Mesh)
class Mesh : public CObject{
	DECLARE_CLASS(Mesh)
public:
	Mesh();
	Mesh(const Mesh& b);
	void copy_from(const SPTR_Mesh b);
	virtual ~Mesh();

	virtual void draw(const std::string& shader);

	void add_render_data(SPTR_RenderData rd);

	GET_SET(bool, use_default_mt)

	RenderDataInstance& render_data(uint id);

protected:

	bool use_default_mt;

	std::vector<RenderDataInstance> render_datas;

};
