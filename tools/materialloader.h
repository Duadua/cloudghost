#pragma once

#include "loader.h"

struct MaterialData {
	std::string name;

	CVector3D ka;
	CVector3D kd;
	CVector3D ks;

	float shininess;

	std::string map_ka;
	std::string map_kd;
	std::string map_ks;

	MaterialData(std::string n = "", CVector3D a = CVector3D(1.0f), CVector3D d = CVector3D(1.0f), CVector3D s = CVector3D(1.0f),
		float sn = 32.0f, std::string ma = "", std::string md = "", std::string ms = "") 
		: name(n), ka(a), kd(d), ks(s), shininess(sn), map_ka(ma), map_kd(md), map_ks(ms) {}

};

class MaterialGen {
public:

	static bool gen_material_txt(std::string& res, const MaterialData& md, SourceType source_type = SourceType::BY_FILE);


	~MaterialGen() {}

private:
	MaterialGen() {}

	static void write_one_material(std::ostream& out, const MaterialData& md);
};

// ===============================================================================================

class MaterialLoader {
public:

	static bool load_material_txt(const std::string& src, std::vector<MaterialData>& md, SourceType source_type = SourceType::BY_FILE);
	static bool load_material_mtl(const std::string& src, std::vector<MaterialData>& md, SourceType source_type = SourceType::BY_FILE);

	~MaterialLoader() {}

private:
	MaterialLoader() {}

};
