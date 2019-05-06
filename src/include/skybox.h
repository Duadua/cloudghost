#pragma once

#include "gameobject.h"

 PRE_DECLARE_CLASS(Texture3D)
 USING_SPTR(Texture3D)
 PRE_DECLARE_CLASS(Texture2D)
 USING_SPTR(Texture2D)

class SkyBox : public GameObject {
	DECLARE_CLASS(SkyBox)
public:
	SkyBox();
	virtual ~SkyBox() {}

	virtual void begin_play();
	
	void set_texture_hdr(SPTR_Texture2D tex);
	GET(SPTR_Texture2D, texture_hdr)

	GET_SET(bool, b_use_hdr)
	GET_SET(SPTR_Texture3D, texture)
private:
	SPTR_Texture3D texture;

	SPTR_Texture2D texture_hdr;				// 使用 hdr 的 cube_map
	bool b_use_hdr;

};
DECLARE_AUTO_PTR(SkyBox)
