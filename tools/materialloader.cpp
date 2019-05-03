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
bool MaterialGen::gen_material_txt_pbr(std::string& res, const MaterialData& md, SourceType source_type) {
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

	write_one_material_pbr((*out), md);

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

	// pbr
	out << "\talbedo " << md.albedo.x() << " " << md.albedo.y() << " " << md.albedo.z() << "\n";
	out << "\tmetallic " << md.metallic << "\n";
	out << "\troughness " << md.roughness << "\n";
	out << "\tao " << md.ao << "\n";
	out << "\n";

	if (md.map_albedo.compare("") != 0) out << "\tmap_albedo " << md.map_albedo << "\n";
	if (md.map_metallic.compare("") != 0) out << "\tmap_metallic " << md.map_metallic << "\n";
	if (md.map_roughness.compare("") != 0) out << "\tmap_roughness " << md.map_roughness << "\n";
	if (md.map_ao.compare("") != 0) out << "\tmap_ao " << md.map_ao << "\n";
	out << "\n";

}
void MaterialGen::write_one_material_pbr(std::ostream& out, const MaterialData& md) {
	out << "name " << md.name << "\n";

	out << "\talbedo " << md.albedo.x() << " " << md.albedo.y() << " " << md.albedo.z() << "\n";
	out << "\tmetallic " << md.metallic << "\n";
	out << "\troughness " << md.roughness << "\n";
	out << "\tao " << md.ao << "\n";
	out << "\n";

	if (md.map_albedo.compare("") != 0) out << "\tmap_albedo " << md.map_albedo << "\n";
	if (md.map_metallic.compare("") != 0) out << "\tmap_metallic " << md.map_metallic << "\n";
	if (md.map_roughness.compare("") != 0) out << "\tmap_roughness " << md.map_roughness << "\n";
	if (md.map_ao.compare("") != 0) out << "\tmap_ao " << md.map_ao << "\n";
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
			if (t_f < 16.0f) t_f = 16.0f;
			if (t_f > 128.0f) t_f = 128.0f;
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
		else if (head.compare("map_normal") == 0) {
			std::string t_str; t_iss >> t_str;
			md[md.size() - 1].map_normal = t_str;
		}
		// pbr
		else if (head.compare("albedo") == 0) {
			std::vector<float> list; float t_f;
			while (t_iss >> t_f) list.push_back(t_f);

			if(list.size() >= 3) md[md.size() - 1].albedo = CVector3D(list[0], list[1], list[2]);
		}
		else if (head.compare("metallic") == 0) {
			float t_value; t_iss >> t_value;
			md.back().metallic = t_value;
		}
		else if (head.compare("roughness") == 0) {
			float t_value; t_iss >> t_value;
			md.back().roughness = t_value;
		}
		else if (head.compare("ao") == 0) {
			float t_value; t_iss >> t_value;
			md.back().ao = t_value;
		}
		else if (head.compare("map_albedo") == 0) {
			std::string t_str; t_iss >> t_str;
			md.back().map_albedo = t_str;
		}
		else if (head.compare("map_metallic") == 0) {
			std::string t_str; t_iss >> t_str;
			md.back().map_metallic = t_str;
		}
		else if (head.compare("map_roughness") == 0) {
			std::string t_str; t_iss >> t_str;
			md.back().map_roughness = t_str;
		}
		else if (head.compare("map_ao") == 0) {
			std::string t_str; t_iss >> t_str;
			md.back().map_ao = t_str;
		}

	}

	return true;
}

bool MaterialLoader::load_material_mtl(const std::string& src, std::vector<MaterialData>& md, SourceType source_type) {
    if(src.compare("") == 0 && md.size() == 0 && source_type == SourceType::BY_FILE) {}
	return true;
}


