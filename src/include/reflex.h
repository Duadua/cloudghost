#pragma once

#include "delegate.h"
#include <map>
#include <string>
#include <memory>
#include <unordered_map>

using ulong = unsigned long;
using uchar = unsigned char;

class ClassInfo;
class ClassFactory;

#define DECLARE_AUTO_PTR(name)											\
class name;																\
using SPTR_##name = std::shared_ptr<name>;							\
using WPTR_##name = std::weak_ptr<name>;							\
using UPTR_##name = std::unique_ptr<name>;

#define DECLARE_CLASS(name)												\
	private:															\
        static ClassInfo class_info_##name;							\
	public:																\
		static ClassInfo* get_class_info();								\
        static SPTR_CObject create_object_##name();

#define ATTR_OFFSET(class_name, attr_key)								\
	((unsigned long)(&((class_name*)0)->attr_key))									// 黑科技 

#define DECLARE_ATTR(access, attr_type, attr_key, class_name)			\
access:																	\
	attr_type attr_key;													\
private:																\
    class ClassAttrRegister_##attr_key{								\
	public:																\
        ClassAttrRegister_##attr_key() {								\
            static ClassAttrRegister reg_##attr_key(					\
				class_name::get_class_info(),							\
				ATTR_OFFSET(class_name, attr_key),						\
				#attr_type,												\
				#attr_key												\
			);															\
		}																\
    }attr_key##_register

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
DELEGATE_BIND_VOID(CObjectConstructor, con_##name, func);				\
ClassInfo name::class_info_##name((#name), con_##name);					\
ClassInfo* name::get_class_info() { return &class_info_##name; }

#define IMPLEMENT_CLASS(name)											\
IMPLEMENT_CINFO(name, name::create_object_##name);						\
SPTR_CObject name::create_object_##name() {								\
    return SPTR_##name(new name());										\
}																		

// ===========================================================================

// 类成员变量
class ClassAttr {
public:
	ClassAttr() {}
	ClassAttr(ulong o, std::string t, std::string k) : offset(o), type(t), key(k) {}

	template<class class_type, class attr_type>
	void get(class_type* t, attr_type& v) { v = *((attr_type*)((uchar*)t + offset)); }

	template<class class_type, class attr_type>
	void set(class_type* t, const attr_type& v) { *((attr_type*)((uchar*)t + offset)) = v; }

	std::string get_key() const { return key; }
	std::string get_type() const { return type; }

private:
	ulong offset;
	std::string type;
	std::string key;
};

class ClassInfo {
public:
	ClassInfo(const std::string& cn, DELEGATE_ICLASS(CObjectConstructor) co);
	virtual ~ClassInfo() {}

	const std::string get_class_name() const;
	DELEGATE_ICLASS(CObjectConstructor) get_constructor();

	std::map<std::string, ClassAttr> get_attrs() { return map_attrs; }
	ClassAttr get_attr(std::string key) { return map_attrs[key]; }
	void add_attr(const ClassAttr& a) { map_attrs.insert(std::make_pair(a.get_key(), a)); }
private:
	std::string class_name;
	DELEGATE_ICLASS(CObjectConstructor) constructor;

	std::map<std::string, ClassAttr> map_attrs;
};

class ClassAttrRegister {
public:
	ClassAttrRegister(ClassInfo* class_info, ulong offset, std::string type, std::string key) {
		ClassAttr attr(offset, type, key); class_info->add_attr(attr);
	}
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


// ===========================================================================

#define GET(type, name)										\
public:														\
	type get_##name##() { return name; }
#define SET(type, name)										\
public:														\
	void set_##name##(const type& t_name) { name = t_name; }

#define GET_SET(type, name)									\
public:														\
	void set_##name##(const type& t_name) { name = t_name; }\
	type get_##name##() { return name; }


