#include "skeleton.h"
#include <cassert>

IMPLEMENT_CLASS(Skeleton)

void Skeleton::init(int r, const std::map<std::string, int>& m_n, const std::vector<SkeletonNode>& n, const std::vector<Bone>& b) {
	clear();
	root_node = r;
	map_nodes.insert(m_n.begin(), m_n.end());
	nodes.assign(n.begin(), n.end());
	bones.assign(b.begin(), b.end());
}
void Skeleton::copy_from(const SPTR_Skeleton b) {
	clear();
	root_node = b->root_node;
	map_nodes.insert(b->map_nodes.begin(), b->map_nodes.end());
	nodes.assign(b->nodes.begin(), b->nodes.end());
	bones.assign(b->bones.begin(), b->bones.end());
}

SkeletonNode& Skeleton::get_node(const std::string& n) {
	assert(map_nodes.count(n));
	return nodes[map_nodes[n]];
}

Skeleton& Skeleton::add_node(const SkeletonNode& node) {
	if (map_nodes.count(node.name)) { return (*this); }
	int t_id = static_cast<int>(nodes.size());
	if (node.father == -1 && root_node == -1) root_node = t_id;

	map_nodes[node.name] = t_id;
	nodes.push_back(node);
	nodes.back().id = t_id;
	if (node.father >= 0 && node.father < t_id) { nodes[node.father].add_children(t_id); }
	return (*this);
}

Skeleton& Skeleton::add_bone(const Bone& bone, std::string n) {
	if (!map_nodes.count(n)) { return (*this); }
	int t_id = static_cast<int>(bones.size());
	get_node(n).bone_id = t_id;
	bones.push_back(bone);
	return (*this);
}