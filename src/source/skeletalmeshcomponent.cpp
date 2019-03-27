#include "shader.h"
#include "animplayer.h"
#include "animsequence.h"
#include "skeletalmesh.h"
#include "assetmanager.h"
#include "skeletalmeshcomponent.h"

IMPLEMENT_CLASS(SkeletalMeshComponent)

SkeletalMeshComponent::SkeletalMeshComponent() { 
	mesh = nullptr; anim = nullptr; 
	anim_player = CREATE_CLASS(AnimPlayer); 
	anim_player->init();
}
SkeletalMeshComponent::~SkeletalMeshComponent() { }

void SkeletalMeshComponent::begin_play() {
	if (!anim_player) { anim_player = CREATE_CLASS(AnimPlayer); anim_player->init(); }
}
void SkeletalMeshComponent::tick(float time) {
	if(anim_player) anim_player->play(anim, mesh, time);
}
void SkeletalMeshComponent::draw(const std::string& shader) {
	if (mesh != nullptr) {
		auto t_shader = AssetManager_ins().get_shader(shader);
		if (t_shader != nullptr) {
			if (is_border) {
				t_shader->set_mat4("u_model", get_transform().scale(1.1f));
			}
			else t_shader->set_mat4("u_model", get_transform());
		}
		mesh->draw(shader);
	}
}

