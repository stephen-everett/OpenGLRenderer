#pragma once
#include "BusNode.h"
#include "Object3D.h"
#include "Interactable.h"
#include "PhysicsStructures.h"
#include "Animator.h"

class GameTable : public BusNode {
public:
	GameTable() = delete;
	GameTable(EventBus* eventBus);
	void update();
	void render(sf::RenderWindow& window, ShaderProgram& shaderProgram) const;

	bool is_interactable;

	std::shared_ptr<Interactable> interactor;

protected:
	void onNotify(Event event);
	std::shared_ptr<Object3D> model;
	std::shared_ptr<Object3D> bunny1;
	//std::shared_ptr<Object3D> bunny2;
	std::shared_ptr<Object3D> cup1;
	std::shared_ptr<Object3D> cup2;
	std::shared_ptr<Object3D> cup3;
	void toggleInteractable(bool);
private:
	float table_mu;
	//PhysicsStructure bunny1_properties;
	//PhysicsStructure bunny2_properties;
	
	Animator animator;
	sf::Time time;
	sf::Clock c;
	bool isAnimated;
	void playAnimation();
	float elapsedTime;
};