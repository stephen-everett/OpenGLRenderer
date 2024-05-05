#pragma once
#include "BusNode.h"
#include "Object3D.h"
#include "Texture.h"

class Room : public BusNode {
public:
	Room() = delete;
	Room(EventBus* eventBus);
	void update();

protected:
	void onNotify(Event event);
	Texture texture;
	std::shared_ptr<Object3D> model;
};