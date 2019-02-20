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
	/*QString path = "cylinder.txt";
	if (!MeshTxtGen::gen_mesh_txt(path.toStdString(), MeshTxtGenType::CYLINDER)) {
		qDebug() << "cannot gen mesh txt" << endl;
	}
	*/
	
	// gen material txt
	/*QString path = "cube_material.txt";
	MaterialData md("cube_wood", CVector3D(0.135, 0.2225, 0.1575), CVector3D(0.54, 0.89, 0.63), CVector3D(0.316228, 0.316228, 0.316228), 12.8f, "", "wood.png");
	if (!MaterialGen::gen_material_txt(path.toStdString(), md)) {
		qDebug() << "cannot gen material txt" << endl;
	}
	*/
	
	// repair png
	//std::string str = "resources";
	//QtImageHelper::repair_png(str);

	MyGameManager gm;
	CPBR w;
	w.show();
	return a.exec();

}
