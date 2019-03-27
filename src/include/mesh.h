#pragma once

#include "reflex.h"
#include "typehelper.h"
#include <string>

PRE_DECLARE_CLASS(Material)
USING_SPTR(Material)
PRE_DECLARE_CLASS(RenderData)
USING_SPTR(RenderData)

struct RenderDataInstance {
	SPTR_RenderData rd;
	SPTR_Material material;
	RenderDataInstance() : material(nullptr) {}
	RenderDataInstance(SPTR_RenderData t_rd, SPTR_Material t_mt = nullptr) : rd(t_rd), material(t_mt) {}
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
