#pragma once

#include "cmath.h"
#include "reflex.h"
#include <string>

class Material : public CObject {
	DECLARE_CLASS(Material)
public:
	Material();
	~Material() {}

	void use(const std::string& shader);

	GET_SET(CVector3D, ka);
	GET_SET(CVector3D, kd);
	GET_SET(CVector3D, ks);

	GET_SET(std::string, map_ka);
	GET_SET(std::string, map_kd);
	GET_SET(std::string, map_ks);

	GET_SET(float, shininess);

private:
	CVector3D ka;
	CVector3D kd;
	CVector3D ks;

	std::string map_ka;
	std::string map_kd;
	std::string map_ks;

	float shininess;

};
DECLARE_AUTO_PTR(Material)
