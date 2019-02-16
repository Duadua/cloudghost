#include "meshloader.h"
#include <memory>
#include <sstream>
#include <fstream>
#include <QDebug>

std::vector<MVertex> MeshTxtGen::vertices;
std::vector<uint> MeshTxtGen::indices;

bool MeshTxtGen::gen_mesh_txt(std::string& res, MeshTxtGenType type, uint depth, SourceType source_type) {
	std::ostream* out;
	std::ofstream fs;
	std::stringstream ss;
	if (source_type == SourceType::BY_FILE) {
		fs.open("resources/models/txt/" + res, std::ios::trunc | std::ios::out);
		if (!fs.is_open()) { return false; }
		out = &fs;
	}
	else if (source_type == SourceType::BY_STRING) { ss.clear(); out = &ss; }
	out->fill('0');
	out->precision(6);
	out->setf(std::ios::fixed, std::ios::floatfield);

	vertices.clear();
	indices.clear();

	bool has_normal = false;
	switch (type) {
	case CIRCLE:			gen_circle(depth); break;
	case RECTANGLE:			gen_rect(); break;
	case TRIANGLE_RIGHT:	gen_triangle_right(); break;
	case TRIANGLE_REGULAR:	gen_triangle_regular(); break;
	case CUBE:				gen_cube(); break;
	case CONE:				gen_cone(depth); break;
	case SPHERE:			gen_sphere(depth); has_normal = true; break;
	case CYLINDER:			gen_cylinder(depth); break;
	default:				break;
	}

	if(!has_normal) cac_normal();

	write((*out));
	if (source_type == SourceType::BY_FILE)		{ fs.close(); }
	else if (source_type == SourceType::BY_STRING) { res = ss.str(); ss.clear(); }

	return true;
}

void MeshTxtGen::gen_triangle_right() {
	MVertex a(CVector3D(-0.5f, -0.5f, 0.0f));
	MVertex b(CVector3D( 0.5f, -0.5f, 0.0f));
	MVertex c(CVector3D(-0.5f,  0.5f, 0.0f));

	a.tex_coord = a.position.xy() + CVector2D(0.5f);
	b.tex_coord = b.position.xy() + CVector2D(0.5f);
	c.tex_coord = c.position.xy() + CVector2D(0.5f);

	add_one_vertex(a);
	add_one_vertex(b);
	add_one_vertex(c);

	add_one_face(0, 1, 2);

}
void MeshTxtGen::gen_triangle_regular() {
	float h = std::sin(CMath::deg_to_rad(60.0f));
	MVertex a(CVector3D(-0.5f, -0.5f, 0.0f));
	MVertex b(CVector3D(0.5f, -0.5f, 0.0f));
	MVertex c(CVector3D(0.0f, -0.5f + h, 0.0f));

	a.tex_coord = a.position.xy() + CVector2D(0.5f);
	b.tex_coord = b.position.xy() + CVector2D(0.5f);
	c.tex_coord = c.position.xy() + CVector2D(0.5f);

	add_one_vertex(a);
	add_one_vertex(b);
	add_one_vertex(c);

	add_one_face(0, 1, 2);
}
void MeshTxtGen::gen_rect() {
	MVertex a(CVector3D(-0.5f, -0.5f, 0.0f));
	MVertex b(CVector3D(0.5f, -0.5f, 0.0f));
	MVertex c(CVector3D(0.5f, 0.5f, 0.0f));
	MVertex d(CVector3D(-0.5f, 0.5f, 0.0f));

	a.tex_coord = a.position.xy() + CVector2D(0.5f);
	b.tex_coord = b.position.xy() + CVector2D(0.5f);
	c.tex_coord = c.position.xy() + CVector2D(0.5f);
	d.tex_coord = d.position.xy() + CVector2D(0.5f);

	add_one_vertex(a);
	add_one_vertex(b);
	add_one_vertex(c);
	add_one_vertex(d);

	add_one_face(0, 1, 2);
	add_one_face(0, 2, 3);
}
void MeshTxtGen::gen_circle(uint depth) {
	MVertex o(CVector3D(0.0f, 0.0f, 0.0f), CVector3D(), CVector2D(0.5f));
	add_one_vertex(o);
	float rad = 2.0f*CMath::pi / depth;
	for (uint i = 0; i < depth; ++i) {
		MVertex t(CVector3D(0.5f*std::cos(i*rad), 0.5f*std::sin(i*rad), 0.0f));
		t.tex_coord = t.position.xy() + CVector2D(0.5f);
		add_one_vertex(t);
	}
	for (uint i = 1; i <= depth; ++i) { add_one_face(0, i, i%depth + 1); }
}

void MeshTxtGen::gen_cube() {
	// front
	{
		MVertex a(CVector3D(-0.5f, -0.5f, -0.5f));
		MVertex b(CVector3D(0.5f, -0.5f, -0.5f));
		MVertex c(CVector3D(0.5f, 0.5f, -0.5f));
		MVertex d(CVector3D(-0.5f, 0.5f, -0.5f));

		a.tex_coord = a.position.xy() + CVector2D(0.5f);
		b.tex_coord = b.position.xy() + CVector2D(0.5f);
		c.tex_coord = c.position.xy() + CVector2D(0.5f);
		d.tex_coord = d.position.xy() + CVector2D(0.5f);

		add_one_vertex(a);	// 0
		add_one_vertex(b);	// 1
		add_one_vertex(c);	// 2
		add_one_vertex(d);	// 3

		add_one_face(0, 1, 2);
		add_one_face(0, 2, 3);
	}

	// back
	{
		MVertex a(CVector3D(0.5f, -0.5f, 0.5f), CVector3D(), CVector2D(0.0f, 0.0f));
		MVertex b(CVector3D(-0.5f, -0.5f, 0.5f), CVector3D(), CVector2D(1.0f, 0.0f));
		MVertex c(CVector3D(-0.5f, 0.5f, 0.5f), CVector3D(), CVector2D(1.0f, 1.0f));
		MVertex d(CVector3D(0.5f, 0.5f, 0.5f), CVector3D(), CVector2D(0.0f, 1.0f));

		add_one_vertex(a);	// 4
		add_one_vertex(b);	// 5
		add_one_vertex(c);	// 6
		add_one_vertex(d);	// 7

		add_one_face(4, 5, 6);
		add_one_face(4, 6, 7);
	}

	// right
	{
		MVertex a(CVector3D(0.5f, -0.5f, -0.5f), CVector3D(), CVector2D(0.0f, 0.0f));
		MVertex b(CVector3D(0.5f, -0.5f, 0.5f), CVector3D(), CVector2D(1.0f, 0.0f));
		MVertex c(CVector3D(0.5f, 0.5f, 0.5f), CVector3D(), CVector2D(1.0f, 1.0f));
		MVertex d(CVector3D(0.5f, 0.5f, -0.5f), CVector3D(), CVector2D(0.0f, 1.0f));

		add_one_vertex(a);	// 8
		add_one_vertex(b);	// 9
		add_one_vertex(c);	// 10
		add_one_vertex(d);	// 11

		add_one_face(8, 9, 10);
		add_one_face(8, 10, 11);
	}
	
	// left
	{
		MVertex a(CVector3D(-0.5f, -0.5f, 0.5f), CVector3D(), CVector2D(0.0f, 0.0f));
		MVertex b(CVector3D(-0.5f, -0.5f, -0.5f), CVector3D(), CVector2D(1.0f, 0.0f));
		MVertex c(CVector3D(-0.5f, 0.5f, -0.5f), CVector3D(), CVector2D(1.0f, 1.0f));
		MVertex d(CVector3D(-0.5f, 0.5f, 0.5f), CVector3D(), CVector2D(0.0f, 1.0f));

		add_one_vertex(a);	// 12
		add_one_vertex(b);	// 13
		add_one_vertex(c);	// 14
		add_one_vertex(d);	// 15

		add_one_face(12, 13, 14);
		add_one_face(12, 14, 15);
	}

	// top
	{
		MVertex a(CVector3D(-0.5f, 0.5f, -0.5f), CVector3D(), CVector2D(0.0f, 0.0f));
		MVertex b(CVector3D(0.5f, 0.5f, -0.5f), CVector3D(), CVector2D(1.0f, 0.0f));
		MVertex c(CVector3D(0.5f, 0.5f, 0.5f), CVector3D(), CVector2D(1.0f, 1.0f));
		MVertex d(CVector3D(-0.5f, 0.5f, 0.5f), CVector3D(), CVector2D(0.0f, 1.0f));

		add_one_vertex(a);	// 16
		add_one_vertex(b);	// 17
		add_one_vertex(c);	// 18
		add_one_vertex(d);	// 19

		add_one_face(16, 17, 18);
		add_one_face(16, 18, 19);
	}

	// bottom
	{
		MVertex a(CVector3D(-0.5f, -0.5f, 0.5f), CVector3D(), CVector2D(0.0f, 0.0f));
		MVertex b(CVector3D(0.5f, -0.5f, 0.5f), CVector3D(), CVector2D(1.0f, 0.0f));
		MVertex c(CVector3D(0.5f, -0.5f, -0.5f), CVector3D(), CVector2D(1.0f, 1.0f));
		MVertex d(CVector3D(-0.5f, -0.5f, -0.5f), CVector3D(), CVector2D(0.0f, 1.0f));

		add_one_vertex(a);	// 20
		add_one_vertex(b);	// 21
		add_one_vertex(c);	// 22
		add_one_vertex(d);	// 23

		add_one_face(20, 21, 22);
		add_one_face(20, 22, 23);
	}
	
}
void MeshTxtGen::gen_cone(uint depth) {
	// bottom
	{
		MVertex o(CVector3D(0.0f, 0.0f, 0.0f), CVector3D(), CVector2D(0.5f));
		add_one_vertex(o);
		float rad = 2.0f*CMath::pi / depth;
		for (uint i = 0; i < depth; ++i) {
			MVertex t(CVector3D(0.5f*std::cos(i*rad), 0.0f, 0.5f*std::sin(i*rad)));
			t.tex_coord = t.position.xz() + CVector2D(0.5f);
			add_one_vertex(t);
		}
		for (uint i = 1; i <= depth; ++i) { add_one_face(0, i%depth + 1, i); }
	}

	// side
	{
		MVertex o(CVector3D(0.0f, 1.0f, 0.0f), CVector3D(), CVector2D(0.5f));
		add_one_vertex(o);
		float rad = 2.0f*CMath::pi / depth;
		for (uint i = 0; i < depth; ++i) {
			MVertex t(CVector3D(0.5f*std::cos(i*rad), 0.0f, 0.5f*std::sin(i*rad)));
			t.tex_coord = t.position.xz() + CVector2D(0.5f);
			add_one_vertex(t);
		}
		for (uint i = 1; i <= depth; ++i) { add_one_face(depth + 1, i + depth + 1, i%depth + 1 + depth + 1); }
	}
}
void MeshTxtGen::gen_sphere(uint depth) {
	depth = CMath::clamp<uint>(depth, 1, 5);

	// first depth
	{
		--depth;
		// x y z
		{
			MVertex a(CVector3D(1.0f, 0.0f, 0.0f));
			MVertex b(CVector3D(0.0f, 0.0f, 1.0f));
			MVertex c(CVector3D(0.0f, 1.0f, 0.0f));

			a.tex_coord = CVector2D(0.0f, 0.5f);
			b.tex_coord = CVector2D(0.25f, 0.5f);
			c.tex_coord = CVector2D(0.0f, 1.0f);

			add_one_vertex(a);
			add_one_vertex(b);
			add_one_vertex(c);
			
			add_one_face(0, 1, 2);
		}
			
		// -x y z
		{
			MVertex a(CVector3D(0.0f, 0.0f, 1.0f));
			MVertex b(CVector3D(-1.0f, 0.0f, 0.0f));
			MVertex c(CVector3D(0.0f, 1.0f, 0.0f));

			a.tex_coord = CVector2D(0.25f, 0.5f);
			b.tex_coord = CVector2D(0.50f, 0.5f);
			c.tex_coord = CVector2D(0.25f, 1.0f);

			add_one_vertex(a);
			add_one_vertex(b);
			add_one_vertex(c);
			
			add_one_face(3, 4, 5);
		}

		// -x y -z
		{
			MVertex a(CVector3D(-1.0f, 0.0f, 0.0f));
			MVertex b(CVector3D(0.0f, 0.0f, -1.0f));
			MVertex c(CVector3D(0.0f, 1.0f, 0.0f));

			a.tex_coord = CVector2D(0.50f, 0.5f);
			b.tex_coord = CVector2D(0.75f, 0.5f);
			c.tex_coord = CVector2D(0.50f, 1.0f);

			add_one_vertex(a);
			add_one_vertex(b);
			add_one_vertex(c);

			add_one_face(6, 7, 8);
		}

		// x y -z
		{
			MVertex a(CVector3D(0.0f, 0.0f, -1.0f));
			MVertex b(CVector3D(1.0f, 0.0f, 0.0f));
			MVertex c(CVector3D(0.0f, 1.0f, 0.0f));

			a.tex_coord = CVector2D(0.75f, 0.5f);
			b.tex_coord = CVector2D(1.00f, 0.5f);
			c.tex_coord = CVector2D(1.00f, 1.0f);

			add_one_vertex(a);
			add_one_vertex(b);
			add_one_vertex(c);

			add_one_face(9, 10, 11);
		}

		// x -y z
		{
			MVertex a(CVector3D(1.0f, 0.0f, 0.0f));
			MVertex b(CVector3D(0.0f, -1.0f, 0.0f));
			MVertex c(CVector3D(0.0f, 0.0f, 1.0f));

			a.tex_coord = CVector2D(0.0f, 0.5f);
			b.tex_coord = CVector2D(0.0f, -1.0f);
			c.tex_coord = CVector2D(0.25f, 0.5f);

			add_one_vertex(a);
			add_one_vertex(b);
			add_one_vertex(c);

			add_one_face(12, 13, 14);
		}

		// -x -y z
		{
			MVertex a(CVector3D(0.0f, 0.0f, 1.0f));
			MVertex b(CVector3D(0.0f, -1.0f, 0.0f));
			MVertex c(CVector3D(-1.0f, 0.0f, 0.0f));

			a.tex_coord = CVector2D(0.25f, 0.5f);
			b.tex_coord = CVector2D(0.25f, -1.0f);
			c.tex_coord = CVector2D(0.50f, 0.5f);

			add_one_vertex(a);
			add_one_vertex(b);
			add_one_vertex(c);

			add_one_face(15, 16, 17);
		}

		// -x -y -z
		{
			MVertex a(CVector3D(-1.0f, 0.0f, 0.0f));
			MVertex b(CVector3D(0.0f, -1.0f, 0.0f));
			MVertex c(CVector3D(0.0f, 0.0f, -1.0f));

			a.tex_coord = CVector2D(0.50f, 0.5f);
			b.tex_coord = CVector2D(0.75f, -1.0f);
			c.tex_coord = CVector2D(0.75f, 0.5f);

			add_one_vertex(a);
			add_one_vertex(b);
			add_one_vertex(c);

			add_one_face(18, 19, 20);
		}

		// x -y -z
		{
			MVertex a(CVector3D(0.0f, 0.0f, -1.0f));
			MVertex b(CVector3D(0.0f, -1.0f, 0.0f));
			MVertex c(CVector3D(1.0f, 0.0f, 0.0f));

			a.tex_coord = CVector2D(0.75f, 0.5f);
			b.tex_coord = CVector2D(1.00f, -1.0f);
			c.tex_coord = CVector2D(1.00f, 0.5f);

			add_one_vertex(a);
			add_one_vertex(b);
			add_one_vertex(c);

			add_one_face(21, 22, 23);
		}

	}

	// loop depth
	{
		std::vector<MVertex> t_vetices;
		std::vector<uint> t_indices;
		while (depth--) {
			t_vetices.clear();
			t_indices.clear();

			for (uint i = 0; i < indices.size(); i += 3) {
				auto a = vertices[indices[i + 0]];
				auto b = vertices[indices[i + 1]];
				auto c = vertices[indices[i + 2]];

				MVertex d = MVertex((a.position + b.position).normalize());
				MVertex e = MVertex((b.position + c.position).normalize());
				MVertex f = MVertex((c.position + a.position).normalize());

				add_one_vertex(t_vetices, a);
				add_one_vertex(t_vetices, b);
				add_one_vertex(t_vetices, c);

				add_one_vertex(t_vetices, d);
				add_one_vertex(t_vetices, e);
				add_one_vertex(t_vetices, f);
				
				add_one_face(t_vetices, t_indices, i*2 + 0, i*2 + 3, i*2 + 5);
				add_one_face(t_vetices, t_indices, i*2 + 3, i*2 + 1, i*2 + 4);
				add_one_face(t_vetices, t_indices, i*2 + 5, i*2 + 4, i*2 + 2);
				add_one_face(t_vetices, t_indices, i*2 + 4, i*2 + 5, i*2 + 3);

			}

			vertices.clear(); indices.clear();
			vertices.assign(t_vetices.begin(), t_vetices.end());
			indices.assign(t_indices.begin(), t_indices.end());
		}

		for (auto& i : vertices) { i.normal = i.position.normalize(); }
	}
}
void MeshTxtGen::gen_cylinder(uint depth) {
	// top
	{
		MVertex o(CVector3D(0.0f, 1.0f, 0.0f), CVector3D(), CVector2D(0.5f));
		add_one_vertex(o);
		float rad = 2.0f*CMath::pi / depth;
		for (uint i = 0; i < depth; ++i) {
			MVertex t(CVector3D(0.5f*std::cos(i*rad), 1.0f, 0.5f*std::sin(i*rad)));
			t.tex_coord = t.position.xz() + CVector2D(0.5f);
			add_one_vertex(t);
		}
		for (uint i = 1; i <= depth; ++i) { add_one_face(0, i, i%depth + 1); }
	}
	
	uint cnt = depth + 1;
	// bottom
	{
		MVertex o(CVector3D(0.0f, 0.0f, 0.0f), CVector3D(), CVector2D(0.5f));
		add_one_vertex(o);
		float rad = 2.0f*CMath::pi / depth;
		for (uint i = 0; i < depth; ++i) {
			MVertex t(CVector3D(0.5f*std::cos(i*rad), 0.0f, 0.5f*std::sin(i*rad)));
			t.tex_coord = t.position.xz() + CVector2D(0.5f);
			add_one_vertex(t);
		}
		for (uint i = 1; i <= depth; ++i) { add_one_face(cnt, cnt + i%depth + 1, cnt + i); }
	}

	cnt += depth + 1;
	// side
	{
		float rad = 2.0f*CMath::pi / depth;
		for (uint i = 0; i <= depth; ++i) {
			MVertex t(CVector3D(0.5f*std::cos(i*rad), 0.0f, 0.5f*std::sin(i*rad)));
			t.tex_coord = CVector2D((float)i / depth, 0.0f);
			add_one_vertex(t);
			MVertex tt(CVector3D(0.5f*std::cos(i*rad), 1.0f, 0.5f*std::sin(i*rad)));
			t.tex_coord = CVector2D((float)i / depth, 1.0f);
			add_one_vertex(tt);
		}

		for (uint i = 0; i < depth; ++i) {
			add_one_face(cnt + i*2, cnt + i*2 + 2, cnt + i*2 + 3);
			add_one_face(cnt + i*2, cnt + i*2 + 3, cnt + i*2 + 1);
		}
	}
}

void MeshTxtGen::write(std::ostream& out) {
	for (auto i : vertices) { write_one_vertex(out, i); }
	out << std::endl << std::endl;

	for (uint i = 0; i < indices.size(); i += 3) {
		write_one_face(out, indices[i+0], indices[i+1], indices[i+2]);
	}
}

void MeshTxtGen::write_one_vertex(std::ostream& out, const MVertex& vertex) {
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
void MeshTxtGen::write_one_face(std::ostream& out, uint a, uint b, uint c) {
	out << "f ";
	out << a << ", "; out << b << ", "; out << c << ", "; 
	out << std::endl;
}

void MeshTxtGen::add_one_vertex(const MVertex& x) { vertices.push_back(x); }
void MeshTxtGen::add_one_vertex(std::vector<MVertex>& v, const MVertex& x) { v.push_back(x); }
void MeshTxtGen::add_one_face(uint a, uint b, uint c) {
	// 利用混合积保持三角面片的一致性顺序 -- 逆时针
	float det = vertices[a].position.mix(vertices[b].position, vertices[c].position);
	if (det > 1e-3) { std::swap(b, c); }		

	indices.push_back(a); indices.push_back(b); indices.push_back(c);
}
void MeshTxtGen::add_one_face(const std::vector<MVertex>& v, std::vector<uint>& idx, uint a, uint b, uint c) {
	// 利用混合积保持三角面片的一致性顺序 -- 逆时针
	float det = v[a].position.mix(v[b].position, v[c].position);
	if (det > 1e-3) { std::swap(b, c); }		

	idx.push_back(a); idx.push_back(b); idx.push_back(c);
}

void MeshTxtGen::cac_normal() {
	for (auto& i : vertices) { i.normal = CVector3D(); }

	for (uint i = 0; i < indices.size(); i += 3) {
		auto& a = vertices[indices[i+0]];
		auto& b = vertices[indices[i+1]];
		auto& c = vertices[indices[i+2]];
		
		// cac face normal
		CVector3D t_normal = (c.position - a.position).cross(b.position - a.position);	// 右手定则

		// accumulate normal
		a.normal += t_normal;
		b.normal += t_normal;
		c.normal += t_normal;
	}

	// normalize normal
	for (auto& i : vertices) { i.normal.normalize(); }
}

// ===============================================================================================

bool MeshLoader::load_mesh_txt(const std::string& src, std::vector<MVertex>& vertices, std::vector<uint>& indices, SourceType source_type) {

	// 打开文件
	std::istream* in;
	std::ifstream fs;
	std::istringstream ss;
	if (source_type == SourceType::BY_FILE) {
		fs.open(src, std::ios::in);
		if (!fs.is_open()) { return false; }
		in = &fs;
	}
	else if (source_type == SourceType::BY_STRING) { ss.clear(); ss.str(src); in = &ss; }

	vertices.clear();
	indices.clear();

	// 按行读取 -- 获得 vertices 和 indices
	while ((*in)) {
		std::string t_line;
		std::getline((*in), t_line);
		std::replace(t_line.begin(), t_line.end(), ',', ' ');
		std::istringstream t_iss(t_line);

		std::string head; t_iss >> head;

		if (head.compare("v") == 0) {
			std::vector<float> list;
			float t_f;
			while (t_iss >> t_f) list.push_back(t_f);

			MVertex v;
			if (list.size() >= 3) v.position = CVector3D(list[0], list[1], list[2]);
			if (list.size() >= 6) v.normal = CVector3D(list[3], list[4], list[5]);
			if (list.size() >= 8) v.tex_coord = CVector2D(list[6], list[7]);
			vertices.push_back(v);
		}
		else if (head.compare("f") == 0) { uint t_u; while (t_iss >> t_u) indices.push_back(t_u); }

	}

	if (source_type == SourceType::BY_FILE) { fs.close(); }
	
	return true;
}

bool MeshLoader::load_mesh_obj(const std::string& src, std::vector<MVertex>& vertices, std::vector<uint>& indices, SourceType source_type) {
	return true;
}
