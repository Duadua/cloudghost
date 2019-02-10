#pragma once

#include "cmath.h"
#include <string>
#include <vector>

enum MeshTxtGenType {
	TRIANGLE,
	RECTANGLE,
	CIRCLE
};

struct MVertex {
	CVector3D position;
	CVector3D normal;
	CVector2D tex_coord;
	MVertex(CVector3D pos = CVector3D(), CVector3D norm = CVector3D(), CVector2D coord = CVector2D()) 
	: position(pos), normal(norm), tex_coord(coord) {}
};
struct MTriangle {
	MVertex a, b, c;
	CVector3D normal;
	MTriangle(MVertex i, MVertex j, MVertex k) : a(i), b(j), c(k) {
		sort(); 
		gen_normal();
	}

	void sort() {
		CVector3D ab(b.position - a.position);
		CVector3D ac(c.position - a.position);
		float t = ab.xy().cross(ac.xy());
		if (t < 1e-3) { MVertex d = b; b = c; c = d; }
	} // 逆时针排序 
	void gen_normal() { normal = (b.position-a.position).cross(c.position - a.position); } // 生成法线
};

class MeshTxtGen {
public:
	~MeshTxtGen() {}
	static bool gen_mesh_txt(const std::string& path, MeshTxtGenType type);

private:
	MeshTxtGen() {}

	static void gen_triangle();
	static void gen_rect();
	static void gen_circle();

private:
	static void write_to_file(std::ofstream& out);

	static void write_one_vertex(std::ofstream& out, const MVertex& vertex);
	static void write_one_face(std::ofstream& out, uint a, uint b, uint c);

	static void add_one_vertex(const MVertex& x);
	static void add_one_face(uint a, uint b, uint c);

	static void cac_normal();

	static std::vector<MVertex> vertices;
	static std::vector<uint> indices;
};

