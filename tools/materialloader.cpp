#include "materialloader.h"

bool MaterialGen::gen_material_txt(std::string& res, const MaterialData& md, SourceType source_type) {
	std::ostream* out;
	std::ofstream fs;
	std::stringstream ss;
	if (source_type == SourceType::BY_FILE) {
		fs.open("resources/materials/txt/" + res, std::ios::app | std::ios::out);
		if (!fs.is_open()) { return false; }
		out = &fs;
	}
	else if (source_type == SourceType::BY_STRING) { ss.clear(); out = &ss; }
    else { return false; }

	out->fill('0');
	out->precision(6);
	out->setf(std::ios::fixed, std::ios::floatfield);

	write_one_material((*out), md);

	if (source_type == SourceType::BY_FILE)		{ fs.close(); }
	else if (source_type == SourceType::BY_STRING) { res = ss.str(); ss.clear(); }

	return true;
}

void MaterialGen::write_one_material(std::ostream& out, const MaterialData& md) {
	out << "name " << md.name << "\n";

	out << "\tka " << md.ka.x() << " " << md.ka.y() << " " << md.ka.z() << "\n";
	out << "\tkd " << md.kd.x() << " " << md.kd.y() << " " << md.kd.z() << "\n";
	out << "\tks " << md.ks.x() << " " << md.ks.y() << " " << md.ks.z() << "\n";
	out << "\n";

	out << "\tshininess " << md.shininess << "\n";
	out << "\n";

	if (md.map_ka.compare("") != 0) out << "\tmap_ka " << md.map_ka << "\n";
	if (md.map_kd.compare("") != 0) out << "\tmap_kd " << md.map_kd << "\n";
	if (md.map_ks.compare("") != 0) out << "\tmap_ks " << md.map_ks << "\n";
	out << "\n";
}

// ===============================================================================================

bool MaterialLoader::load_material_txt(const std::string& src, std::vector<MaterialData>& md, SourceType source_type) {

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
    else { return false; }

	MaterialData t_md;

	// read every line
	while ((*in)) {
		std::string t_line;
		std::getline((*in), t_line);
		std::replace(t_line.begin(), t_line.end(), ',', ' ');
		std::istringstream t_iss(t_line);

		std::string head; t_iss >> head;

		if (head.compare("name") == 0) {
			std::string t_name; t_iss >> t_name;
			md.push_back(t_md);
			md[md.size() - 1].name = t_name;
			
		}
		else if (head.compare("ka") == 0) {
			std::vector<float> list; float t_f;
			while (t_iss >> t_f) list.push_back(t_f);

			if(list.size() >= 3) md[md.size() - 1].ka = CVector3D(list[0], list[1], list[2]);
		}
		else if (head.compare("kd") == 0) {
			std::vector<float> list; float t_f;
			while (t_iss >> t_f) list.push_back(t_f);

			if(list.size() >= 3) md[md.size() - 1].kd = CVector3D(list[0], list[1], list[2]);
		}
		else if (head.compare("ks") == 0) {
			std::vector<float> list; float t_f;
			while (t_iss >> t_f) list.push_back(t_f);

			if(list.size() >= 3) md[md.size() - 1].ks = CVector3D(list[0], list[1], list[2]);

		}
		else if (head.compare("shininess") == 0) {
			float t_f; t_iss >> t_f;
			md[md.size() - 1].shininess = t_f;
		}
		else if (head.compare("map_ka") == 0) {
			std::string t_str; t_iss >> t_str;
			md[md.size() - 1].map_ka = t_str;
		}
		else if (head.compare("map_kd") == 0) {
			std::string t_str; t_iss >> t_str;
			md[md.size() - 1].map_kd = t_str;
		}
		else if (head.compare("map_ks") == 0) {
			std::string t_str; t_iss >> t_str;
			md[md.size() - 1].map_ks = t_str;
		}
	}

	return true;
}

bool MaterialLoader::load_material_mtl(const std::string& src, std::vector<MaterialData>& md, SourceType source_type) {
    if(src.compare("") == 0 && md.size() == 0 && source_type == SourceType::BY_FILE) {}
	return true;
}


