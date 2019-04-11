#pragma once

#include "gamemanager.h"

class MyGameManager : private GameManager {
public:

	virtual void load_asset() override;
	virtual void begin_play() override;
	virtual void tick(float time) override { time; }
	virtual void map_input() override;

private:

};

MyGameManager gm;
