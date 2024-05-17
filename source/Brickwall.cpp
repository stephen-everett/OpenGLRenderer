#include "Brickwall.h"
#include "Mesh3D.h"
#include "Texture.h"
#include "AssimpImport.h"
#include <iostream>

Brickwall::Brickwall(EventBus* eventBus) : BusNode(BRICKWALL, eventBus) {
    
    // This block of code uses assimp to load a pre-defined mesh
    /*
    model = assimpLoad("models/brick-wall-segment-02-tilable/model.obj", true);
    if (model) {
        std::cout << "Brickwall model loaded successfully." << std::endl;
        model->move(glm::vec3(0, 0, -5));
		model->setScale(glm::vec3(0.02f, 0.02f, 0.02f));
        std::shared_ptr<struct Event> register_object = std::make_shared<Event>(USER, R_REGISTER, RENDERER, static_cast<void*>(model.get()));
        eventBus->sendMessage(register_object);
    }
    else {
        std::cerr << "Failed to load brickwall model." << std::endl;
    }
    */



    // manually create plane and use textures provides by learnopngl
	sf::Image image;
	image.loadFromFile("models/logl_brickwall/brickwall.jpg");
	texture = texture.loadImage(image, "baseTexture");

	sf::Image normal_map;
	normal_map.loadFromFile("models/logl_brickwall/brickwall_normal.jpg");
	normalMap = normalMap.loadImage(normal_map, "normalMap");

	std::vector<Texture> textures;
	textures.push_back(texture);
	textures.push_back(normalMap);

	//std::shared_ptr <Mesh3D> mesh = std::make_shared<Mesh3D>(Mesh3D::cube(texture));
	Mesh3D my_mesh = Mesh3D::square(std::move(textures));
	std::vector<Mesh3D> meshes;
	meshes.push_back(my_mesh);

	//Object3D myObject(std::move(meshes));
	model = std::make_shared<Object3D>(std::move(meshes));
	model->move(glm::vec3(-10.1716, 0.171622, 2.5));
	model->setNorm(true);
	model->setSpec(true);
	model->setTBN(false);

	model->setScale(glm::vec3(5.0f, 5.0f, 5.0f));
	std::shared_ptr<struct Event>  register_object = std::make_shared<Event>(USER, R_REGISTER, RENDERER, static_cast<void*>(model.get()));
	eventBus->sendMessage(register_object);

	// set up interactable
	is_interactable = false;

	interactor = std::make_shared<Interactable>(
		&model->getPosition(),
		[this](bool value) {
			return this->toggleInteractable(value);
		}
	);
	std::shared_ptr<struct Event>  register_interactor = std::make_shared<Event>(USER, I_REGISTER, INTERACTOR, static_cast<void*>(interactor.get()));
	eventBus->sendMessage(register_interactor);
}

void Brickwall::onNotify(Event event) {
	switch (event.event_type) {
	case SFML:
		if (is_interactable) {
			sf::Event ev = get<sf::Event>(event.event);
			if (ev.type == sf::Event::KeyPressed) {
				if (ev.key.scancode == sf::Keyboard::Scan::C) {
					model->setTBN(!model->hasTBN());
					if (model->hasTBN()) {
						std::cout << "Wall is now using TBN with Normal Map" << std::endl;
					}
					else {
						std::cout << "Wall is NOT using TBN with Normal Map" << std::endl;
					}
				}
				else if (ev.key.scancode == sf::Keyboard::Scan::V) {
					model->setSpec(!model->hasSpec());
					if (model->hasSpec()) {
						std::cout << "Wall is now using Spec Map" << std::endl;
					}
					else {
						std::cout << "Wall is NOT using Spec Map" << std::endl;
					}
				}
				else if (ev.key.scancode == sf::Keyboard::Scan::B) {
					model->setNorm(!model->hasNorm());
					if (model->hasNorm()) {
						std::cout << "Wall is now using Normal Map" << std::endl;
					}
					else {
						std::cout << "Wall is NOT using Normal Map" << std::endl;
					}
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
void Brickwall::update() {}

void Brickwall::toggleInteractable(bool toggle) {

	/*
	* Statements for debugging, each time interactable toggles from one state
	* to another for the first time, print it
	*/
	if (toggle == true) {
		if (!is_interactable) {
			std::cout << "Wall is now interactable" << std::endl;
		}
	}
	// toggle is false, but interactable is true
	else {
		if (is_interactable) {
			std::cout << "Wall is now NOT interactable" << std::endl;
		}
	}

	is_interactable = toggle;
}
