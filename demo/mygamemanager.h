#pragma once

#include "gamemanager.h"

class MyGameManager : private GameManager {
public:

	virtual void load_asset() override;
	virtual void begin_play(QOpenGLWidget* gl) override;
	virtual void tick() override;
	virtual void map_input() override;

private:

};
