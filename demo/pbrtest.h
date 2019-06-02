#pragma once

#include "gameobject.h"

PRE_DECLARE_CLASS(Material)
USING_SPTR(Material)

class PBRTest : public GameObject {
	DECLARE_CLASS(PBRTest)
public:
	PBRTest();
	virtual ~PBRTest() {}

	virtual void begin_play();

	void init(int r = 10, int c = 10, float s = 0.5f, float sz = 1.0f);

	void set_material(SPTR_Material name, uint rid = 0);

private:
	int row;
	int col;
	float space;
	float size;

};
DECLARE_AUTO_PTR(PBRTest)

