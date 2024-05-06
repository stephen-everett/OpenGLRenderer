#pragma once
#include "BusNode.h"
#include "Object3D.h"
#include "Animator.h"

class AnimBunny : public BusNode {
public:
	AnimBunny() = delete;
	AnimBunny(EventBus* eventBus);
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
};