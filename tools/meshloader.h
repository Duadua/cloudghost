#pragma once

#include "loader.h"
#include "materialloader.h"
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

struct MeshData {
	std::vector<MVertex> vertices;
	std::vector<uint> indices;
	MaterialData material;
};

class MeshTxtGen {
public:
	static bool gen_mesh_txt(std::string& res, MeshTxtGenType type, uint depth = 32, SourceType source_type = SourceType::BY_FILE);

	~MeshTxtGen() {}

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
	static void write(std::ostream& out);

	static void write_one_vertex(std::ostream& out, const MVertex& vertex);
	static void write_one_face(std::ostream& out, uint a, uint b, uint c);
	static void write_one_material(std::ostream& out, const std::string& mt);
	static void write_one_mtfile(std::ostream& out, const std::string& path);

	static void add_one_vertex(const MVertex& x);
	static void add_one_face(uint a, uint b, uint c);
	static void add_one_material(const std::string& mt);
	static void add_one_mtfile(const std::string& path);

	static void cac_normal();

	static std::vector<MVertex> vertices;
	static std::vector<MeshData> mesh_datas;
	static std::vector<std::string> mt_files;
};

// ===============================================================================================

class MeshLoader {

public:

	static bool load_mesh_txt(const std::string& src, std::vector<MeshData>& mds, std::vector<std::string>& mt_files, SourceType source_type = SourceType::BY_FILE);
	static bool load_mesh_obj(const std::string& src, std::vector<MeshData>& mds, std::vector<std::string>& mt_files, SourceType source_type = SourceType::BY_FILE);

	// load mesh by assimp
	static bool load_mesh_x(const std::string& path, std::vector<MeshData>& mds);

	~MeshLoader(){}

private:
	MeshLoader(){}
};

