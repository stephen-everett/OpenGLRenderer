#include "Table.h"
#include "AssimpImport.h"
#include <iostream>

Table::Table(EventBus* eventBus) : BusNode(TABLE, eventBus) {
	// initialize variables
	is_interactable = false;
	model = assimpLoad("models/simple-office-table/table_new/table.obj", true);
	model->setOrientation(glm::vec3(0, 1.56, 0));
	model->move(glm::vec3(-6.7, -0.1, 1.2));
	//model->setNorm(false);
	//model->setScale(glm::vec3(0.01, 0.01, 0.01));

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

void Table::render(sf::RenderWindow& window, ShaderProgram& shaderProgram) const {
	model->render(window, shaderProgram);
}

void Table::update() {

}

void Table::onNotify(Event event) {
	switch (event.event_type) {
	case SFML:
		sf::Event ev = get<sf::Event>(event.event);
		if (ev.type == sf::Event::KeyPressed) {
			if (ev.key.scancode == sf::Keyboard::Scan::Up) {
				model->move(glm::vec3(0, 0.1, 0));
				std::cout << model->getPosition().x << ", " << model->getPosition().y << ", " << model->getPosition().z << std::endl;
			}
			if (ev.key.scancode == sf::Keyboard::Scan::Down) {
				model->move(glm::vec3(0, -0.1, 0));
				std::cout << model->getPosition().x << ", " << model->getPosition().y << ", " << model->getPosition().z << std::endl;
			}
			if (ev.key.scancode == sf::Keyboard::Scan::Left) {
				model->move(glm::vec3(0.1, 0, 0));
				std::cout << model->getPosition().x << ", " << model->getPosition().y << ", " << model->getPosition().z << std::endl;
			}
			if (ev.key.scancode == sf::Keyboard::Scan::Right) {
				model->move(glm::vec3(-0.1, 0, 0));
				std::cout << model->getPosition().x << ", " << model->getPosition().y << ", " << model->getPosition().z << std::endl;

			}
			if (ev.key.scancode == sf::Keyboard::Scan::M) {
				model->move(glm::vec3(0, 0, 0.1));
				std::cout << model->getPosition().x << ", " << model->getPosition().y << ", " << model->getPosition().z << std::endl;

			}
			if (ev.key.scancode == sf::Keyboard::Scan::N) {
				model->move(glm::vec3(0, 0, -0.1));
				std::cout << model->getPosition().x << ", " << model->getPosition().y << ", " << model->getPosition().z << std::endl;

			}
			if (ev.key.scancode == sf::Keyboard::Scan::R) {
				model->rotate(glm::vec3(0, 0.01, 0));
				std::cout << model->getOrientation().x << ", " << model->getOrientation().y << ", " << model->getOrientation().z << std::endl;
			}
		}
		break;
	case USER:
		EventEnums user_ev = get<EventEnums>(event.event);
		if (user_ev == UPDATE) {
			update();
		}
		break;
	}
}

void Table::toggleInteractable(bool toggle) {

	/*
	* Statements for debugging, each time interactable toggles from one state
	* to another for the first time, print it
	*/
	if (toggle == true) {
		if (!is_interactable) {
			std::cout << "Table is now interactable" << std::endl;
		}
	}
	// toggle is false, but interactable is true
	else {
		if (is_interactable) {
			std::cout << "Table is now NOT interactable" << std::endl;
		}
	}

	is_interactable = toggle;
}