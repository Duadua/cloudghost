#pragma once

#include "singleton.h"
#include <string>
#include <sstream>

#ifndef NDEBUG
#define C_DEBUG
#define C_DEBUG_QT
#endif // !NDEBUG -- 没有定义 no_debug -- 定义 c_debug

#ifndef C_DEBUG
#undef C_DEBUG_QT
#endif // !C_DEBUG -- 如果没有定义 c_debug 则 取消定义 c_debuger_qt

// custom set
//#define C_DEBUG_QT			// 是否使用 c_debuger_qt 窗口

#ifdef C_DEBUG_QT
#define C_DEBUG
#endif // C_DEBUG_QT -- 如果定义了 C_DEBUG_QT -- 则也需要定义 C_DEBUG

//class CDebuger {
SINGLETON_CLASS(CDebuger) {
	SINGLETON(CDebuger)
public:

	void log(std::string str);

	CDebuger& operator << (const std::string& str);

	std::string get_data() const { return data; }

#ifdef C_DEBUG_QT
	class QtDebugWidget* c_debuger_qt;
#endif // C_DEBUG_QT

private:
	std::string data;
	std::ostringstream ss;
	bool save(const std::string& path);
	
	virtual void _init() override;
};
SINGLETON_X(CDebuger)
inline CDebuger& c_debuger() { return CDebuger::get_instance(); }	// 还是这样方便些




