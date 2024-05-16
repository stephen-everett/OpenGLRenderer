#pragma once
#include "BusNode.h"
#include "Object3D.h"
#include "Interactable.h"

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
	void toggleInteractable(bool);
};