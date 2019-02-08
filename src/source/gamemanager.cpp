#include "gameobject.h"
#include "gamemanager.h"
#include <assert.h>

GameManager* GameManager::instance = nullptr;
GameManager* GameManager::get_instance() {
	assert(instance);
	return instance;

}
GameManager::GameManager() {
	assert(instance == nullptr);
	instance = this;
	game_objects.clear();
}
GameManager::~GameManager() {
	assert(instance == this);
	instance = nullptr;
}

void GameManager::init() {
	
}

void GameManager::draw() {

}

void GameManager::add_game_object(const QString& key, SPTR_GameObject value) {
	if (game_objects.count(key)) return;
	game_objects.insert(key, value);
}
