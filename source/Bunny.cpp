#include "Bunny.h"
#include "AssimpImport.h"
#include <iostream>

Bunny::Bunny(EventBus* eventBus) : BusNode(BUNNY, eventBus) {
	// initialize variables
	is_interactable = false;
	model = assimpLoad("models/bunny/bunny_textured.obj", true);
	model->move(glm::vec3(0, 0, -3));
	//model->setScale(glm::vec3(0.1, 0.1, 0.1));

	// register with renderer
	std::shared_ptr<struct Event>  register_object = std::make_shared<Event>(USER, R_REGISTER, RENDERER, static_cast<void*>(model.get()));
	eventBus->sendMessage(register_object);

	// register with interactor
	glm::vec3 test_pos = model->getPosition();
	interactor = std::make_shared<Interactable>(
		&model->getPosition(), 
		[this](bool value) {
			return this->toggleInteractable(value);
		}
	);
	std::shared_ptr<struct Event>  register_interactor = std::make_shared<Event>(USER, I_REGISTER, INTERACTOR, static_cast<void*>(interactor.get()));
	eventBus->sendMessage(register_interactor);
}

void Bunny::render(sf::RenderWindow& window, ShaderProgram& shaderProgram) const {
	model->render(window, shaderProgram);
}

void Bunny::update() {

}

void Bunny::onNotify(Event event){
	switch (event.event_type) {
	case SFML:
		break;
	case USER:
		EventEnums ev = get<EventEnums>(event.event);
		if (ev == UPDATE) {
			update();
		}
		break;
	}
}

void Bunny::toggleInteractable(bool toggle) {

	/*
	* Statements for debugging, each time interactable toggles from one state
	* to another for the first time, print it
	*/
	if (toggle == true) {
		if (!is_interactable) {
			std::cout << "Bunny is now interactable" << std::endl;
		}
	}
	// toggle is false, but interactable is true
	else {
		if (is_interactable) {
			std::cout << "Bunny is now NOT interactable" << std::endl;
		}
	}

	is_interactable = toggle;
}