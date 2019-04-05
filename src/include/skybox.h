#pragma once

#include "gameobject.h"

 PRE_DECLARE_CLASS(Texture3D)
 USING_SPTR(Texture3D)

class SkyBox : public GameObject {
	DECLARE_CLASS(SkyBox)
public:
	SkyBox();
	virtual ~SkyBox() {}

	virtual void begin_play();
	
	GET_SET(SPTR_Texture3D, texture)

private:
	SPTR_Texture3D texture;

};
DECLARE_AUTO_PTR(SkyBox)
