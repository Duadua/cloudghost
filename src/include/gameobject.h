#pragma once

#include "reflex.h"
#include "scenecomponent.h"
#include <memory>

class GameObject : public CObject {
	DECLARE_CLASS(GameObject)
public:
	GameObject(SPTR_SceneComponent root = nullptr);
	virtual ~GameObject();

    GET_SET(SPTR_SceneComponent, root_component)

    GET(uint, id)
	static uint get_num() { return num; }

	virtual void bind_input();
	virtual void begin_play();
	virtual void tick();
	virtual void draw(const std::string& shader);

protected:
	SPTR_SceneComponent root_component;

	uint id;			// pick 用
	static uint num;	// 数量
};
DECLARE_AUTO_PTR(GameObject)
