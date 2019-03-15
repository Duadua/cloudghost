#include "cpbr.h"
#include "reflex.h"
#include "gameobject.h"
#include "mygamemanager.h"
#include <QtWidgets/QApplication>
#include <QMatrix4x4>

#include "tools/meshloader.h"
#include "tools/materialloader.h"
#include "tools/textureloader.h"

#include "tools/qtimagehelper.h"

#include "plane.h"

#ifdef C_DEBUG
#include "loader.h"
#endif // C_DEBUG

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	
	/* test reflex
	CObject* obj = ClassFactory::create_object("CObject");
	c_debug() << obj->get_class_info()->get_class_name();
	
	CObject* gobj = ClassFactory::create_object("GameObject");
	c_debug() << gobj->get_class_info()->get_class_name();
	*/

	// gen mesh txt
	/*QString path = "hhh.txt";
	if (!MeshTxtGen::gen_mesh_txt(path.toStdString(), MeshTxtGenType::CONE)) {
		c_debug() << "cannot gen mesh txt";
	}*/
	
	// gen material txt
	/*QString path = "default_material.txt";
	MaterialData md("default", CVector3D(0.1f), CVector3D(0.7f), CVector3D(0.2f), 32.0f, "texture_default.txt", "texture_default.txt");
	if (!MaterialGen::gen_material_txt(path.toStdString(), md)) {
		c_debug() << "cannot gen material txt";
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

	{
		CVector3D v(10.0, 20.0, 30.0);
		CMatrix4x4 t;
		t.rotate(20.0f, 30.0f, 40.0f, 50.0f);
		v = t * v;
		c_debug() << t.to_string();
		//qDebug() << v[0] << " " << v[1] << " " << v[2];
		//qDebug() << v.length();

	}
	{
		CVector3D v(10.0, 20.0, 30.0);
		CVector3D ax(30.0f, 40.0f, 50.0f);
		CQuaternion t_r(20.0f, ax);
		CMatrix4x4 t;
		t.rotate_quaternion(t_r);
		v = t * v;

		//qDebug() << QMatrix4x4(t.get_transpose().data());
		//qDebug() << v[0] << " " << v[1] << " " << v[2];
		//qDebug() << v.length();

		//qDebug() << ax[0] << " " << ax[1] << " " << ax[2];
		//qDebug() << aa[0] << " " << aa[1] << " " << aa[2] << " " << aa[3];
		//qDebug() << v[0] << " " << v[1] << " " << v[2];
	}
	
#ifdef C_DEBUG
	// test c_debug()
	{ c_debug() << "【debug】start"; }
#endif // C_DEBUG

	MyGameManager gm;
	CPBR w;
	w.show();

	return a.exec();

}
