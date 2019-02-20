#include "qtimagehelper.h"

bool QtImageHelper::repair_png(std::string& path) {
	std::vector<std::string> t_files;

	get_all_files_from_dir(path, t_files);

	bool res = true;
	for (auto i : t_files) { if(!repair_one_png(i)) res = false; }
	return res;
}


bool QtImageHelper::txt_to_png(std::string& path, SPTR_uchar data) {

	return true;
}

bool QtImageHelper::repair_one_png(std::string& path) {

	return true;
}

void QtImageHelper::get_all_files_from_dir(std::string& path, std::vector<std::string>& res) {

}
