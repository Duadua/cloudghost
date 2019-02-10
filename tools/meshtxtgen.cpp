#include "meshtxtgen.h"
#include <iostream>
#include <fstream>

std::vector<MVertex> MeshTxtGen::vertices;
std::vector<uint> MeshTxtGen::indices;

bool MeshTxtGen::gen_mesh_txt(const std::string& path, MeshTxtGenType type) {
	std::ofstream out;
	out.open("resources/models/txt/" + path, std::ios::trunc | std::ios::out);
	if (!out.is_open()) { return false; }
	out.fill('0');
	out.precision(6);
	out.setf(std::ios::fixed, std::ios::floatfield);

	vertices.clear();
	indices.clear();

	switch (type) {
	case TRIANGLE:	gen_triangle(); break;
	case RECTANGLE:	gen_rect(); break;
	case CIRCLE:	gen_circle(); break;
	default:		break;
	}

	cac_normal();
	write_to_file(out);

	out.close();
	return true;
}

void MeshTxtGen::gen_triangle() {
	MVertex a(CVector3D(-0.5f, -0.5f, 0.0f));
	MVertex b(CVector3D( 0.5f, -0.5f, 0.0f));
	MVertex c(CVector3D(-0.5f,  0.5f, 0.0f));
	a.tex_coord = a.position.xy();
	b.tex_coord = b.position.xy();
	c.tex_coord = c.position.xy();

	add_one_vertex(a);
	add_one_vertex(b);
	add_one_vertex(c);

	add_one_face(0, 1, 2);
}
void MeshTxtGen::gen_rect() {

}
void MeshTxtGen::gen_circle() {

}

void MeshTxtGen::write_to_file(std::ofstream& out) {
	for (auto i : vertices) { write_one_vertex(out, i); }
	out << std::endl << std::endl;

	for (uint i = 0; i < indices.size(); i += 3) {
		write_one_face(out, indices[i+0], indices[i+1], indices[i+2]);
	}
}
void MeshTxtGen::write_one_vertex(std::ofstream& out, const MVertex& vertex) {
	out << "v ";
	if (vertex.position.x() >=0.0f) out << " "; out << vertex.position.x() << ", ";
	if (vertex.position.y() >= 0.0f) out << " "; out << vertex.position.y() << ", ";
	if (vertex.position.z() >= 0.0f) out << " "; out << vertex.position.z() << ", ";

	if (vertex.normal.x() >= 0.0f) out << " "; out << vertex.normal.x() << ", ";
	if (vertex.normal.y() >= 0.0f) out << " "; out << vertex.normal.y() << ", ";
	if (vertex.normal.z() >= 0.0f) out << " "; out << vertex.normal.z() << ", ";
	
	if (vertex.tex_coord.x() >= 0.0f) out << " "; out << vertex.tex_coord.x() << ", ";
	if (vertex.tex_coord.y() >= 0.0f) out << " "; out << vertex.tex_coord.y() << ", ";
	out << std::endl;
}
void MeshTxtGen::write_one_face(std::ofstream& out, uint a, uint b, uint c) {
	out << "f ";
	out << a << ", "; out << b << ", "; out << c << ", "; 
	out << std::endl;
}

void MeshTxtGen::add_one_vertex(const MVertex& x) { vertices.push_back(x); }
void MeshTxtGen::add_one_face(uint a, uint b, uint c) {
	CVector3D ab(vertices[b].position - vertices[a].position);
	CVector3D ac(vertices[c].position - vertices[a].position);
	float t = ab.xy().cross(ac.xy());
	if (t < 1e-3) { std::swap(b, c); }		// ±£Ö¤ÄæÊ±Õë

	indices.push_back(a); indices.push_back(b); indices.push_back(c);
}

void MeshTxtGen::cac_normal() {
	for (auto& i : vertices) { i.normal = CVector3D(); }

	for (uint i = 0; i < indices.size(); i += 3) {
		auto& a = vertices[indices[i+0]];
		auto& b = vertices[indices[i+1]];
		auto& c = vertices[indices[i+2]];
		
		// cac face normal
		CVector3D t_normal = (b.position - a.position).cross(c.position - a.position);
		// accumulate normal
		a.normal += t_normal;
		b.normal += t_normal;
		c.normal += t_normal;
	}

	// normalize normal
	for (auto& i : vertices) { i.normal.normalize(); }
}




