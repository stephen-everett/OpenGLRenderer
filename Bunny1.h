#pragma once
#include "BusNode.h"
#include "Object3D.h"
#include "Animator.h"

class Bunny1 : public BusNode {
public:
	Bunny1() = delete;
	Bunny1(EventBus* eventBus);
	void update();
	void render(sf::RenderWindow& window, ShaderProgram& shaderProgram) const;
protected:
	void onNotify(Event event);
	std::shared_ptr<Object3D> model;
private:
	Animator animator;
	sf::Time time;
	sf::Clock c;
	bool isAnimated;
	void playAnimation();
	float elapsedTime;
};