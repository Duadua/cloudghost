#pragma once

#include "cmath.h"
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#ifndef NDEBUG
#define C_DEBUG
#endif // !NDEBUG -- 没有定义 no_debug -- 定义 c_debug

#ifndef C_DEBUG
#undef C_DEBUG_QT
#endif // !C_DEBUG -- 如果没有定义 c_debug 则 取消定义 c_debug_qt

// custom set
#define C_DEBUG_QT			// 是否使用 c_debug_qt 窗口

#ifdef C_DEBUG_QT
#define C_DEBUG
#endif // C_DEBUG_QT -- 如果定义了 C_DEBUG_QT -- 则也需要定义 C_DEBUG

enum SourceType {
	BY_FILE,
	BY_STRING
};

// array 使用 shared_ptr -- 需要手动指定删除器
template<class T>
inline std::shared_ptr<T> make_shared_array(size_t size) { return std::shared_ptr<T>(new T[size], std::default_delete<T[]>()); }

// files helper
std::string get_suff_of_file(const std::string& path);
std::string get_name_of_file(const std::string& path);
void get_all_files_from_dir(std::string& path, std::vector<std::string>& res);

std::string load_txt(const std::string& path);
bool		save_txt(const std::string& path, const std::string& source);

// ===========================================================================

class CDebug {
public:
	static CDebug* get_instance();
	~CDebug();

	void log(std::string str);

	CDebug& operator << (const std::string& str);

	std::string get_data() const { return data; }

#ifdef C_DEBUG_QT
	class QtDebugWidget* c_debug_qt;
#endif // C_DEBUG_QT

private:
	std::string data;
	std::ostringstream ss;
	bool save(const std::string& path);

	CDebug();
	static CDebug* instance;
};

CDebug& c_debug();

