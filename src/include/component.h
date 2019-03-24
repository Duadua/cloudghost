#pragma once

#include "reflex.h"

class Component : public CObject {
	DECLARE_CLASS(Component)
public:
	Component();
	virtual ~Component() override;

private:
	
	
};
DECLARE_AUTO_PTR(Component)
