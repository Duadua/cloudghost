#pragma once

#include "cmath.h"
#include "reflex.h"
#include <string>

PRE_DECLARE_CLASS(Shader)
USING_SPTR(Shader)

DECLARE_AUTO_PTR(Material)
class Material : public CObject {
	DECLARE_CLASS(Material)
public:
	Material();
	Material(const Material& b);
	void copy_from(const SPTR_Material b);
	~Material() {}

	void use(SPTR_Shader shader);
	static void un_use(SPTR_Shader shader);

    GET_SET(std::string, name)

    GET_SET(CVector3D, ka)
    GET_SET(CVector3D, kd)
    GET_SET(CVector3D, ks)

    GET_SET(std::string, map_ka)
    GET_SET(std::string, map_kd)
    GET_SET(std::string, map_ks)
    GET_SET(std::string, map_normal)

    GET_SET(float, shininess)

	GET_SET(CVector3D, albedo)
	GET_SET(float, metallic)
	GET_SET(float, roughness)
	GET_SET(float, ao)

	GET_SET(std::string, map_albedo)
	GET_SET(std::string, map_metallic)
	GET_SET(std::string, map_roughness)
	GET_SET(std::string, map_ao)

	static std::string default_material_name;

private:
	std::string name;

	CVector3D ka;
	CVector3D kd;
	CVector3D ks;

	float shininess;
	
	std::string map_ka;
	std::string map_kd;
	std::string map_ks;

	std::string map_normal;		// 法线贴图

	// pbr 部分
	CVector3D albedo;
	float metallic;
	float roughness;
	float ao;

	std::string map_albedo;
	std::string map_metallic;
	std::string map_roughness;
	std::string map_ao;

};


