#pragma once
#include "BusNode.h"
#include "Object3D.h"

class Gman : public BusNode {
public:
	Gman() = delete;
	Gman(EventBus* eventBus);
	void update();
	void render(sf::RenderWindow& window, ShaderProgram& shaderProgram) const;
protected:
	void onNotify(Event event);
	std::shared_ptr<Object3D> model;
};