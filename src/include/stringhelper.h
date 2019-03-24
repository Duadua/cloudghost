#pragma once

#include "singleton.h"
#include "typehelper.h"
#include <string>
#include <memory>

SINGLETON_CLASS(StringHelper) {
	SINGLETON(StringHelper)
public:

	// array 使用 shared_ptr -- 需要手动指定删除器
	template<class T>
	inline std::shared_ptr<T> make_shared_array(size_t size) { return std::shared_ptr<T>(new T[size], std::default_delete<T[]>()); }
	
	std::string int_to_string(int ui);
	std::string uint_to_string(uint ui);
	std::string char_to_string(char c);

private:
	
};
SINGLETON_X(StringHelper)

