#include "textureloader.h"
#include "stb_head.h"

bool TextureGen::gen_texture_txt(const std::string& res, TextureGenType type, CColor color, uint depth, SourceType source_type) {
	depth = CMath_ins().clamp(depth, 1u, 10u);
	uint width = CMath_ins().pow(2u, depth);
	uint heigh = CMath_ins().pow(2u, depth);

	// 打开文件
	std::ostream* out;
	std::ofstream fs;
	std::stringstream ss;
	if (source_type == SourceType::BY_FILE) {
		fs.open("resources/textures/txt/" + res, std::ios::binary | std::ios::trunc | std::ios::out);
		if (!fs.is_open()) { return false; }
		out = &fs;
	}
	else if (source_type == SourceType::BY_STRING) { ss.clear(); out = &ss; }
    else { return false; }

	// 生成数据
	SPTR_uchar t_data;
	uint t_size = 0;
	switch (type) {
	case TextureGenType::SOLIDE: t_data = gen_solide(t_size, width, heigh, color); break;
	case TextureGenType::GRADUA: t_data = gen_gradua(t_size, width, heigh, color); break;
	case TextureGenType::THE_TWO: t_data = gen_thetwo(t_size, width, heigh, color); break;
	}

    if(t_data != nullptr) out->write(reinterpret_cast<char*>(t_data.get()), t_size);

	return true;
}

SPTR_uchar TextureGen::gen_solide(uint& data_size, uint width, uint heigh, CColor color) {
	data_size = width * 4 * heigh + sizeof(uint)*2;
	auto t_data = StringHelper_ins().make_shared_array<uchar>(data_size + 1);

	memcpy(t_data.get(), &width, sizeof(uint));
	memcpy(t_data.get() + sizeof(uint), &heigh, sizeof(uint));
	
	for (uint i = 0; i < heigh; ++i) {
		for (uint j = 0; j < width; ++j) {
			uint t_c = color.get_rgba();
			memcpy(t_data.get() + sizeof(uint) * 2 + i * width * 4 + j * 4, &t_c, sizeof(uint));
		}
	}
	return t_data;
}
SPTR_uchar TextureGen::gen_gradua(uint& data_size, uint width, uint heigh, CColor color) {
	data_size = width * 4 * heigh + sizeof(uint) * 2;
	auto t_data = StringHelper_ins().make_shared_array<uchar>(data_size + 1);

	memcpy(t_data.get(), &width, sizeof(uint));
	memcpy(t_data.get() + sizeof(uint), &heigh, sizeof(uint));

	for (uint i = 0; i < heigh; ++i) {
		for (uint j = 0; j < width; ++j) {
            float t_v = 1.0f * j / width;
            uint t_r = static_cast<uint>(CMath_ins().linear_lerp(1.0f*color.r(), 255.0f, t_v));
            uint t_g = static_cast<uint>(CMath_ins().linear_lerp(1.0f*color.g(), 255.0f, t_v));
            uint t_b = static_cast<uint>(CMath_ins().linear_lerp(1.0f*color.b(), 255.0f, t_v));
			//uint t_a = CMath_ins().linear_lerp(color.a(), 0.0f, t_v);
			CColor t_color(t_r, t_g, t_b);
			uint t_c = t_color.get_rgba();
			memcpy(t_data.get() + sizeof(uint) * 2 + i * width * 4 + j * 4, &t_c, sizeof(uint));
		}
	}
	return t_data;
}
SPTR_uchar TextureGen::gen_thetwo(uint& data_size, uint width, uint heigh, CColor color) {
	data_size = width * 4 * heigh + sizeof(uint) * 2;
	auto t_data = StringHelper_ins().make_shared_array<uchar>(data_size + 1);

	memcpy(t_data.get(), &width, sizeof(uint));
	memcpy(t_data.get() + sizeof(uint), &heigh, sizeof(uint));

	for (uint i = 0; i < heigh; ++i) {
		for (uint j = 0; j < width; ++j) {
			uint t_i = i / 32;
			uint t_j = j / 32;
			CColor t_color;
			if ((t_i + t_j) & 1) { t_color = color; }
			else { t_color = CColor(255 - color.r(), 255 - color.g(), 255 - color.b()); }
			uint t_c = t_color.get_rgba();
			memcpy(t_data.get() + sizeof(uint) * 2 + i * width * 4 + j * 4, &t_c, sizeof(uint));
		}
	}
	return t_data;

}

// ===============================================================================================

SPTR_uchar TextureLoader::load_texture_txt(const std::string& path, uint& width, uint& heigh, SourceType source_type) {
	std::istream* in;
	std::ifstream fs;
	std::istringstream ss;
	if (source_type == SourceType::BY_FILE) {
		fs.open(path, std::ios::binary | std::ios::in);
		if (!fs.is_open()) { return nullptr; }
		in = &fs;
	}
	else if (source_type == SourceType::BY_STRING) { ss.clear(); ss.str(path); in = &ss; }
    else { return nullptr; }

	uint t_size;
	in->seekg(0, std::ios::end);										// 跳到文件尾
    t_size = static_cast<uint>(in->tellg());												// 以获得内容大小
	auto t_data = StringHelper_ins().make_shared_array<uchar>(t_size + 1);					// 以开辟相应容量的存储空间

	in->seekg(0, std::ios::beg);										// 跳到文件头
    in->read(reinterpret_cast<char*>(t_data.get()), t_size);
	
	memcpy(&width, t_data.get(), sizeof(uint));
	memcpy(&heigh, t_data.get()+sizeof(uint), sizeof(uint));
	uint data_size = width * 4 * heigh;

	auto t_res = StringHelper_ins().make_shared_array<uchar>(data_size + 1);
	memcpy(t_res.get(), t_data.get() + sizeof(uint) * 2, data_size);
	return t_res;
}
SPTR_uchar TextureLoader::load_texture_png(const std::string& path, uint& data_size) {
	std::ifstream fs;
	fs.open(path, std::ios::binary);
	if (!fs.is_open()) { return nullptr; }
	
	fs.seekg(0, std::ios::end);										// 跳到文件尾
    data_size = static_cast<uint>(fs.tellg());											// 以获得内容大小
	auto t_data = StringHelper_ins().make_shared_array<uchar>(data_size + 1);			// 以开辟相应容量的存储空间

	fs.seekg(0, std::ios::beg);										// 跳到文件头
    fs.read(reinterpret_cast<char*>(t_data.get()), data_size);
	
	return t_data;
}
SPTR_uchar TextureLoader::load_texture_x(const std::string& path, int& width, int& heigh, int& channel) {
	//stbi_set_flip_vertically_on_load(true);
	auto data = stbi_load(path.c_str(), &width, &heigh, &channel, 0);
	auto data_size = width * heigh * channel;
	if (data == nullptr || data_size == 0) { return nullptr; }
	auto t_data = StringHelper_ins().make_shared_array<uchar>(data_size + 1);
	memcpy(t_data.get(), data, data_size);
	stbi_image_free(data);
	return t_data;
}
SPTR_float TextureLoader::load_texture_x_hdr(const std::string& path, int& width, int& heigh, int& channel) {
	//stbi_set_flip_vertically_on_load(true);
	auto data = stbi_loadf(path.c_str(), &width, &heigh, &channel, 0);
	auto data_size = width * heigh * channel * sizeof(float);
	if (data == nullptr || data_size == 0) { return nullptr; }
	auto t_data = StringHelper_ins().make_shared_array<float>(data_size + 1);
	memcpy(t_data.get(), data, data_size);
	stbi_image_free(data);
	return t_data;
}
SPTR_uchar TextureLoader::load_texture_dds(const std::string& path, int& width, int& heigh, int& channel) {
	//stbi_set_flip_vertically_on_load(true);
	auto data = stbi_dds_load(path.c_str(), &width, &heigh, &channel, 0);
	auto data_size = width * heigh * channel;
	if (data == nullptr || data_size == 0) { return nullptr; }
	auto t_data = StringHelper_ins().make_shared_array<uchar>(data_size + 1);
	memcpy(t_data.get(), data, data_size);
	stbi_image_free(data);
	return t_data;
}


