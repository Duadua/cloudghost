#pragma once

class SceneComponent;

class GameObject {
public:
	GameObject(SceneComponent* root = nullptr);
	~GameObject();

	void set_root(SceneComponent* root);
	SceneComponent* get_root();

	void draw();

protected:
	SceneComponent * root_component;
};