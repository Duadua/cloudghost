#pragma once

#include "cmath.h"
#include <string>
#include <vector>

enum MeshTxtGenType {
	CIRCLE,						// 圆
	RECTANGLE,					// 矩形
	TRIANGLE_RIGHT,				// 直角
	TRIANGLE_REGULAR,			// 等边

	CUBE,						// 立方体
	CONE,						// 锥体
	SPHERE,						// 球体
	CYLINDER					// 柱体
};

struct MVertex {
	CVector3D position;
	CVector3D normal;
	CVector2D tex_coord;
	MVertex(CVector3D pos = CVector3D(), CVector3D norm = CVector3D(), CVector2D coord = CVector2D()) 
	: position(pos), normal(norm), tex_coord(coord) {}
};

class MeshTxtGen {
public:
	~MeshTxtGen() {}
	static bool gen_mesh_txt(const std::string& path, MeshTxtGenType type, uint depth = 32);

private:
	MeshTxtGen() {}

	// 2d
	static void gen_triangle_right();
	static void gen_triangle_regular();
	static void gen_rect();
	static void gen_circle(uint depth = 32);

	// 3d
	static void gen_cube();
	static void gen_cone(uint depth = 32);
	static void gen_sphere(uint depth = 5);
	static void gen_cylinder(uint depth = 32);

private:
	static void write_to_file(std::ofstream& out);

	static void write_one_vertex(std::ofstream& out, const MVertex& vertex);
	static void write_one_face(std::ofstream& out, uint a, uint b, uint c);

	static void add_one_vertex(const MVertex& x);
	static void add_one_vertex(std::vector<MVertex>& v, const MVertex& x);
	static void add_one_face(uint a, uint b, uint c);
	static void add_one_face(const std::vector<MVertex>& v, std::vector<uint>& idx, uint a, uint b, uint c);

	static void cac_normal();

	static std::vector<MVertex> vertices;
	static std::vector<uint> indices;
};

