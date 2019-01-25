#pragma once

#include "shader.h"

class SceneComponent;

class GameObject {
public:
	GameObject(SceneComponent* root = nullptr);
	virtual ~GameObject();

	void set_root(SceneComponent* root);
	SceneComponent* get_root();

	virtual void draw(Shader& shader);

protected:
	SceneComponent * root_component;
};