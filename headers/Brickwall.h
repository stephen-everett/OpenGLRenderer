#pragma once
#include "BusNode.h"
#include "Object3D.h"

class Brickwall : public BusNode {
public:
	Brickwall() = delete;
	Brickwall(EventBus* eventBus);
	void update();
	//void render(sf::RenderWindow& window, ShaderProgram& shaderProgram) const;
protected:
	void onNotify(Event event);
	Texture texture;
	std::shared_ptr<Object3D> model;
private:
	//Animator animator;
	//sf::Time time;
	//sf::Clock c;
	//bool isAnimated;
	//void playAnimation();
	//float elapsedTime;
};