#pragma once
#include <memory>

// array 使用 shared_ptr -- 需要手动指定删除器
template<class T>
inline std::shared_ptr<T> make_shared_array(size_t size) { return std::shared_ptr<T>(new T[size], std::default_delete<T[]>()); }
