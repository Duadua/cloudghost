#pragma once

#include "cmath.h"
#include "reflex.h"
#include <map>

struct Bone {
	CMatrix4x4 mat_offset;
	CMatrix4x4 mat_finall;
	Bone() { mat_offset = CMatrix4x4(); mat_finall = CMatrix4x4(); }
};

struct SkeletonNode {
	std::string name;
	int id;
	int father;
	std::vector<int> children;

	int bone_id;					// if -1 then no bone

	SkeletonNode() { children.clear(); }
	SkeletonNode(const std::string& n, int f = -1, int i = -1, int b = -1)
		: name(n), id(i), father(f), bone_id(b) {
		children.clear();
	}
	SkeletonNode& add_children(int i) { children.push_back(i); return (*this); }
};

DECLARE_AUTO_PTR(Skeleton)
class Skeleton : public CObject {
	DECLARE_CLASS(Skeleton)

public:

	Skeleton() { clear(); }
	void clear() { root_node = -1; map_nodes.clear(); nodes.clear(); bones.clear(); }
	void init(int r, const std::map<std::string, int>& m_n, const std::vector<SkeletonNode>& n, const std::vector<Bone>& b);
	void copy_from(const SPTR_Skeleton b);
	~Skeleton() {}

	SkeletonNode& get_node(const std::string& n); 
	Skeleton& add_node(const SkeletonNode& node);
	Skeleton& add_bone(const Bone& bone, std::string n);

	GET_SET(std::string, name)

private:
	int root_node;
	std::map<std::string, int> map_nodes;			// 节点名称到节点下标的映射
	std::vector<SkeletonNode> nodes;				// 所有的节点序列 -- 每个节点有其孩子和父亲的下标信息 -- 形成树

	std::vector<Bone> bones;						// 所有的真正的骨骼节点信息 -- 与层次节点一对一，但是并非所有的层次节点都有骨骼

	std::string name;
};



