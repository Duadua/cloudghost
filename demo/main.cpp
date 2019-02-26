#include "cpbr.h"
#include "reflex.h"
#include "gameobject.h"
#include "mygamemanager.h"
#include <QtWidgets/QApplication>
#include <QDebug>

#include "tools/meshloader.h"
#include "tools/materialloader.h"
#include "tools/textureloader.h"

#include "tools/qtimagehelper.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	
	/* test reflex
	CObject* obj = ClassFactory::create_object("CObject");
	qDebug() << QString::fromStdString(obj->get_class_info()->get_class_name()) << endl;
	
	CObject* gobj = ClassFactory::create_object("GameObject");
	qDebug() << QString::fromStdString(gobj->get_class_info()->get_class_name()) << endl;

	auto com = ClassFactory::create_object("Component");
	qDebug() << QString::fromStdString(com->get_class_info()->get_class_name()) << endl;

	auto tmp = ClassFactory::create_object("CameraComponent");
	qDebug() << QString::fromStdString(tmp->get_class_info()->get_class_name()) << endl;
	*/

	// gen mesh txt
	/*QString path = "rect.txt";
	if (!MeshTxtGen::gen_mesh_txt(path.toStdString(), MeshTxtGenType::RECTANGLE)) {
		qDebug() << "cannot gen mesh txt" << endl;
	}
	*/
	
	
	// gen material txt
	/*QString path = "default_material.txt";
	MaterialData md("default", CVector3D(0.1f), CVector3D(0.7f), CVector3D(0.2f), 32.0f, "texture_default.txt", "texture_default.txt");
	if (!MaterialGen::gen_material_txt(path.toStdString(), md)) {
		qDebug() << "cannot gen material txt" << endl;
	}
	*/

	// repair png
	//std::string str = "resources";
	//QtImageHelper::repair_png(str);

	// texture load test
	/*{
		std::string path = "texture_default.txt";
		TextureGen::gen_texture_txt(path, TextureGenType::THE_TWO, CColor(80, 80, 80, 255));
	}

	// change txt to png
	uint width, heigh;
	std::string path = "resources/textures/txt/texture_default.txt";
	QtImageHelper::text_to_png(path);
	*/

	const float t[2] = { 1.0f, 2.0f };
	CVector2D c(t);
	c[0] = 3.0f;
	qDebug() << c[0] << " " << c[1] << endl;

	MyGameManager gm;
	CPBR w;
	w.show();
	return a.exec();

}
