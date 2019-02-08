#include "mesh.h"
#include <QFile>
#include <QDebug>

IMPLEMENT_CLASS(Mesh)

Mesh::Mesh() {}
Mesh::~Mesh() {}

void Mesh::load(const QString& path) {
	// 获得文件路径后缀
	QString suf = path.right(path.length() - path.lastIndexOf('.') - 1);
	// 通过后缀调用函数
	if (suf.compare("txt") == 0) { load_mesh_with_txt(path); }
	else if (suf.compare("obj") == 0) { load_mesh_with_obj(path); }
	else { load_mesh_with_txt(path); }
}

void Mesh::load_mesh_with_txt(const QString& path) {
	QVector<Vertex> t_v;
	QVector<GLuint> t_i;

	// 打开文件
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "【error】【mesh】cannot open mesh text" << endl;
		file.close();
		return;
	}
	QTextStream in(&file);

	// 按行读取 -- 获得 vertices 和 indices
	while (!in.atEnd()) {
		QString line = in.readLine();
		line.replace(",", " ");
		QStringList list = line.split(" ", QString::SkipEmptyParts);
		if (list.empty()) continue;

		if (list[0].compare("v") == 0) {
			Vertex v;
			if(list.length() > 3) v.position = QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat());
			if(list.length() > 6) v.normal = QVector3D(list[4].toFloat(), list[5].toFloat(), list[6].toFloat());
			if (list.length() > 8) v.tex_coord = QVector2D(list[7].toFloat(), list[8].toFloat());
			t_v.append(v);
		}
		else if (list[0].compare("i") == 0) {
			for (int i = 1; i < list.length(); ++i) t_i.append(list[i].toInt());
		}
	}
	file.close();

	// 用上面获得的数据初始化 render_data
	SPTR_RenderData t_rd = std::dynamic_pointer_cast<RenderData>(ClassFactory::create_object("RenderData"));
	t_rd->init(t_v, t_i);
	render_datas.append(t_rd);

	
}
void Mesh::load_mesh_with_obj(const QString& path) {

}

void Mesh::draw() {
	for (auto rd : render_datas) { rd->draw(); }
}