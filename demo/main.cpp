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

	CMatrix4x4 t;
	//t.translate(CVector3D(1.0f, 2.0f, 3.0f));
	t.translate(1.0f, 2.0f);
	t.scale(3.0f);
	qDebug() << QMatrix4x4(t.get_transpose().data());

	QMatrix4x4 tt;
	tt.rotate(50.0f, QVector3D(0.0f, 1.0f, 0.0f));
	qDebug() << tt << endl;
	CMatrix4x4 ttt(tt.data());
	ttt.scale(3.0f, 3.0f, 4.0f);
	qDebug() << QMatrix4x4(ttt.get_transpose().data());
	MyGameManager gm;
	CPBR w;
	w.show();
	return a.exec();

}
