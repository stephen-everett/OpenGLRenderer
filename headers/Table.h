#pragma once
#include "BusNode.h"
#include "Object3D.h"
#include "Interactable.h"
#include "PhysicsStructures.h"

class Table : public BusNode {
public:
	Table() = delete;
	Table(EventBus* eventBus);
	void update();
	void render(sf::RenderWindow& window, ShaderProgram& shaderProgram) const;

	bool is_interactable;

	std::shared_ptr<Interactable> interactor;

protected:
	void onNotify(Event event);
	std::shared_ptr<Object3D> model;
	std::shared_ptr<Object3D> bunny1;
	std::shared_ptr<Object3D> bunny2;
	void toggleInteractable(bool);
private:
	float table_mu;
	//PhysicsStructure bunny1_properties;
	//PhysicsStructure bunny2_properties;

};