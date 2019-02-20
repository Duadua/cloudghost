#pragma once

// 从 data 数组 生成 png 及其他格式图片
// 修复 libpng 的 png warning

#include <string>
#include <vector>
#include <memory>

using uchar = unsigned char;
using SPTR_uchar = std::shared_ptr<uchar>;

class QtImageHelper {	
public:

	static bool repair_png(std::string& path);

	static bool txt_to_png(std::string& path, SPTR_uchar data);

	
	~QtImageHelper() {}
private:
	QtImageHelper() {}

	static void get_all_files_from_dir(std::string& path, std::vector<std::string>& res);

	static bool repair_one_png(std::string& path);

};