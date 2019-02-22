#pragma once

#include "reflex.h"
#include "scenecomponent.h"
#include <memory>

class GameObject : public CObject {
	DECLARE_CLASS(GameObject)
public:
	GameObject(SPTR_SceneComponent root = nullptr);
	virtual ~GameObject();

	GET_SET(SPTR_SceneComponent, root_component);

	virtual void bind_input();
	virtual void begin_play();
	virtual void tick();
	virtual void draw(const std::string& shader);

protected:
	SPTR_SceneComponent root_component;
};
DECLARE_AUTO_PTR(GameObject)
