#include "qtimagehelper.h"
#include <io.h>
#include <fstream>
#include <QDebug>
#include <QImage>

bool QtImageHelper::txt_to_png(std::string& path, SPTR_uchar data) {
	return true;
}

bool QtImageHelper::repair_png(std::string& path) {
	std::vector<std::string> t_files;

	get_all_files_from_dir(path, t_files);

	bool res = true;
	for (auto i : t_files) { 
		if (get_suff_of_file(i).compare(".png") != 0) continue;
		if(!repair_one_png(i)) res = false; 
	}
	return res;
}

// ===========================================================================

bool QtImageHelper::repair_one_png(std::string& path) {
	qDebug() << "repair : " << QString::fromStdString(path);

	std::ifstream fs;
	fs.open(path, std::ios::binary);
	if (!fs.is_open()) { return false; }

	fs.seekg(0, std::ios::end);								
	uint t_size = fs.tellg();									
	auto t_data = make_shared_array<uchar>(t_size + 1);

	fs.seekg(0, std::ios::beg);			// 跳到文件头
	fs.read((char*)t_data.get(), t_size);

	QByteArray t_ba((char*)t_data.get(), t_size);
	QImage img;
	img.loadFromData(t_ba, "png");
	img.save(QString::fromStdString(path));

	return true;
}

std::string QtImageHelper::get_suff_of_file(std::string& path) {
	int t_idx = path.find_last_of('.');
	return path.substr(t_idx);
}
std::string QtImageHelper::get_name_of_file(const std::string& path) {
	int t_f = path.find_last_of('/');
	return path.substr(t_f + 1);
}
void QtImageHelper::get_all_files_from_dir(std::string& path, std::vector<std::string>& res) {
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
