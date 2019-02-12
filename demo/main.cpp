#include "cpbr.h"
#include "reflex.h"
#include "gameobject.h"
#include "mygamemanager.h"
#include <QDebug>
#include <QtWidgets/QApplication>

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

#include "tools/meshtxtgen.h"

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
	if (!MeshTxtGen::gen_mesh_txt("cylinder.txt", MeshTxtGenType::CYLINDER, 32)) {
		qDebug() << "cannot gen txt" << endl;
	}

	MyGameManager gm;
	CPBR w;
	w.show();
	return a.exec();

}
