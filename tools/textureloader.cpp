#include "textureloader.h"

// ===============================================================================================


bool TextureLoader::load_texture_png(const std::string& path, SPTR_uchar& data, uint& data_size) {
	std::ifstream fs;
	fs.open(path, std::ios::binary);
	if (!fs.is_open()) { return false; }
	
	fs.seekg(0, std::ios::end);										// 跳到文件尾
	data_size = fs.tellg();											// 以获得内容大小
	auto t_data = make_shared_array<uchar>(data_size + 1);			// 以开辟相应容量的存储空间
	data = t_data;

	fs.seekg(0, std::ios::beg);			// 跳到文件头
	fs.read((char*)data.get(), data_size);
	
	return true;
}
