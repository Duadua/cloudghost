#pragma once

// 从 data 数组 生成 png 及其他格式图片
// 修复 libpng 的 png warning
// 依赖 <QImage>

#include <string>
#include <vector>
#include <memory>

using uint  = unsigned int;
using uchar = unsigned char;
using SPTR_uchar = std::shared_ptr<uchar>;

class QtImageHelper {	
public:

	static bool data_to_png(const std::string& path, const SPTR_uchar data, const uint width, const uint heigh);
	static bool text_to_png(const std::string& path);

	static bool repair_png(std::string& path);
	
	~QtImageHelper() {}
private:
	QtImageHelper() {}

	static bool repair_one_png(std::string& path);

	// file opr
	static std::string get_suff_of_file(const std::string& path);
	static std::string get_pref_of_file(const std::string& path);	// 获得去掉后缀后的前缀
	static std::string get_name_of_file(const std::string& path);
	static void get_all_files_from_dir(const std::string& path, std::vector<std::string>& res);


	// array 使用 shared_ptr -- 需要手动指定删除器
	template<class T>
	static inline std::shared_ptr<T> make_shared_array(size_t size) { return std::shared_ptr<T>(new T[size], std::default_delete<T[]>()); }

};