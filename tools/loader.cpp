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
