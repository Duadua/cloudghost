#pragma once

#include "delegate.h"
#include <string>
#include <memory>
#include <unordered_map>

class ClassInfo;
class ClassFactory;

#define DECLARE_AUTO_PTR(name)											\
class name;																\
using SPTR_##name = std::shared_ptr<##name##>;							\
using WPTR_##name = std::weak_ptr<##name##>;							\
using UPTR_##name = std::unique_ptr<##name##>;

#define DECLARE_CLASS(name)												\
	private:															\
		static ClassInfo class_info_##name##;							\
	public:																\
		virtual ClassInfo* get_class_info() const;						\
		static SPTR_CObject create_object_##name##();					

// ===========================================================================

DECLARE_AUTO_PTR(CObject)
class CObject {
	DECLARE_CLASS(CObject)
public:
	CObject() {}
	virtual ~CObject() {}

};

// ===========================================================================

DELEGATE0(CObjectConstructor, SPTR_CObject);

#define IMPLEMENT_CINFO(name, func)										\
DELEGATE_BIND_VOID(CObjectConstructor, con_##name##, func);				\
ClassInfo name::class_info_##name((#name), con_##name##);				\
ClassInfo* name::get_class_info() const { return &class_info_##name##; }

#define IMPLEMENT_CLASS(name)											\
IMPLEMENT_CINFO(name, name::create_object_##name##);					\
SPTR_CObject name::create_object_##name##() {							\
	return SPTR_##name##(new name());									\
}																		

// ===========================================================================

class ClassInfo {
public:
	ClassInfo(const std::string& cn, DELEGATE_ICLASS(CObjectConstructor) co);
	virtual ~ClassInfo() {}

	const std::string get_class_name() const;
	DELEGATE_ICLASS(CObjectConstructor) get_constructor();

private:
	std::string class_name;
	DELEGATE_ICLASS(CObjectConstructor) constructor;
};

// ===========================================================================

class ClassFactory {
public:
	static bool register_class(ClassInfo* ci);
	static SPTR_CObject create_object(std::string name);

private:
	ClassFactory() {}

	static std::unordered_map<std::string, ClassInfo*>* class_map;
};

// ===========================================================================

#define CREATE_CLASS(name)												\
std::dynamic_pointer_cast<name>(ClassFactory::create_object((#name)))




