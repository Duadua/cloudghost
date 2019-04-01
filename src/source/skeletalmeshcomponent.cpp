#include "shader.h"
#include "animplayer.h"
#include "animsequence.h"
#include "skeletalmesh.h"
#include "assetmanager.h"
#include "skeletalmeshcomponent.h"

IMPLEMENT_CLASS(SkeletalMeshComponent)

SkeletalMeshComponent::SkeletalMeshComponent() { 
	mesh = nullptr; anim = nullptr; 
	anim_player = nullptr;
}
SkeletalMeshComponent::~SkeletalMeshComponent() { }

void SkeletalMeshComponent::begin_play() {
	if (!anim_player) { anim_player = CREATE_CLASS(AnimPlayer); anim_player->init(anim, mesh); }
	anim_player->set_is_looping(true);
	//anim_player->set_init_position(0.5f);
}
void SkeletalMeshComponent::tick(float time) {
	// test anim_player
	/*static float i_time = time;
	static bool f1 = true;
	if (f1 && time - i_time > 5000.0f) {
		//if (anim_player) anim_player->stop();
		if (anim_player) anim_player->pause();
		f1 = false;
	}
	static bool f2 = true;
	if (f2 && time - i_time > 10000.0f) {
		if (anim_player) anim_player->start();
		f2 = false;
	}
	*/
	
	if(anim_player) anim_player->play(time);
}
void SkeletalMeshComponent::draw(SPTR_Shader shader) {
	if (mesh != nullptr) {
		if (shader != nullptr) {
			if (is_border) {
				shader->set_mat4("u_model", get_transform().scale(1.1f));
			}
			else shader->set_mat4("u_model", get_transform());
		}
		mesh->draw(shader);
	}
}

