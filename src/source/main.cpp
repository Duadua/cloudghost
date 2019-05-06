#include "cmainwindow.h"
//#include "mygamemanager.h"
#include <QtWidgets/QApplication>
#include <QSplashScreen>

#include "tools/meshloader.h"
#include "tools/materialloader.h"

/*#include "tools/meshloader.h"
#include "tools/materialloader.h"
#include "tools/textureloader.h"
#include "tools/qtimagehelper.h"
*/

#include "timemanager.h"
#include "singleton.h"
#include "cmath.h"
#include "stringhelper.h"

#include "assetmanager.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
		
	/* test reflex
	CObject* obj = ClassFactory::create_object("CObject");
	c_debuger() << obj->get_class_info()->get_class_name();
	
	CObject* gobj = ClassFactory::create_object("GameObject");
	c_debuger() << gobj->get_class_info()->get_class_name();
	*/

	// gen mesh txt
	QString path = "sphere.txt";
	if (!MeshTxtGen::gen_mesh_txt(path.toStdString(), MeshTxtGenType::SPHERE)) {
		c_debuger() << "cannot gen mesh txt";
	}
	
	// gen material txt
	/*QString path = "pbr_material.txt";
	MaterialData md("pbr_rusted_iron", CVector3D(0.1f), CVector3D(0.7f), CVector3D(0.2f), 32.0f, 
					"texture_default.png", "texture_default.png", "", "rusted_iron_normal.png",
					CVector3D(0.5f, 0.0f, 0.0f), 0.0f, 0.5f, 1.0f,
					"rusted_iron_albedo.png", "rusted_iron_metallic.png", "rusted_iron_roughness.png",
					"rusted_iron_ao.png");
	if (!MaterialGen::gen_material_txt(path.toStdString(), md)) {
		c_debuger() << "cannot gen material txt";
	}
	*/
	

	// repair png
	//std::string str = "resources";
	//QtImageHelper::repair_png(str);

	// texture load test
	/*{
		std::string path = "resources/textures/wood.png";
		std::string path2 = "resources/textures/wood2.png";
		//TextureGen::gen_texture_txt(path, TextureGenType::THE_TWO, CColor(80, 80, 80, 255));
		int w, h, n;
		auto t_data = TextureLoader::load_texture_x(path, w, h, n);
		QtImageHelper::data_to_png(path2, t_data, w, h);
	}
	*/

	// change txt to png
	/*
	uint width, heigh;
	std::string path = "resources/textures/txt/texture_default.txt";
	QtImageHelper::text_to_png(path);
	*/

	/*
	{
		CMatrix4x4 t;
		t.rotate(45.0f, 1.0f, 0.0f, 0.0f);
		c_debuger() << "use zhoujiao " + t.to_string();

	}
	{
		CMatrix4x4 t;
		t.rotate_euler(45.0f, 0.0f, 0.0f);
		c_debuger() << "use euler " + t.to_string();
	}
	*/

	// test timemanager
	/*{
		c_debuger() << TimeManager_ins().cur_time_data().to_string();
		c_debuger() << std::to_string(TimeManager_ins().cur_time_msconds());
		c_debuger() << std::to_string(TimeManager_ins().cur_runtime_msconds());
	}*/
	
	/*CMatrix4x4 mat4x4(
		5.0f, 2.0f, -0.0f, 0.0f,
		2.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 8.0f, 3.0f,
		0.0f, 0.0f, 5.0f, 2.0f
		);
	CMatrix3x3 mat3x3( mat4x4.data(), 4, 4 );
	CMatrix4x4 res(mat3x3.data(), 3, 3);
	c_debuger() << res.to_string();
	*/
	
	
	// test quaternion slerp
	/*{
		CQuaternion a(0.0f, 0.0f, 0.0f, 1.0f);
		CQuaternion b(0.0f, 0.0f, 0.707f, 0.707f);
		c_debuger() << StringHelper_ins().to_string(slerp(a, b, 0.333f));

		aiQuaternion aa(1.0f, 0.0f, 0.0f, 0.0f);
		aiQuaternion bb(0.707f, 0.0f, 0.0f, 0.707f);
		aiQuaternion res;
		aiQuaternion::Interpolate(res, aa, bb, 0.333f);
		res.Normalize();
		c_debuger() << std::to_string(res.x);
		c_debuger() << std::to_string(res.y);
		c_debuger() << std::to_string(res.z);
		c_debuger() << std::to_string(res.w);
	}
	*/

	QSplashScreen* splash = new QSplashScreen();
	splash->setPixmap(QPixmap("resources/textures/cover.png"));
	splash->setFont(QFont("Consolas", 18, 100));
	splash->showMessage(QObject::tr("CloudGhost V1.3...\t"),  Qt::AlignBottom | Qt::AlignRight, Qt::white);
	splash->show();
  
//	MyGameManager gm;
	CMainWindow cmw;
	cmw.show();
	splash->finish(&cmw);
	delete splash;
	return a.exec();

}
