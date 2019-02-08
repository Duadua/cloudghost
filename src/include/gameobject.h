#pragma once

#include "shader.h"
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

	virtual void draw(SPTR_Shader shader);

	virtual void bind_input();
protected:
	SPTR_SceneComponent root_component;
};
DECLARE_AUTO_PTR(GameObject)
