#pragma once

#include <string>
#include <sstream>

#ifndef NDEBUG
#define C_DEBUG
#define C_DEBUG_QT
#endif // !NDEBUG -- 没有定义 no_debug -- 定义 c_debug

#ifndef C_DEBUG
#undef C_DEBUG_QT
#endif // !C_DEBUG -- 如果没有定义 c_debug 则 取消定义 c_debug_qt

// custom set
//#define C_DEBUG_QT			// 是否使用 c_debug_qt 窗口

#ifdef C_DEBUG_QT
#define C_DEBUG
#endif // C_DEBUG_QT -- 如果定义了 C_DEBUG_QT -- 则也需要定义 C_DEBUG

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

