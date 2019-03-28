#pragma once

#include "cmath.h"
#include "reflex.h"
#include <map>

struct SkeletonNode {
	std::string name;
	int id;
	int father;
	std::vector<int> children;

	int bone_id;					// if -1 then no bone
	CMatrix4x4 mat_trans;			// 变换矩阵

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
	void clear() { root_node = -1; map_nodes.clear(); nodes.clear(); }
	void init(int r, const std::map<std::string, int>& m_n, const std::vector<SkeletonNode>& n);
	void copy_from(const SPTR_Skeleton b);
	~Skeleton() {}

	SkeletonNode& get_node(const std::string& n); 
	SkeletonNode& get_node(int nid);
	void add_node(const SkeletonNode& node);
	void add_bone(int bid, std::string n);

	GET_SET(std::string, name)
	GET_SET(int, root_node)

private:
	int root_node;
	std::map<std::string, int> map_nodes;			// 节点名称到节点下标的映射
	std::vector<SkeletonNode> nodes;				// 所有的节点序列 -- 每个节点有其孩子和父亲的下标信息 -- 形成层次树

	std::string name;
};



