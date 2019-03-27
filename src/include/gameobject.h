#pragma once

#include "reflex.h"
#include "typehelper.h"
#include <memory>

PRE_DECLARE_CLASS(SceneComponent)
USING_SPTR(SceneComponent)

class GameObject : public CObject {
	DECLARE_CLASS(GameObject)
public:
	GameObject(SPTR_SceneComponent root = nullptr);
	virtual ~GameObject() {}

    GET_SET(SPTR_SceneComponent, root_component)

    GET(uint, id)
	static uint get_num() { return num; }

	virtual void bind_input() {}
	virtual void begin_play() {}
	virtual void tick(float time) { time; }
	virtual void draw(const std::string& shader) { shader; }

	void _begin_play();						// inner begin_play
	void _tick(float time);					// inner tick
	void _draw(const std::string& shader);	// inner draw

protected:
	SPTR_SceneComponent root_component;

	uint id;			// pick 用
	static uint num;	// 数量
};
DECLARE_AUTO_PTR(GameObject)
