#include "qtimagehelper.h"
#include "loader.h"
#include <io.h>
#include <fstream>
#include <QImage>
#include <QPainter>

bool QtImageHelper::data_to_png(const std::string& path, const SPTR_uchar data, const uint width, const uint heigh) {
    QImage img(static_cast<int>(width), static_cast<int>(heigh), QImage::Format_ARGB32);

    for (uint i = 0; i < heigh; ++i) {
        for (uint j = 0; j < width; ++j) {
			uint t_c;
			memcpy(&t_c, data.get() + i * width * 4 + j * 4, sizeof(uint));
            img.setPixel(static_cast<int>(j), static_cast<int>(i), t_c);
		}
	}
	if (img.save(QString::fromStdString(path), "png")) {
		c_debug() << "save png : " + path;
	}
	
	return true;
}
bool QtImageHelper::text_to_png(const std::string& path) {
	std::ifstream fs;
	fs.open(path, std::ios::binary | std::ios::in);
	if (!fs.is_open()) { return false; }

	uint t_size;
	fs.seekg(0, std::ios::end);										
    t_size = static_cast<uint>(fs.tellg());								// 以获得内容大小
	auto t_data = make_shared_array<uchar>(t_size + 1);					// 以开辟相应容量的存储空间

	fs.seekg(0, std::ios::beg);											// 跳到文件头
    fs.read(reinterpret_cast<char*>(t_data.get()), t_size);

	uint width, heigh, data_size;
	memcpy(&width, t_data.get(), sizeof(uint));
	memcpy(&heigh, t_data.get() + sizeof(uint), sizeof(uint));
	data_size = width * 4 * heigh;

	auto t_res = make_shared_array<uchar>(data_size + 1);
	memcpy(t_res.get(), t_data.get() + sizeof(uint) * 2, data_size);

	auto t_pref = get_pref_of_file(path);

	return data_to_png(t_pref.append(".png"), t_res, width, heigh);
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
	c_debug() << "repair : " << path;

	std::ifstream fs;
	fs.open(path, std::ios::binary);
	if (!fs.is_open()) { return false; }

	fs.seekg(0, std::ios::end);								
    uint t_size = static_cast<uint>(fs.tellg());
	auto t_data = make_shared_array<uchar>(t_size + 1);

	fs.seekg(0, std::ios::beg);			// 跳到文件头
    fs.read(reinterpret_cast<char*>(t_data.get()), t_size);

    QByteArray t_ba(reinterpret_cast<const char*>(t_data.get()), static_cast<int>(t_size));
	QImage img;
	img.loadFromData(t_ba, "png");
	img.save(QString::fromStdString(path), "png");

	return true;
}

std::string QtImageHelper::get_suff_of_file(const std::string& path) {
    auto t_idx = path.find_last_of('.');
	return path.substr(t_idx);
}
std::string QtImageHelper::get_pref_of_file(const std::string& path) {
    auto t_idx = path.find_last_of('.');
	return path.substr(0, t_idx);
}
std::string QtImageHelper::get_name_of_file(const std::string& path) {
    auto t_f = path.find_last_of('/');
	return path.substr(t_f + 1);
}
void QtImageHelper::get_all_files_from_dir(const std::string& path, std::vector<std::string>& res) {
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
