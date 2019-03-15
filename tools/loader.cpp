#include "loader.h"
#include <io.h>

std::string get_suff_of_file(const std::string& path) {
	uint t_idx = path.find_last_of('.');
	return path.substr(t_idx);
}
std::string get_name_of_file(const std::string& path) {
	uint t_f = path.find_last_of('/');
	return path.substr(t_f + 1);
}
void get_all_files_from_dir(std::string& path, std::vector<std::string>& res) {
	intptr_t h_file;
	_finddata_t fd;
	std::string t_str;
	if ((h_file = _findfirst(t_str.assign(path).append("/*").c_str(), &fd)) == -1) { return; }

	do {
		t_str = path + "/" + fd.name;
		if ((fd.attrib & _A_SUBDIR)) {
			if (strcmp(fd.name, ".") == 0 || strcmp(fd.name, "..") == 0) continue;
			get_all_files_from_dir(t_str, res);
		} // 子文件夹
		else { res.push_back(t_str); } // 文件

	} while (_findnext(h_file, &fd) == 0);

	_findclose(h_file);
}

std::string load_txt(const std::string& path) {
	std::string res;
	std::ifstream ifs;
	ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		ifs.open(path);
		std::stringstream ss;
		ss << ifs.rdbuf();
		res = ss.str();
		ifs.close();

	}
	catch (std::ifstream::failure e) { res = ""; }
	return res;
}
bool save_txt(const std::string& path, const std::string& source) {
	std::ofstream fs;
	fs.open(path, std::ios::trunc | std::ios::out);
	if (!fs.is_open()) { return false; }

	fs.write(source.c_str(), source.length());
	fs.close();
	return true;
}

// ========================================

CDebug* CDebug::instance = nullptr;
CDebug* CDebug::get_instance() {
	if (instance == nullptr) { instance = new CDebug(); }
	return instance;
}
CDebug::CDebug() { data = ""; ss.clear(); } 

void CDebug::log(std::string str) {
#ifdef C_DEBUG
	data += "# =========================\n\t" + str + "\n";
	save("resources/logs/log.txt");
#endif // C_DEBUG
}

CDebug& CDebug::operator << (const std::string& str) {
#ifdef C_DEBUG
	ss << "# =========================\n\t" << str << "\n";
	data = ss.str();
	save("resources/logs/log.txt");
#endif // C_DEBUG
	return (*this);
}
bool CDebug::save(const std::string& path) { return save_txt(path, data); }

CDebug& c_debug() { return (*CDebug::get_instance()); }
