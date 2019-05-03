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

	std::string map_normal;	// 法线贴图

	// pbr 部分
	CVector3D albedo;
	float metallic;
	float roughness;
	float ao;

	std::string map_albedo;
	std::string map_metallic;
	std::string map_roughness;
	std::string map_ao;


	MaterialData(	
		std::string n = "", 
		CVector3D a = CVector3D(1.0f), CVector3D d = CVector3D(1.0f), CVector3D s = CVector3D(1.0f),
		float sn = 32.0f, 
		std::string ma = "", std::string md = "", std::string ms = "", std::string mn = "",
		CVector3D abd = CVector3D(1.0f), float meta = 0.0f, float rough = 0.5f, float t_ao = 1.0f,
		std::string mabd = "", std::string mmeta = "", std::string mrough = "", std::string mao = "" 
	) : name(n), ka(a), kd(d), ks(s), shininess(sn), 
		map_ka(ma), map_kd(md), map_ks(ms) , map_normal(mn),
		albedo(abd), metallic(meta), roughness(rough), ao(t_ao),
		map_albedo(mabd), map_metallic(mmeta), map_roughness(mrough), map_ao(mao) {}

};

class MaterialGen {
public:

	static bool gen_material_txt(std::string& res, const MaterialData& md, SourceType source_type = SourceType::BY_FILE);
	static bool gen_material_txt_pbr(std::string& res, const MaterialData& md, SourceType source_type = SourceType::BY_FILE);


	~MaterialGen() {}

private:
	MaterialGen() {}

	static void write_one_material(std::ostream& out, const MaterialData& md);
	static void write_one_material_pbr(std::ostream& out, const MaterialData& md);
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
