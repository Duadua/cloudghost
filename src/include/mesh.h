#pragma once

#include "reflex.h"
#include "renderdata.h"
#include <QVector>

class Mesh : public CObject{
	DECLARE_CLASS(Mesh)
public:
	Mesh();
	~Mesh();

	void load(const QString& path);
	void draw();

private:
	void load_mesh_with_txt(const QString& path);
	void load_mesh_with_obj(const QString& path);

private:

	QVector<SPTR_RenderData> render_datas;

};
DECLARE_AUTO_PTR(Mesh)
