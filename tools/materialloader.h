#pragma once

#include "loader.h"

struct MaterialData {

};

class MaterialGen {

};

class MaterialLoader {
public:

	static bool load_material_txt(const std::string& src, std::vector<MaterialData>& md, SourceType source_type = SourceType::BY_FILE);
	static bool load_material_mtl(const std::string& src, std::vector<MaterialData>& md, SourceType source_type = SourceType::BY_FILE);

	~MaterialLoader() {}

private:
	MaterialLoader() {}

};
