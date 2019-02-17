#include "materialloader.h"
#include <algorithm>




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
	}

	return true;
}

bool MaterialLoader::load_material_mtl(const std::string& src, std::vector<MaterialData>& md, SourceType source_type) {
	return true;
}


