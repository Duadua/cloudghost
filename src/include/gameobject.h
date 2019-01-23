#pragma once

class SceneComponent;

class GameObject {
public:
	GameObject();
	~GameObject();
protected:
	SceneComponent * root_component;
};