#include "textureloader.h"

bool TextureGen::gen_texture_txt(const std::string& res, TextureGenType type, CVector3D color, SourceType source_type) {
	return true;
}

// ===============================================================================================

SPTR_uchar TextureLoader::load_texture_txt(const std::string& path, uint& data_size, SourceType source_type) {
	return nullptr;
}

SPTR_uchar TextureLoader::load_texture_png(const std::string& path, uint& data_size) {
	std::ifstream fs;
	fs.open(path, std::ios::binary);
	if (!fs.is_open()) { return nullptr; }
	
	fs.seekg(0, std::ios::end);										// 跳到文件尾
	data_size = fs.tellg();											// 以获得内容大小
	auto t_data = make_shared_array<uchar>(data_size + 1);			// 以开辟相应容量的存储空间

	fs.seekg(0, std::ios::beg);			// 跳到文件头
	fs.read((char*)t_data.get(), data_size);
	
	return t_data;
}
