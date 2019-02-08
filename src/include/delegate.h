#pragma once

#include <memory>

#define DELEGATE_ICLASS(class_name)	std::shared_ptr<I##class_name##Handler>

#define	DELEGATE_BIND(class_name, type, name, obj, func)				\
std::shared_ptr<class_name<type> > name(new class_name<type>(obj, func))

#define DELEGATE_BIND_VOID(class_name, name, func)						\
std::shared_ptr<class_name<void> > name(new class_name<void>(func))

#define DELEGATE_DECLARE(class_name,return_type, params, args)			\
class I##class_name##Handler {											\
public:																	\
	virtual ~I##class_name##Handler() {}								\
	virtual return_type invoke(params) = 0;								\
};																		\
template<typename T>													\
class class_name : public I##class_name##Handler {						\
public:																	\
	class_name(T* t, return_type (T::*f)(params)) : type(t), func(f) {}	\
	return_type invoke(params) { return (type->*func)(args); }			\
private:																\
	T* type;															\
	return_type (T::*func)(params);										\
};																		\
template<>																\
class class_name<void> : public I##class_name##Handler {				\
public:																	\
	class_name(return_type(*f)(params)) : func(f) {}					\
	return_type invoke(params) { return (*func)(args); }				\
private:																\
	return_type(*func)(params);											\
}																		

#define DELEGATE_PARAMS(...) __VA_ARGS__
#define DELEGATE_ARGS(...) __VA_ARGS__

#define DELEGATE0(class_name, return_type)								\
	DELEGATE_DECLARE(class_name, return_type, DELEGATE_PARAMS(void), )

#define DELEGATE1(class_name, return_type, p1)							\
DELEGATE_DECLARE(class_name, return_type,								\
	DELEGATE_PARAMS(p1 a),												\
	DELEGATE_ARGS(a)													\
)					

#define DELEGATE2(class_name, return_type, p1, p2)						\
DELEGATE_DECLARE(class_name, return_type,								\
	DELEGATE_PARAMS(p1 a, p2 b),										\
	DELEGATE_ARGS(a, b)													\
)					
