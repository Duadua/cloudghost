#include "cpbr.h"
#include "reflex.h"
#include "gameobject.h"
#include "mygamemanager.h"
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QImage>

// test shared_ptr
/*
#include <memory>
class Test {
public:
	Test() { qDebug() << "new test" << endl; }
	virtual void run() { qDebug() << "test" << endl; }
	virtual ~Test() { qDebug() << "delete test" << endl; }
};
DECLARE_AUTO_PTR(Test)

class A : public Test {
public:
	A() { qDebug() << "new a" << endl; }
	virtual void run() { qDebug() << "a" << endl; }
	virtual ~A() { qDebug() << "delete a" << endl; }
};
DECLARE_AUTO_PTR(A)
*/

#include "tools/meshloader.h"
#include "tools/materialloader.h"
#include "tools/textureloader.h"

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

	// test shared ptr
	/*
	SPTR_A sp_a(new A());
	qDebug() << sp_a.use_count() << endl;
	SPTR_Test sp_t(new Test());
	qDebug() << sp_t.use_count() << endl;
	sp_t = sp_a;
	qDebug() << sp_t.use_count() << endl;
	sp_a->run();
	sp_t->run();
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
	
	MyGameManager gm;
	CPBR w;
	w.show();
	return a.exec();

}
