#pragma once

#include "gamemanager.h"


class MyGameManager : public GameManager {
public:

	virtual void pre_init();
	virtual void post_init();

	virtual void pre_draw();
	virtual void post_draw();

private:

};
