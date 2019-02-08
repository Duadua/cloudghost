#include "reflex.h"
#include <unordered_map>

// ===========================================================================

ClassInfo::ClassInfo(const std::string& cn, DELEGATE_ICLASS(CObjectConstructor) co) 
	: class_name(cn), constructor(co) {
	ClassFactory::register_class(this);
}

const std::string ClassInfo::get_class_name() const { return class_name; }
DELEGATE_ICLASS(CObjectConstructor) ClassInfo::get_constructor() { return constructor; }

// ===========================================================================

std::unordered_map<std::string, ClassInfo*>* ClassFactory::class_map = nullptr;

bool ClassFactory::register_class(ClassInfo* ci) {
	if (class_map == nullptr) class_map = new std::unordered_map<std::string, ClassInfo*>();
	if (ci == nullptr) return false;
	
	std::string t_name = ci->get_class_name();
	if (class_map->find(t_name) == class_map->end()) { class_map->insert(std::make_pair(t_name, ci)); }
	return true;

}
SPTR_CObject ClassFactory::create_object(std::string name) {
	if (class_map->find(name) == class_map->end()) { return nullptr; }
	if ((*class_map)[name]->get_constructor() == nullptr) { return nullptr; }
	return (*class_map)[name]->get_constructor()->invoke();
}

// =========================================================================

IMPLEMENT_CLASS(CObject)


