#include "Table.h"
#include "AssimpImport.h"
#include "Bunny.h"
#include <iostream>

Table::Table(EventBus* eventBus) : BusNode(TABLE, eventBus) {
	// initialize variables
	is_interactable = false;
	model = assimpLoad("models/simple-office-table/table_new/table.obj", true);
	model->setOrientation(glm::vec3(0, 1.56, 0));
	model->move(glm::vec3(-6.7, -0.1, 1.2));
	model->setNorm(false);
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

	// load bunnies
	bunny1 =  assimpLoad("models/bunny/bunny_textured.obj", true);
	bunny1->move(glm::vec3(-6.11122, 0.7, 0.996469));
	bunny1->setOrientation(glm::vec3(0, -1.56, 0));
	bunny2 = assimpLoad("models/bunny/bunny_textured.obj", true);
	bunny2_start_pos = glm::vec3(-7, 0.7, 0.996469);
	bunny2->move(bunny2_start_pos);
	bunny2->setOrientation(glm::vec3(0, -1.56, 0));

	// register bunnies with renderer
	std::shared_ptr<struct Event>  reg_bunny1 = std::make_shared<Event>(USER, R_REGISTER, RENDERER, static_cast<void*>(bunny1.get()));
	eventBus->sendMessage(reg_bunny1);
	std::shared_ptr<struct Event>  reg_bunny2 = std::make_shared<Event>(USER, R_REGISTER, RENDERER, static_cast<void*>(bunny2.get()));
	eventBus->sendMessage(reg_bunny2);

	// set bunny physical properties
	bunny1_properties = PhysicsStructure(5, glm::vec3(0), glm::vec3(0));
	bunny2_properties = PhysicsStructure(2, glm::vec3(0), glm::vec3(0));
	simulation_running = false;
	table_mu = 0.0000001;
}

void Table::render(sf::RenderWindow& window, ShaderProgram& shaderProgram) const {
	model->render(window, shaderProgram);
}

void Table::update() {
	tick_simulation();
}

void Table::start_simulation() {
	time = c.getElapsedTime();
	bunny2_properties.acceleration = glm::vec3(0.0001, 0, 0);
	simulation_running = true;
}

void Table::tick_simulation() {
	if (simulation_running) {
		sf::Time now = c.getElapsedTime();
		auto diff = now - time;
		bunny2_properties.acceleration += glm::vec3(-(bunny2_properties.mass * table_mu),0,0);
		bunny2_properties.velocity += bunny2_properties.acceleration * diff.asSeconds();
		if (bunny2_properties.velocity.x <= 0) {
			simulation_running = false;
		}
		bunny2->move(bunny2_properties.velocity);
	}
}

void Table::restart_simulation() {
	simulation_running = false;
	bunny2->move(-bunny2->getPosition());
	bunny2->move(bunny2_start_pos);
	bunny2_properties.acceleration = glm::vec3(0);
	bunny2_properties.velocity = glm::vec3(0);
}

void Table::onNotify(Event event) {
	switch (event.event_type) {
	case SFML:
		if (is_interactable) {
			sf::Event ev = get<sf::Event>(event.event);
			if (ev.type == sf::Event::KeyPressed) {
				if (ev.key.scancode == sf::Keyboard::Scan::C) {
					model->setTBN(!model->hasTBN());
					if (model->hasTBN()) {
						std::cout << "Table is now using TBN with Normal Map" << std::endl;
					}
					else {
						std::cout << "Table is NOT using TBN with Normal Map" << std::endl;
					}
				}
				else if (ev.key.scancode == sf::Keyboard::Scan::V) {
					model->setSpec(!model->hasSpec());
					if (model->hasSpec()) {
						std::cout << "Table is now using Spec Map" << std::endl;
					}
					else {
						std::cout << "Table is NOT using Spec Map" << std::endl;
					}
				}
				else if (ev.key.scancode == sf::Keyboard::Scan::B) {
					model->setNorm(!model->hasNorm());
					if (model->hasNorm()) {
						std::cout << "Table is now using Normal Map" << std::endl;
					}
					else {
						std::cout << "Table is NOT using Normal Map" << std::endl;
					}
				}
				else if (ev.key.scancode == sf::Keyboard::Scan::Up) {
					bunny1->move(glm::vec3(0, .01, 0));
					std::cout << bunny1->getPosition().x << ", " << bunny1->getPosition().y << ", " << bunny1->getPosition().z << std::endl;
				}
				else if (ev.key.scancode == sf::Keyboard::Scan::E) {
					if (!simulation_running) {
						start_simulation();
					}
				}
				else if (ev.key.scancode == sf::Keyboard::Scan::R) {
					restart_simulation();
				}
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