#pragma once

#include <QVector>

class RenderData;

class Mesh {
public:
	Mesh();
	~Mesh();

	void load(const QString& path);
	void draw();

private:
	void load_mesh_with_txt(const QString& path);
	void load_mesh_with_obj(const QString& path);

private:

	QVector<RenderData*> render_datas;

};
