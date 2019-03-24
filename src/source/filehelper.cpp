#include "filehelper.h"
#include "typehelper.h"
#include <io.h>
#include <sstream>
#include <fstream>
#include <algorithm>

std::string FileHelper::get_suff_of_file(const std::string& path) {
    uint t_idx = static_cast<uint>(path.find_last_of('.'));
	if (t_idx >= path.size()) { return "nope"; }
	return path.substr(t_idx);
}
std::string FileHelper::get_path_of_file(const std::string& path) {
	std::string t_s; t_s.assign(path);
	std::replace(t_s.begin(), t_s.end(), '\\', '/');
    uint t_f = static_cast<uint>(t_s.find_last_of('/'));
	if (t_f >= t_s.size()) { return ""; }
	return t_s.substr(0, t_f);

}
std::string FileHelper::get_name_of_file(const std::string& path) {
	std::string t_s; t_s.assign(path);
	std::replace(t_s.begin(), t_s.end(), '\\', '/');
    uint t_f = static_cast<uint>(t_s.find_last_of('/'));
	if (t_f >= t_s.size()) { return t_s; }
	return t_s.substr(t_f + 1);
}
void FileHelper::get_all_files_from_dir(std::string& path, std::vector<std::string>& res) {
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

std::string FileHelper::load_txt(const std::string& path) {
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
bool FileHelper::save_txt(const std::string& path, const std::string& source) {
	std::ofstream fs;
	fs.open(path, std::ios::trunc | std::ios::out);
	if (!fs.is_open()) { return false; }

    fs.write(source.c_str(), static_cast<ll>(source.length()));
	fs.close();
	return true;
}

