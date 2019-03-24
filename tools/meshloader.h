#pragma once

#include "loader.h"
#include "materialloader.h"
#include <map>
#include <cassert>
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

// ===============================================================================================
// skeleton mesh loader data
const int m_bone_num_per_vertex = 4;
struct MVertexBone {
	uint ids[m_bone_num_per_vertex];
	float weights[m_bone_num_per_vertex];
	MVertexBone();
	MVertexBone& add(uint id, float weigtht);
};
struct SkeletalMeshData {
	std::vector<MVertex> vertices;
	std::vector<uint> indices;
	MaterialData material;
	std::vector<MVertexBone> bones;
};

struct MBone {
	CMatrix4x4 mat_offset;
	CMatrix4x4 mat_finall;
	MBone() { mat_offset = CMatrix4x4(); mat_finall = CMatrix4x4(); }
};
struct MSkeletonNode {
	std::string name;
	int id;
	int father;
	std::vector<int> children;

	int bone_id;					// if -1 then no bone

	MSkeletonNode() {}
	MSkeletonNode(const std::string& n, int f = -1, int i = -1, int b = -1)
		: name(n), id(i), father(f), bone_id(b) {
		children.clear();
	}
	MSkeletonNode& add_children(int i) { children.push_back(i); return (*this); }
};	// 单个骨骼层次节点
struct MSkeleton {
	int root_node;
	std::map<std::string, int> map_nodes;			// 节点名称到节点下标的映射
	std::vector<MSkeletonNode> nodes;				// 所有的节点序列 -- 每个节点有其孩子和父亲的下标信息 -- 形成树

	MSkeleton() { clear(); }
	void clear() { root_node = -1; map_nodes.clear(); nodes.clear(); }

	MSkeletonNode& get_node(const std::string& n) {
		assert(map_nodes.count(n));
		return nodes[map_nodes[n]];
	}
	MSkeleton& add_node(const MSkeletonNode& node) {
		if (map_nodes.count(node.name)) { return (*this); }
		int t_id = static_cast<int>(nodes.size());
		if (node.father == -1 && root_node == -1) root_node = t_id;

		map_nodes[node.name] = t_id;
		nodes.push_back(node);
		nodes.back().id = t_id;
		if (node.father >= 0 && node.father < t_id) { nodes[node.father].add_children(t_id); }
		return (*this);
	}

	MSkeleton& add_bone(int bid, std::string n) {
		if (!map_nodes.count(n)) { return (*this); }
		get_node(n).bone_id = bid;
		return (*this);
	} // bone 与 nodes 的映射关系

	std::string name;

};	// 骨骼层次树

// ===============================================================================================

// 动画相关
struct MAnimPositionKey {
	float time;
	CVector3D value;
	MAnimPositionKey() {}
	MAnimPositionKey(float t, const CVector3D& v) : time(t), value(v) {}
};
struct MAnimRotationKey {
	float time;
	CQuaternion value;
	MAnimRotationKey() {}
	MAnimRotationKey(float t, const CQuaternion& v) : time(t), value(v) {}
};
struct MAnimScaleKey {
	float time;
	CVector3D value;
	MAnimScaleKey() {}
	MAnimScaleKey(float t, const CVector3D& v) : time(t), value(v) {}
};

struct MAnimNode {
	int id;
	std::string name;								// node name 与 骨骼层次里的 node 名称对应
	std::vector<MAnimPositionKey> position_keys;
	std::vector<MAnimRotationKey> rotation_keys;
	std::vector<MAnimScaleKey> scale_keys;

	MAnimNode() {}
	MAnimNode(const MAnimNode& b) : id(b.id), name(b.name) {
		position_keys.clear(); position_keys.assign(b.position_keys.begin(), b.position_keys.end());
		rotation_keys.clear(); rotation_keys.assign(b.rotation_keys.begin(), b.rotation_keys.end());
		scale_keys.clear(); scale_keys.assign(b.scale_keys.begin(), b.scale_keys.end());
	}

};
struct AnimData {

	std::map<std::string, int> map_anim_nodes;
	std::vector<MAnimNode> anim_nodes;

	std::string name;
	float duration;									// 持续时间 -- 总共的帧数
	float ticks_per_seconds;						// 每秒多少帧 -- 1000.0 / ticks_per_seconds 为每帧多少毫秒

	AnimData& add_node(const MAnimNode& an) {
		if (map_anim_nodes.count(an.name)) { return (*this); }
		int t_id = static_cast<int>(anim_nodes.size());

		map_anim_nodes[an.name] = t_id;
		anim_nodes.push_back(an);
		anim_nodes.back().id = t_id;
		return (*this);
	}

};

// ===============================================================================================

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
	static bool load_mesh_skeletal(const std::string& path, std::vector<SkeletalMeshData>& mds, MSkeleton& skeleton, std::vector<MBone>& bones, std::vector<AnimData>& ads);
	static bool load_mesh_animation(const std::string& path, std::vector<AnimData>& ads);

	~MeshLoader(){}

private:
	MeshLoader(){}
};

