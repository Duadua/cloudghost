#include "reflex.h"
#include "gameobject.h"
#include "cmainwindow.h"
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
		
#ifdef C_DEBUG
	// test c_debug()
	{ c_debug() << "[debug]start"; }
#endif // C_DEBUG

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

	/*
	{
		CMatrix4x4 t;
		t.rotate(45.0f, 1.0f, 0.0f, 0.0f);
		c_debug() << "use zhoujiao " + t.to_string();

	}
	{
		CMatrix4x4 t;
		t.rotate_euler(45.0f, 0.0f, 0.0f);
		c_debug() << "use euler " + t.to_string();
	}
	*/

	MyGameManager gm;
	CMainWindow cmw;
	cmw.show();

	return a.exec();

}
