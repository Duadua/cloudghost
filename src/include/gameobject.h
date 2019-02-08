#pragma once

#include "shader.h"
#include "reflex.h"
#include <memory>

class SceneComponent;

class GameObject : public CObject {
	DECLARE_CLASS(GameObject)
public:
	GameObject(SceneComponent* root = nullptr);
	virtual ~GameObject();

	void set_root(SceneComponent* root);
	SceneComponent* get_root();

	virtual void draw(SPTR_Shader shader);

	virtual void bind_input();
protected:
	SceneComponent * root_component;
};
DECLARE_AUTO_PTR(GameObject)
