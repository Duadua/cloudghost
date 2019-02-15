#pragma once

#include "reflex.h"
#include "scenecomponent.h"
#include <memory>

class GameObject : public CObject {
	DECLARE_CLASS(GameObject)
public:
	GameObject(SPTR_SceneComponent root = nullptr);
	virtual ~GameObject();

	void set_root(SPTR_SceneComponent root);
	SPTR_SceneComponent get_root();

	virtual void bind_input();
	virtual void begin_play();
	virtual void tick();
	virtual void draw(const std::string& shader);

protected:
	SPTR_SceneComponent root_component;
};
DECLARE_AUTO_PTR(GameObject)
