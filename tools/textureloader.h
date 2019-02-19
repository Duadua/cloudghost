#pragma once

#include "loader.h"

class TextureGen {

};

// ===============================================================================================

class TextureLoader {
public:

	static bool load_texture_png(const std::string& path, SPTR_uchar& data, uint& data_size);

	~TextureLoader() {}
private:
	TextureLoader() {}

};
