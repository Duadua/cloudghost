#pragma once

#include "gameobject.h"
#include <QMap>
#include <memory>

class GameanagerImpl {
public:
	virtual ~GameanagerImpl() {}

	virtual void pre_init() = 0;
	virtual void post_init() = 0;

	virtual void pre_draw() = 0;
	virtual void post_draw() = 0;

};


class GameManager : public GameanagerImpl {
public:

	GameManager();
	virtual ~GameManager();

	static GameManager* get_instance();

	void init();
	void draw();

	void add_game_object(const QString& key, SPTR_GameObject value);

protected:
	static GameManager* instance;
	
private:
	QMap<QString, SPTR_GameObject> game_objects;
};
